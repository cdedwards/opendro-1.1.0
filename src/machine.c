///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Virtual machine object
//
// Copyright (C) 2008-2011 ShumaTech http://www.shumatech.com/
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "machine.h"
#include "axis.h"
#include "scale.h"
#include "display.h"
#include "keypad.h"
#include "drolim.h"
#include "function.h"
#include "config.h"
#include "timer.h"
#include "tach.h"
#include "edge.h"
#include "assert.h"
#include "trace.h"
#include "luash.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

/// Special value to indicate display is off
#define DISPLAY_OFF         UINT8_MAX

/// Special value to indicate no zero line
#define ZERO_OFF            UINT8_MAX

/// Special value to indicate no edge finder
#define EDGE_NONE           UINT8_MAX

/// Special value to indicate tachometer as the edge finder
#define EDGE_TACH           (UINT8_MAX - 1)

/// Special value to indicate no beeper
#define BEEPER_NONE         UINT8_MAX

/// Tachometer display update rate
#define TACH_UPDATE_HZ      2

/// Allowed number of consecutive zero readings
#define TACH_ZERO_COUNT     4

/// Maximum tach divisor
#define TACH_DIVISOR_MAX    100

/// Feed rate display update rate
#define FEED_UPDATE_HZ      2

/// Number of times to cycle the beeper for a zero warning
#define WARN_BEEP_COUNT     5

/// Beeper cycle period for a zero warning
#define WARN_BEEP_PERIOD    (SEC_TICKS / 4)

/// Edge beep time
#define EDGE_BEEP_TIME      SEC_TICKS

/// Machine display mode
typedef enum display
{
    DISPLAY_READING,                ///< Display the axis readings 
    DISPLAY_COUNTS,                 ///< Display raw scale counts
    DISPLAY_JITTER,                 ///< Display scale jitter
    DISPLAY_MAX,                    ///< MUST be last!!! 
} display_t;

/// Machine object
typedef struct machine
{
    uint8_t         machine_num;    ///< Global machine number (0..MACHINE_MAX-1)
    machine_type_t  type;           ///< Machine type (mill, lathe, etc.)
    axis_units_t    units;          ///< Display units (inch, mm, etc.)
    axis_zero_t     zero;           ///< Absolute or incremental zero
    uint8_t         zero_line;      ///< Set to the line zero operation is pending on
    uint8_t         axis[DISPLAY_LINE_MAX]; ///< Axis each display line is assigned to
    display_t       display;        ///< Display mode
    uint8_t         edge_num;       ///< Edge finder input assigned to this machine
    uint8_t         tach_num;       ///< Tachometer input assigned to this machine
    uint8_t         tach_line;      ///< Display line to show tachometer on
    bool            tach_surface;   ///< Set to display surface rate instead of RPM
    uint8_t         tach_divisor;   ///< Divisor to divide the tachometer input by
    double          warn_thresh;    ///< Threshold to display zero warning
    axis_units_t    warn_units;     ///< Zero warning units
    uint8_t         feed_line;      ///< Line the feed rate is displayed on
    uint8_t         feed_axis;      ///< Axis the feed rate is calculated for
    uint8_t         beeper;         ///< DPU-550: Tachometer port beeper is on
                                    ///< DRO-550: Beeper enable
    uint32_t        auto_off;       ///< Auto-off time in seconds
    double          auto_on;        ///< Auto-on scale distance
    axis_units_t    auto_units;     ///< Auto-on units
    bool            edge_beep;      ///< Beep on edge detection
    bool            prompt;         ///< Display prompts
} machine_t;

typedef enum machine_vid
{
    VID_TYPE,                       ///< Machine type
    VID_UNITS,                      ///< Display units
    VID_AXIS,                       ///< Axis display line assignments
    VID_TACH_NUM,                   ///< Tachometer number
    VID_TACH_LINE,                  ///< Tachometer display line
    VID_TACH_SURFACE,               ///< Tachometer surface rate
    VID_EDGE_NUM,                   ///< Edge finder number
    VID_WARN_THRESH,                ///< Zero warning threshold
    VID_FEED_LINE,                  ///< Feed rate display line
    VID_FEED_AXIS,                  ///< Axis for feed rate
    VID_BEEPER,                     ///< Beeper setting
    VID_WARN_UNITS,                 ///< Zero warning threshold units
    VID_AUTO_OFF,                   ///< Auto-off in seconds
    VID_TACH_DIVISOR,               ///< Tachometer divisor
    VID_AUTO_ON,                    ///< Auto-on distance
    VID_AUTO_UNITS,                 ///< Auto-on units
    VID_EDGE_BEEP,                  ///< Edge detector beep
    VID_PROMPT,                     ///< Display prompts
    VID_MAX                         ///< MUST be last!!!
} machine_vid_t;

/// System configuration variable IDs
typedef enum system_vid
{
    VID_MACHINE_ACTIVE,       ///< Active machine
} system_vid_t;

typedef void (*key_handler_t)(keypad_t key);

typedef enum {
    MACHINE_STATE_ON,
    MACHINE_STATE_OFF,
    MACHINE_STATE_STOP,
} machine_state_t;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

// Key press handling functions
static void key_digit(keypad_t key);
static void key_function(keypad_t key);
static void key_mm_inch(keypad_t key);
static void key_abs_incr(keypad_t key);
static void key_clear(keypad_t key);
static void key_enter(keypad_t key);
static void key_zero(keypad_t key);
static void key_preset(keypad_t key);

// Map of key press to corresponding handler
static const key_handler_t key_handlers[KEYPAD_MAX] =
{
    [KEYPAD_0]          = key_digit,
    [KEYPAD_1]          = key_digit,
    [KEYPAD_2]          = key_digit,
    [KEYPAD_3]          = key_digit,
    [KEYPAD_4]          = key_digit,
    [KEYPAD_5]          = key_digit,
    [KEYPAD_6]          = key_digit,
    [KEYPAD_7]          = key_digit,
    [KEYPAD_8]          = key_digit,
    [KEYPAD_9]          = key_digit,
    [KEYPAD_FUNC]       = key_function,
    [KEYPAD_MM_INCH]    = key_mm_inch,
    [KEYPAD_ABS_INCR]   = key_abs_incr,
    [KEYPAD_CLEAR]      = key_clear,
    [KEYPAD_ENTER]      = key_enter,
    [KEYPAD_ZERO0]      = key_zero,
    [KEYPAD_PRESET0]    = key_preset,
    [KEYPAD_ZERO1]      = key_zero,
    [KEYPAD_PRESET1]    = key_preset,
    [KEYPAD_ZERO2]      = key_zero,
    [KEYPAD_PRESET2]    = key_preset,
#if CONFIG_lcd
    [KEYPAD_ZERO3]      = key_zero,
    [KEYPAD_PRESET3]    = key_preset,
    [KEYPAD_ZERO4]      = key_zero,
    [KEYPAD_PRESET4]    = key_preset,
#endif
};

// Active machine pointer
static machine_t* machine_active;

// Array of virtual machines        
static machine_t machine[MACHINE_MAX];

// Timer for zero warning beep
static timer_node_t warn_beep_timer;

// Phase count for zero warning beep
static uint8_t warn_beep_phase;

// Timer for edge beep
static timer_node_t edge_beep_timer;

// Indicates if an axis should be warned
static uint8_t warn_enable[AXIS_MAX];

// Axis type labels for each axis
static const char* axis_label[MACHINE_TYPE_MAX][MACHINE_AXIS_TYPES] = {
    [MACHINE_TYPE_MILL] = {
        [MILL_X_AXIS] = "X",
        [MILL_Y_AXIS] = "Y",
        [MILL_Z_AXIS] = "Z",
        [MILL_W_AXIS] = "W",
        [MILL_C_AXIS] = "C",
    },
    [MACHINE_TYPE_LATHE] = {
        [LATHE_X_AXIS]  = "X",
        [LATHE_Z1_AXIS] = "Z1",
        [LATHE_Z2_AXIS] = "Z2",
        [LATHE_W_AXIS]  = "W",
        [LATHE_C_AXIS]  = "C",
    },
};

///< Amount of time machine is idle
static uint32_t idle_timer;

///< Power off reading for auto power on logic
static double off_reading[SCALE_MAX];

///< Machine state
static machine_state_t state = MACHINE_STATE_ON;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static void 
start_abs_zero(uint8_t    line_num)
{
    display_indicator_set(INDICATOR_ZERO, true);
    machine_active->zero_line = line_num;
    if (machine_active->edge_num < EDGE_MAX)
        edge_start(machine_active->edge_num);
}

static void 
stop_abs_zero(void)
{
    display_indicator_set(INDICATOR_ZERO, false);
    machine_active->zero_line = ZERO_OFF;
    if (machine_active->edge_num < EDGE_MAX)
        edge_stop();
}

static void 
set_abs_zero(void)
{
    double  reading;
    uint8_t line_num;
    uint8_t axis_num;
    
    line_num = machine_active->zero_line;
    axis_num = machine_active->axis[line_num];
    
    reading = axis_get_reading(axis_num,
                               machine_active->units,
                               AXIS_ZERO_ABS);

    TRACE_INFO("abs zero %d %d %f %d\n", machine_active->machine_num,
         machine_active->axis[line_num], reading, machine_active->units);

    axis_set_absolute(axis_num,
                      reading,
                      machine_active->units);      

    stop_abs_zero();
}

static void 
set_abs_zero_pos(uint8_t     line_num,
                 double      reading)
{
    uint8_t axis_num;

    axis_num = machine_active->axis[line_num];

    axis_set_absolute(axis_num,
                      reading,
                      machine_active->units);      
}

static void
set_inc_zero(uint8_t    line_num)
{
    double  reading;
    uint8_t axis_num;

    axis_num = machine_active->axis[line_num];

    reading = axis_get_reading(axis_num,
                               machine_active->units,
                               AXIS_ZERO_ABS);

    TRACE_INFO("inc zero %d %d %f %d\n", machine_active->machine_num,
         machine_active->axis[line_num], reading, machine_active->units);

    axis_set_incremental(axis_num,
                         reading,
                         machine_active->units);       
}

static void 
set_inc_zero_pos(uint8_t    line_num,
                 double     reading)
{
    uint8_t axis_num;

    axis_num = machine_active->axis[line_num];

    axis_set_incremental(axis_num,
                         reading,
                         machine_active->units);   
}

static void 
set_inc_zero_offset(uint8_t     line_num,
                    double      reading)
{
    uint8_t axis_num;

    axis_num = machine_active->axis[line_num];

    reading += axis_get_incremental(axis_num,
                                    machine_active->units);
    
    axis_set_incremental(axis_num,
                         reading,
                         machine_active->units);      
}

static void 
key_digit(keypad_t      key)
{
    // Send digits to the function object
    function_key(key);
}

static void 
key_function(keypad_t   key)
{
    function_select();
}

static void 
key_mm_inch(keypad_t    key)
{
    axis_units_t units;

    if (machine_active->units == AXIS_UNITS_INCH)
        units = AXIS_UNITS_MM;
    else
        units = AXIS_UNITS_INCH;

    machine_set_units(units);

    TRACE_INFO("mm_inch %d %d\n", machine_active->machine_num, machine_active->units);
}

static void
key_abs_incr(keypad_t   key)
{
    // Toggle the machine zero
    if (machine_active->zero == AXIS_ZERO_ABS)
        machine_set_zero(AXIS_ZERO_INC);
    else
        machine_set_zero(AXIS_ZERO_ABS);
        
    TRACE_INFO("abs_incr %d %d\n", machine_active->machine_num, machine_active->zero);
}

static void 
key_enter(keypad_t      key)
{
    if (machine_active->zero_line != ZERO_OFF)
    {
        set_abs_zero();
    }
    else
    {
        // Send enter to the function object
        function_key(key);
    }
}

static void 
key_clear(keypad_t      key)
{
    if (machine_active->zero_line != ZERO_OFF)
    {
        stop_abs_zero();
    }
    else
    {
        // Send clear to the function object
        function_key(key);
    }
}

static void 
key_zero(keypad_t   key)
{
    uint8_t line_num;

    if (!keypad_display_line(key, &line_num))
        return; 

    if (machine_active->axis[line_num] == AXIS_OFF)
        return;

    if (machine_active->zero == AXIS_ZERO_ABS)
    {
        if (line_num == machine_active->zero_line)
        {
            set_abs_zero();
        }
        else
        {
            start_abs_zero(line_num);
        }           
    }
    else
    {
        set_inc_zero(line_num);
    }   
}

static void 
key_preset(keypad_t     key)
{
    uint8_t line_num;
    double  reading;

    if (!keypad_display_line(key, &line_num))
        return; 

    if (machine_active->axis[line_num] == AXIS_OFF)
        return;

    display_menu_puts(MENU_TOP, "enter");
    display_menu_puts(MENU_MIDDLE, "preset");

    reading = 0.0;
    if (keypad_get_float(MENU_BOTTOM, &reading, NULL))
    {
        TRACE_INFO("preset %d = %f\n", line_num, reading);
        if (machine_active->zero_line == ZERO_OFF)
        {
            if (machine_active->zero == AXIS_ZERO_ABS)
                set_inc_zero_pos(line_num, reading);
            else
                set_inc_zero_offset(line_num, reading);

            machine_active->zero = AXIS_ZERO_INC;
            display_indicator_set(INDICATOR_INCR, true);
        }
        else
        {
            set_abs_zero_pos(line_num, reading);
            stop_abs_zero();
        }
    }   
}

static void 
display_tach(void)
{
    static uint32_t last_time;
    static uint32_t zero_count = 0;
    uint32_t        time;
    double          reading;

    // Do not update the display faster than the update rate
    time = timer_get() * TIMER_PERIOD;
    if ((time - last_time) < (1000000 / TACH_UPDATE_HZ))
        return;
    last_time = time;
    
    // Read the tachometer
    reading = tach_get_reading(machine_active->tach_num) / machine_active->tach_divisor;

    // Count the number of consecutive zero readings and display
    // zero if they cross the threshold
    if (reading == 0.0)
    {
        if (zero_count++ > TACH_ZERO_COUNT)
        {
            display_reading_set(machine_active->tach_line, 0.0, 0, false);
        }
        return;
    }

    zero_count = 0;

    // Calculate the surface rate   
    if (machine_active->tach_surface)
    {
        // Get the diameter of the cut
        reading *= function_get_cut_dia(machine_active->units);

        // Multiply by PI and covert inches/mm to feet/meter to
        // arrive at SFM/SMM       
        if (machine_active->units == AXIS_UNITS_INCH)
            reading *= PI / 12.0;
        else
            reading *= PI / 1000.0;
    }           

    // Display the result on the configured line
    display_reading_set(machine_active->tach_line, reading, 0, false);
}

static void 
display_feed(void)
{
    static double   last_reading;
    static uint32_t last_time;
    double          reading;
    uint32_t        time;
    double          feed_rate;
    
    time = timer_get() * TIMER_PERIOD;
    if ((time - last_time) < (1000000 / FEED_UPDATE_HZ))
        return;
    
    reading = axis_get_reading(machine_active->feed_axis,
                               machine_active->units,
                               machine_active->zero);
    
    // Calculate the feed rate in units per minute
    feed_rate = fabs((reading - last_reading) / (double) (time - last_time) * (1000000.0 * 60.0));
    
    // Display the result on the configured line            
    display_reading_set(machine_active->feed_line, feed_rate, 2, false);

    TRACE_DEBUG("feed %f %d %f %d %f\n", last_reading, last_time, reading, time, feed_rate);
    
    // Save the measurements
    last_reading = reading;
    last_time = time;
}

static void
warn_beep_timeout(void* user)
{
    warn_beep_phase--;
    if (warn_beep_phase == 0)
        timer_stop(&warn_beep_timer);

    if (warn_beep_phase % 2 == 0)
        machine_beeper_off();
    else
        machine_beeper_on();
}

static void
edge_beep_timeout(void* user)
{
    machine_beeper_off();
}

static void 
update_readings(void)
{
    uint8_t line_num;
    uint8_t axis_num;
    uint8_t scale_num;
    uint8_t precision;
    uint8_t machine_type;
    double  reading;
    double  jitter;
    double  zero_warn;
    bool    mark;

    machine_type = machine_active->type;

    for (line_num = 0; line_num < DISPLAY_LINE_MAX; line_num++)
    {
        axis_num = machine_active->axis[line_num];

        if (line_num == machine_active->tach_line)
        {
            display_tach();
            display_label_set(line_num, "T%d", machine_active->tach_num + 1);
        }
        else if (line_num == machine_active->feed_line)
        {
            display_feed();
            display_label_set(line_num, "F%s", axis_label[machine_type][machine_active->feed_axis]);
        }
        else
        {
            if (axis_num == AXIS_OFF)
            {
                display_reading_off(line_num);
                display_label_set(line_num, " ");
            }
            else
            {
                display_reading_on(line_num);

                if (machine_active->display == DISPLAY_READING)
                {
                    reading = axis_get_reading(axis_num,
                                               machine_active->units,
                                               machine_active->zero);
                    
                    precision = axis_get_precision(axis_num,
                                                   machine_active->units);
                    
                    // If the axis has a tool offset or is scaled, then
                    // set the mark for that line.
                    if (axis_get_tool_offset(axis_num, machine_active->units) != 0.0 ||
                        axis_get_scale_factor(axis_num) != 1.0)
                        mark = true;
                    else
                        mark = false;

                    display_reading_set(line_num, reading, precision, mark);
                    display_label_set(line_num, axis_label[machine_type][axis_num]);

                    // Convert the reading units
                    zero_warn = fabs(reading);
                    zero_warn = axis_convert_units(zero_warn, machine_active->units, machine_active->warn_units);

                    // If this axis can be warned and the
                    // reading is below the threshold, then warn
                    if (warn_enable[axis_num] &&
                        zero_warn < machine_active->warn_thresh)
                    {
                        warn_enable[axis_num] = false;
                        display_reading_blink(line_num);
                        if (machine_active->beeper != BEEPER_NONE &&
                            warn_beep_phase == 0 &&
                            !timer_running(&edge_beep_timer))
                        {
                            warn_beep_phase = WARN_BEEP_COUNT;
                            timer_start(&warn_beep_timer, WARN_BEEP_PERIOD, TIMER_RECURRING,
                                        warn_beep_timeout, NULL);
                            machine_beeper_on();
                        }
                    }
                    // If this axis should not be zero warned and the reading is more
                    // than 10% above the threshold, then re-enable the warning
                    else if (!warn_enable[axis_num] &&
                             zero_warn > (machine_active->warn_thresh * 1.1))
                    {
                        warn_enable[axis_num] = true;
                    }
                }
                else if (machine_active->display == DISPLAY_COUNTS)
                {
                    scale_num = axis_get_scale(axis_num);
                    display_reading_set(line_num, scale_get_count(scale_num), 0, false);
                    display_label_set(line_num, "%sC", axis_label[machine_type][axis_num]);
                }
                else if (machine_active->display == DISPLAY_JITTER)
                {
                    jitter = 0.0;
                    scale_num = axis_get_scale(axis_num);
                    scale_get_stats(scale_num, &reading, &jitter);
                    display_reading_set(line_num, jitter, 2, false);
                    display_label_set(line_num, "%sJ", axis_label[machine_type][axis_num]);
                }
            }
        }
    }
}

static void 
machine_display_axis(uint8_t obj_inst,
                     uint8_t var_inst)
{
    if (machine_active->axis[var_inst] == AXIS_OFF)
        display_menu_puts(MENU_BOTTOM, "off");
    else
        display_menu_printf(MENU_BOTTOM, "axis%d",
                       machine_active->axis[var_inst] + 1);
}

static void 
machine_select_axis(uint8_t obj_inst,
                    uint8_t var_inst)
{
    if (machine_active->axis[var_inst] == AXIS_OFF)
    {
        machine_active->axis[var_inst] = 0;
    }
    else
    {
        machine_active->axis[var_inst]++;
        if (machine_active->axis[var_inst] == AXIS_MAX)
            machine_active->axis[var_inst] = AXIS_OFF;
    }
}

static void 
machine_display_type(uint8_t obj_inst,
                     uint8_t var_inst)
{
    static char *type_name[MACHINE_TYPE_MAX] = {
        [MACHINE_TYPE_MILL]     = "mill",
        [MACHINE_TYPE_LATHE]    = "lathe",
    };

    display_menu_puts(MENU_BOTTOM, type_name[machine_active->type]);        
}

static void 
machine_select_type(uint8_t obj_inst,
                    uint8_t var_inst)
{
    machine_active->type++;
    if (machine_active->type == MACHINE_TYPE_MAX)
        machine_active->type = 0;
}

static void 
machine_display_edge(uint8_t obj_inst,
                     uint8_t var_inst)
{
    if (machine_active->edge_num == EDGE_NONE)
        display_menu_printf(MENU_BOTTOM, "none");
    else if (machine_active->edge_num == EDGE_TACH)
        display_menu_printf(MENU_BOTTOM, "tach");
    else
        display_menu_printf(MENU_BOTTOM, "edge%d", machine_active->edge_num + 1);
}

static void 
machine_select_edge(uint8_t obj_inst,
                    uint8_t var_inst)
{
    if (machine_active->edge_num == EDGE_NONE)
    {
        machine_active->edge_num = EDGE_TACH;
    }
    else if (machine_active->edge_num == EDGE_TACH)
    {
        machine_active->edge_num = 0;
    }
    else
    {
        machine_active->edge_num++;
        if (machine_active->edge_num == EDGE_MAX)
            machine_active->edge_num = EDGE_NONE;
    }
}

static void 
machine_display_tach(uint8_t obj_inst,
                     uint8_t var_inst)
{
    display_menu_printf(MENU_BOTTOM, "tach%d", machine_active->tach_num + 1);
}

static void 
machine_select_tach(uint8_t obj_inst,
                    uint8_t var_inst)
{
    machine_active->tach_num++;
    if (machine_active->tach_num == TACH_MAX)
        machine_active->tach_num = 0;
}

static void 
machine_display_tach_divsor(uint8_t obj_inst,
                            uint8_t var_inst)
{
    display_menu_printf(MENU_BOTTOM, "%d", machine_active->tach_divisor);
}

static void 
machine_select_tach_divsor(uint8_t obj_inst,
                           uint8_t var_inst)
{
    int32_t tach_divisor;
    
    tach_divisor = machine_active->tach_divisor;
    if (keypad_get_number(MENU_BOTTOM, 1, TACH_DIVISOR_MAX, &tach_divisor))
        machine_active->tach_divisor = tach_divisor;
}

static void 
machine_display_display(uint8_t obj_inst,
                        uint8_t var_inst)
{
    char*       string;

    if (machine_active->display == DISPLAY_READING)
        string = "readng";
    else if (machine_active->display == DISPLAY_COUNTS)
        string = "counts";
    else if (machine_active->display == DISPLAY_JITTER)
        string = "jitter";
    else
        string = "unknwn";
        
    display_menu_puts(MENU_BOTTOM, string);      
}

static void 
machine_select_display(uint8_t obj_inst,
                       uint8_t var_inst)
{
    machine_active->display++;
    if (machine_active->display == DISPLAY_MAX)
        machine_active->display = 0;
}

static void 
machine_display_warn_thresh(uint8_t obj_inst,
                            uint8_t var_inst)
{
    double      warn_thresh;

    warn_thresh = axis_convert_units(machine_active->warn_thresh, machine_active->warn_units, machine_active->units);
    display_menu_printf(MENU_BOTTOM, "%*f", MENU_WIDTH, warn_thresh);
}

static void 
machine_select_warn_thresh(uint8_t obj_inst,
                           uint8_t var_inst)
{
    double warn_thresh;

    warn_thresh = axis_convert_units(machine_active->warn_thresh, machine_active->warn_units, machine_active->units);
    if (keypad_get_float(MENU_BOTTOM, &warn_thresh, NULL))
    {
        machine_active->warn_thresh = fabs(warn_thresh);
        machine_active->warn_units = machine_active->units;
    }
}

static void
machine_display_beeper(uint8_t obj_inst,
                       uint8_t var_inst)
{
#if PLATFORM_dpu550
    if (machine_active->beeper == BEEPER_NONE)
        display_menu_printf(MENU_BOTTOM, "none");
    else
        display_menu_printf(MENU_BOTTOM, "tach%d", machine_active->beeper + 1);
#else
    if (machine_active->beeper == BEEPER_NONE)
        display_menu_printf(MENU_BOTTOM, "no");
    else
        display_menu_printf(MENU_BOTTOM, "yes");
#endif
}

static void
machine_select_beeper(uint8_t obj_inst,
                      uint8_t var_inst)
{
    if (machine_active->beeper == BEEPER_NONE)
    {
        machine_active->beeper = 0;
    }
    else
    {
#if PLATFORM_dpu550
        machine_active->beeper++;
        if (machine_active->beeper == TACH_MAX)
            machine_active->beeper = BEEPER_NONE;
#else
        machine_active->beeper = BEEPER_NONE;
#endif
    }
}

static void
machine_display_edge_beep(uint8_t obj_inst,
                          uint8_t var_inst)
{
    display_menu_printf(MENU_BOTTOM, machine_active->edge_beep ? "yes" : "no");
}

static void
machine_select_edge_beep(uint8_t obj_inst,
                         uint8_t var_inst)
{
    machine_active->edge_beep = (machine_active->edge_beep ? false : true);
}

static void
machine_display_prompt(uint8_t obj_inst,
                       uint8_t var_inst)
{
    display_menu_printf(MENU_BOTTOM, machine_active->prompt ? "yes" : "no");
}

static void
machine_select_prompt(uint8_t obj_inst,
                      uint8_t var_inst)
{
    machine_active->prompt = (machine_active->prompt ? false : true);
}

static void
machine_display_auto_off(uint8_t obj_inst,
                         uint8_t var_inst)
{
    display_menu_printf(MENU_BOTTOM, "%*" PRIu32, MENU_WIDTH, machine_active->auto_off / 60);
}

static void
machine_select_auto_off(uint8_t obj_inst,
                        uint8_t var_inst)
{
    int32_t auto_off;

    auto_off = machine_active->auto_off / 60;
    if (keypad_get_number(MENU_BOTTOM, 0, 999, &auto_off))
        machine_active->auto_off = auto_off * 60;
}

static void
machine_display_auto_on(uint8_t obj_inst,
                        uint8_t var_inst)
{
    double auto_on;

    auto_on = axis_convert_units(machine_active->auto_on, machine_active->auto_units, machine_active->units);
    display_menu_printf(MENU_BOTTOM, "%*f", MENU_WIDTH, auto_on);
}

static void
machine_select_auto_on(uint8_t obj_inst,
                       uint8_t var_inst)
{
    double auto_on;

    auto_on = axis_convert_units(machine_active->auto_on, machine_active->auto_units, machine_active->units);
    if (keypad_get_float(MENU_BOTTOM, &auto_on, NULL))
    {
        machine_active->auto_on = fabs(auto_on);
        machine_active->auto_units = machine_active->units;
    }
}

#ifdef PLATFORM_dpu550
static void
machine_tach_mux(void)
{
    bool tach1_used = false;
    uint8_t machine_num;
    
    // On the DPU-550, the scale clock is output via tach 1 (marked
    // TACH 2 on the PCB) so we must multiplex between the two
    // functions depending on whether tach 1 is used or not.
    for (machine_num = 0; machine_num < MACHINE_MAX; machine_num++)
    {
        if (machine[machine_num].tach_num == 1)
        {
            tach1_used = true;
            break;
        }
    }

    if (tach1_used)
    {
        GPIO_PULLUP(TACH_PORT, TACH1_INPUT);
        GPIO_PERIPH_B(TACH_PORT, TACH1_INPUT);
    }
    else
    {
        GPIO_NOPULLUP(PWM_OUT_PORT, PWM_OUT_ALL);
        GPIO_PERIPH_A(PWM_OUT_PORT, PWM_OUT_ALL);
    }
}
#endif

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
machine_dec2frac(double decimal,
                 int32_t maxdenom,
                 int32_t* integral,
                 int32_t* numerator,
                 int32_t* denominator)
{
    *integral = decimal;
    *numerator = round((decimal - *integral) * maxdenom);
    *denominator = maxdenom;
    if (*numerator / *denominator)
    {
        *integral += *numerator / *denominator;
        *numerator = 0;
    }
    else
    {
        while (*numerator && *numerator % 2 == 0)
        {
            *numerator /= 2;
            *denominator /= 2;
        }
        if (*integral < 0)
            *numerator = -*numerator;
    }
}

uint8_t 
machine_axis_by_line(uint8_t line_num)
{
    ASSERT(line_num < DISPLAY_LINE_MAX);
    
    return machine_active->axis[line_num]; 
}

uint8_t
machine_axis_by_label(const char* label)
{
    uint8_t axis_num;
    machine_type_t machine_type;
    
    machine_type = machine_active->type;
    if (isalpha(label[0])) {
        for (axis_num = 0; axis_num < AXIS_MAX; axis_num++) {
            if (strcasecmp(label, axis_label[machine_type][axis_num]) == 0)
                return axis_num;
        }
    } else {
        return atoi(label) - 1;
    }
    
    return AXIS_MAX;
}

const char*
machine_axis_get_label(uint8_t axis_num)
{
    machine_type_t machine_type;
    
    ASSERT(axis_num < AXIS_MAX);

    machine_type = machine_active->type;
    
    return axis_label[machine_type][axis_num];
}

machine_type_t 
machine_get_type(void)
{
    return machine_active->type;       
}

axis_zero_t 
machine_get_mode(void)
{
    return machine_active->zero;       
}

axis_units_t 
machine_get_units(void)
{
    return machine_active->units;      
}

void
machine_set_units(axis_units_t  units)
{
    machine_active->units = units;

    if (machine_active->units == AXIS_UNITS_INCH)
        display_indicator_set(INDICATOR_MM, false);
    else
        display_indicator_set(INDICATOR_MM, true);

    if (config_obj_write(CONFIG_OID_MACHINE, machine_active->machine_num))
    {
        config_var_write(VID_UNITS, 0,
                     sizeof(machine_active->units),
                     &machine_active->units);
        config_obj_close();
    }
}

double
machine_to_units(double value, axis_units_t units)
{
    if (units == machine_active->units)
        return value;
        
    if (units == AXIS_UNITS_MM)
        value *= INCH_TO_MM;
    else
        value *= MM_TO_INCH;
        
    return value;
}

double
machine_from_units(double value, axis_units_t units)
{
    if (units == machine_active->units)
        return value;
        
    if (units == AXIS_UNITS_MM)
        value *= MM_TO_INCH;
    else
        value *= INCH_TO_MM;
        
    return value;
}

uint8_t
machine_get_active(void)
{
    return machine_active->machine_num;
}

void 
machine_set_active(uint8_t machine_num)
{
    ASSERT(machine_num < MACHINE_MAX);
    
    machine_active = &machine[machine_num];

    function_set_active();

    if (config_obj_write(CONFIG_OID_SYSTEM, 0))
    {
        config_var_write(VID_MACHINE_ACTIVE, 0, sizeof(machine_num), &machine_num);
        config_obj_close();
    }
}

void 
machine_set_tach(uint8_t    line_num,
                 bool       surface)
{
    if (line_num >= DISPLAY_LINE_MAX)
        line_num = DISPLAY_OFF;
    machine_active->tach_line = line_num;
    machine_active->tach_surface = surface;    

    if (config_obj_write(CONFIG_OID_MACHINE, machine_active->machine_num))
    {
        config_var_write(VID_TACH_LINE, 0, sizeof(machine_active->tach_line), &machine_active->tach_line);
        config_var_write(VID_TACH_SURFACE, 0, sizeof(machine_active->tach_surface), &machine_active->tach_surface);
        config_var_write(VID_TACH_DIVISOR, 0, sizeof(machine_active->tach_divisor), &machine_active->tach_divisor);
        config_obj_close();
    }
}

void 
machine_get_tach(uint8_t*   line_num,
                 bool*      surface)
{
    *line_num = machine_active->tach_line;
    *surface = machine_active->tach_surface;   
}

void 
machine_set_feed(uint8_t    line_num,
                 uint8_t    axis_num)
{
    if (line_num >= DISPLAY_LINE_MAX)
        line_num = DISPLAY_OFF;
    machine_active->feed_line = line_num;
    machine_active->feed_axis = axis_num;    

    if (config_obj_write(CONFIG_OID_MACHINE, machine_active->machine_num))
    {
        config_var_write(VID_FEED_LINE, 0, sizeof(machine_active->feed_line), &machine_active->feed_line);
        config_var_write(VID_FEED_AXIS, 0, sizeof(machine_active->feed_axis), &machine_active->feed_axis);
        config_obj_close();
    }
}

void 
machine_get_feed(uint8_t*   line_num,
                 uint8_t*   axis_num)
{
    *line_num = machine_active->feed_line;
    *axis_num = machine_active->feed_axis;   
}

void
machine_set_zero(axis_zero_t    zero)
{
    uint8_t     axis_num;

    machine_active->zero = zero;

    for (axis_num = 0; axis_num < AXIS_MAX; axis_num++)
        warn_enable[axis_num] = false;

    if (machine_active->zero == AXIS_ZERO_INC)
        display_indicator_set(INDICATOR_INCR, true);
    else
        display_indicator_set(INDICATOR_INCR, false);
}

axis_zero_t
machine_get_zero(void)
{
    return machine_active->zero;
}

void
machine_display_toggle(void)
{
    if (machine_active->display == DISPLAY_READING)
        machine_active->display = DISPLAY_COUNTS;
    else if (machine_active->display == DISPLAY_COUNTS)
        machine_active->display = DISPLAY_JITTER;
    else if (machine_active->display == DISPLAY_JITTER)
        machine_active->display = DISPLAY_READING;
}

#if PLATFORM_dpu550
void
machine_beeper_on(void)
{
    pin_t       tach;

    if (machine_active->beeper != BEEPER_NONE)
    {
        tach = tach_pin(machine_active->beeper);
        GPIO_LOW(TACH_PORT, tach);
        GPIO_OUTPUT(TACH_PORT, tach);
    }
}

void
machine_beeper_off(void)
{
    pin_t       tach;

    if (machine_active->beeper != BEEPER_NONE)
    {
        tach = tach_pin(machine_active->beeper);
        GPIO_INPUT(TACH_PORT, tach);
    }
}
#else
void
machine_beeper_on(void)
{
    if (machine_active->beeper != BEEPER_NONE)
    {
#if PLATFORM_dro550
        GPIO_PERIPH_A(BEEPER_PORT, BEEPER_DRIVE);
#elif PLATFORM_dro375
        GPIO_PERIPH_B(BEEPER_PORT, BEEPER_DRIVE);
#endif
    }
}

void
machine_beeper_off(void)
{
    if (machine_active->beeper != BEEPER_NONE)
    {
        GPIO_OUTPUT(BEEPER_PORT, BEEPER_DRIVE);
    }
}
#endif

void
machine_set_prompt(bool enable)
{
    machine_active->prompt = enable;
    
    if (config_obj_write(CONFIG_OID_MACHINE, machine_active->machine_num))
    {
        config_var_write(VID_PROMPT, 0, sizeof(machine_active->prompt), &machine_active->prompt);
        config_obj_close();
    }
}

void
machine_all_prompt(bool enable)
{
    uint8_t machine_num;

    for (machine_num = 0; machine_num < MACHINE_MAX; machine_num++)
        machine[machine_num].prompt = enable;
}

bool
machine_get_prompt(void)
{
    return machine_active->prompt;
}

void 
machine_process(void)
{
    keypad_t    key;
    uint32_t    axis_num;

    if (state == MACHINE_STATE_ON)
    {
        // Handle an absolute zero set with the edge finder
        if (machine_active->zero_line != ZERO_OFF)
        {
            if ((machine_active->edge_num < EDGE_MAX && 
                     edge_trigger()) ||
                (machine_active->edge_num == EDGE_TACH &&
                    GPIO_IS_LOW(TACH_PORT, tach_pin(machine_active->tach_num))))
            {
                set_abs_zero();
                
                // Edge finder beep
                if (machine_active->edge_beep &&
                    machine_active->beeper != BEEPER_NONE)
                {
                    machine_beeper_on();
                    timer_start(&edge_beep_timer, EDGE_BEEP_TIME, TIMER_ONE_SHOT,
                                edge_beep_timeout, NULL);
                }
            }
        }

        // Process a key press
        if (!keypad_empty())
        {
            key = keypad_get_press();
            
            if (key_handlers[key])
                key_handlers[key](key);

            // Disable all zero warnings after a key press
            for (axis_num = 0; axis_num < AXIS_MAX; axis_num++)
                warn_enable[axis_num] = false;
        }
    }
    
    machine_idle(true);
}

void
machine_off(void)
{
    uint8_t axis_num;
    
    if (state != MACHINE_STATE_OFF)
    {
         // Turn the function and display off
        state = MACHINE_STATE_OFF;
        function_off();
        display_off();
        
        // Capture the axis readings if auto power on configured
        if (machine_active->auto_on > 0.0)
        {
            for (axis_num = 0; axis_num < SCALE_MAX; axis_num++)
            {
                off_reading[axis_num] = axis_get_reading(axis_num,
                                                         machine_active->auto_units,
                                                         AXIS_ZERO_ABS);
            }
        }
    }
}

void
machine_on(void)
{
    if (state != MACHINE_STATE_ON)
    {
        // Turn the display and function back on
        state = MACHINE_STATE_ON;
        display_on();
        function_on();
        
        // Flush the keypad
        keypad_flush();
        
        // Machine is busy
        machine_busy();
    }
}

void
machine_stop(void)
{
    state = MACHINE_STATE_STOP;
}

void
machine_idle(bool update)
{
    int32_t axis_num;
    double reading;
    
    if (state == MACHINE_STATE_ON)
    {
        // Process axis readings
        if (update)
            update_readings();

        // If the auto off is configured, then turn the machine off
        // if the idle time exceeds the power off time
        if (machine_active->auto_off > 0)
        {
            if (((timer_get() - idle_timer) / SEC_TICKS) >= machine_active->auto_off)
            {
                TRACE_INFO("Machine idle - auto power off");
                machine_off();
            }
        }

        // Process the Lua shell
        luash_process();
    }

    while (state == MACHINE_STATE_OFF)
    {
        // Check if a scale moved more than the auto power on threshold
        if (machine_active->auto_on > 0.0)
        {
            for (axis_num = 0; axis_num < AXIS_MAX; axis_num++)
            {
                reading = axis_get_reading(axis_num,
                                           machine_active->auto_units,
                                           AXIS_ZERO_ABS);

                if (fabs(reading - off_reading[axis_num]) >= machine_active->auto_on)
                {
                    TRACE_INFO("Axis %d movement - auto power on\n", axis_num);
                    machine_on();
                    break;
                }
            }
        }
        
        // Wake from a key press
        if (!keypad_empty())
        {
            TRACE_INFO("Key press - power on\n");
            machine_on();
        }

        // Process the Lua shell
        luash_process();
    }

    while (state == MACHINE_STATE_STOP)
    {
        // Process the Lua shell
        luash_process();
    }    
}

void
machine_busy()
{
    // Reset the idle timer
    idle_timer = timer_get();
}

void
machine_save(void)
{
    machine_t*  machine_ptr;
    uint8_t     machine_num;
    uint8_t     line_num;
    
    if (config_obj_write(CONFIG_OID_SYSTEM, 0))
    {
        machine_num = machine_active->machine_num;
        config_var_write(VID_MACHINE_ACTIVE, 0, sizeof(machine_num), &machine_num);
        config_obj_close();
    }

    for (machine_num = 0; machine_num < MACHINE_MAX; machine_num++)
    {
        machine_ptr = &machine[machine_num];
        
        if (config_obj_write(CONFIG_OID_MACHINE, machine_num))
        {
            config_var_write(VID_TYPE, 0, sizeof(machine_ptr->type), &machine_ptr->type);
            config_var_write(VID_UNITS, 0, sizeof(machine_ptr->units), &machine_ptr->units);
            config_var_write(VID_TACH_NUM, 0, sizeof(machine_ptr->tach_num), &machine_ptr->tach_num);
            config_var_write(VID_TACH_LINE, 0, sizeof(machine_ptr->tach_line), &machine_ptr->tach_line);
            config_var_write(VID_TACH_SURFACE, 0, sizeof(machine_ptr->tach_surface), &machine_ptr->tach_surface);
            config_var_write(VID_TACH_DIVISOR, 0, sizeof(machine_ptr->tach_divisor), &machine_ptr->tach_divisor);
            config_var_write(VID_EDGE_NUM, 0, sizeof(machine_ptr->edge_num), &machine_ptr->edge_num);
            config_var_write(VID_WARN_THRESH, 0, sizeof(machine_ptr->warn_thresh), &machine_ptr->warn_thresh);
            config_var_write(VID_WARN_UNITS, 0, sizeof(machine_ptr->warn_units), &machine_ptr->warn_units);
            config_var_write(VID_BEEPER, 0, sizeof(machine_ptr->beeper), &machine_ptr->beeper);
            config_var_write(VID_AUTO_OFF, 0, sizeof(machine_ptr->auto_off), &machine_ptr->auto_off);
            config_var_write(VID_AUTO_ON, 0, sizeof(machine_ptr->auto_on), &machine_ptr->auto_on);
            config_var_write(VID_AUTO_UNITS, 0, sizeof(machine_ptr->auto_units), &machine_ptr->auto_units);
            config_var_write(VID_EDGE_BEEP, 0, sizeof(machine_ptr->edge_beep), &machine_ptr->edge_beep);
            config_var_write(VID_PROMPT, 0, sizeof(machine_ptr->prompt), &machine_ptr->prompt);
    
            for (line_num = 0; line_num < DISPLAY_LINE_MAX; line_num++)
            {
                config_var_write(VID_AXIS, line_num,
                    sizeof(machine_ptr->axis[line_num]),
                    &machine_ptr->axis[line_num]);
            }
        
            config_obj_close();
        }
    }
}

void 
machine_setup(void)
{
    static const var_list_t var_list[] = {
        { "type", 0, machine_display_type, machine_select_type },
        { "tach", 0, machine_display_tach, machine_select_tach },
        { "tacdiv", 0, machine_display_tach_divsor, machine_select_tach_divsor },
        { "edge", 0, machine_display_edge, machine_select_edge },
        { "disply", 0, machine_display_display, machine_select_display },
        { "warn", 0, machine_display_warn_thresh, machine_select_warn_thresh },
        { "beeper", 0, machine_display_beeper, machine_select_beeper },
        { "autoff", 0, machine_display_auto_off, machine_select_auto_off },
        { "autoon", 0, machine_display_auto_on, machine_select_auto_on },
        { "edgbep", 0, machine_display_edge_beep, machine_select_edge_beep },
        { "prmpt", 0, machine_display_prompt, machine_select_prompt },
        { "line%d", DISPLAY_LINE_MAX, machine_display_axis, machine_select_axis },
    };

    if (config_setup("mach", 0, ARRAY_SIZE(var_list), var_list))
    {
        machine_save();

        // Set the active machine in case there were changes
        function_set_active();

#if PLATFORM_dpu550
        machine_tach_mux();
#endif
    }
}

void
machine_show(FILE* out)
{
    uint8_t     n;
    static const char *type_names[] = {
        [MACHINE_TYPE_MILL]     = "mill",
        [MACHINE_TYPE_LATHE]    = "lathe",
    };
    static const char *unit_names[] = {
        [AXIS_UNITS_INCH]   = "inch",
        [AXIS_UNITS_MM]     = "mm",
    };
    static const char *zero_names[] = {
        [AXIS_ZERO_ABS] = "absolute",
        [AXIS_ZERO_INC] = "incremental",
    };
    static const char *display_names[] = {
        [DISPLAY_READING]   = "reading",
        [DISPLAY_COUNTS]    = "counts",
        [DISPLAY_JITTER]    = "jitter",
    };
    static const char *state_names[] = {
        [MACHINE_STATE_ON]      = "on",
        [MACHINE_STATE_OFF]     = "off",
        [MACHINE_STATE_STOP]    = "stop",
    };
#define FIELD_WIDTH     12
    char field[FIELD_WIDTH];

    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Machine", machine_active->machine_num);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Type", type_names[machine_active->type]);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Units", unit_names[machine_active->units]);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Zero", zero_names[machine_active->zero]);
    for (n = 0; n < DISPLAY_LINE_MAX; n++)
    {
        snprintf(field, sizeof(field), "Line %d", n);
        fprintf(out, "%-*s : Axis %d\n", FIELD_WIDTH, field, machine_active->axis[n]);
    }
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Display", display_names[machine_active->display]);
    fprintf(out, "%-*s : ", FIELD_WIDTH, "Edge");
    if (machine_active->edge_num == EDGE_NONE)
        fprintf(out, "none\n");
    else if (machine_active->edge_num == EDGE_NONE)
        fprintf(out, "tach\n");
    else
        fprintf(out, "%d\n", machine_active->edge_num);
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Tach", machine_active->tach_num);
    fprintf(out, "%-*s : ", FIELD_WIDTH, "Tach Line");
    if (machine_active->tach_line == DISPLAY_OFF)
        fprintf(out, "off\n");
    else
        fprintf(out, "%d\n", machine_active->tach_line);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Tach Surface", machine_active->tach_surface ? "yes" : "no");
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Tach Divisor", machine_active->tach_divisor);
    fprintf(out, "%-*s : %f %s\n", FIELD_WIDTH, "Warn Thresh", machine_active->warn_thresh,
            machine_active->warn_units == AXIS_UNITS_MM ? "mm" : "inch");
    fprintf(out, "%-*s : ", FIELD_WIDTH, "Feed Line");
    if (machine_active->feed_line == DISPLAY_OFF)
        fprintf(out, "off\n");
    else
        fprintf(out, "%d\n", machine_active->feed_line);
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Feed Axis", machine_active->feed_axis);
    fprintf(out, "%-*s : ", FIELD_WIDTH, "Beeper");
#if PLATFORM_dpu550
    if (machine_active->beeper == BEEPER_NONE)
        fprintf(out, "none\n");
    else
        fprintf(out, "%d\n", machine_active->beeper);
#else
    if (machine_active->beeper == BEEPER_NONE)
        fprintf(out, "no\n");
    else
        fprintf(out, "yes\n");
#endif
    fprintf(out, "%-*s : %"PRIu32"\n", FIELD_WIDTH, "Auto Off", machine_active->auto_off);
    fprintf(out, "%-*s : %f %s\n", FIELD_WIDTH, "Auto On", machine_active->auto_on,
            machine_active->auto_units == AXIS_UNITS_MM ? "mm" : "inch");
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Edge Beep", machine_active->edge_beep ? "yes" : "no");
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Prompt", machine_active->prompt ? "yes" : "no");
    fprintf(out, "%-*s : %"PRIu32"\n", FIELD_WIDTH, "Idle Timer", (timer_get() - idle_timer) / SEC_TICKS);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "State", state_names[state]);
}

void 
machine_init(void)
{
    uint8_t     machine_num = 0;
    machine_t*  machine_ptr;
    uint8_t     line_num;

    if (config_obj_read(CONFIG_OID_SYSTEM, 0))
    {
        config_var_read(VID_MACHINE_ACTIVE, 0, sizeof(machine_num), &machine_num);
        if (machine_num >= MACHINE_MAX)
            machine_num = 0;
        config_obj_close();
    }

    // Set the active machine
    machine_active = &machine[machine_num];

    for (machine_num = 0; machine_num < MACHINE_MAX; machine_num++)
    {
        // Non-zero defaults
        machine_ptr = &machine[machine_num];
        machine_ptr->machine_num = machine_num;
        machine_ptr->zero_line = ZERO_OFF;
        machine_ptr->type = MACHINE_TYPE_MILL;
        machine_ptr->tach_line = DISPLAY_LINE_MAX;
        machine_ptr->tach_divisor = 1;
        machine_ptr->feed_line = DISPLAY_LINE_MAX;
#if PLATFORM_dpu550
        machine_ptr->beeper = BEEPER_NONE;
#else
        machine_ptr->beeper = 0;
#endif
        machine_ptr->auto_on = 0.01;
        machine_ptr->auto_units = AXIS_UNITS_INCH;
        machine_ptr->prompt = true;
        
        for (line_num = 0; line_num < DISPLAY_LINE_MAX; line_num++)
        {
            machine_ptr->axis[line_num] = line_num;                 
        }
        
        if (config_obj_read(CONFIG_OID_MACHINE, machine_num))
        {
            config_var_read(VID_TYPE, 0, sizeof(machine_ptr->type), &machine_ptr->type);
            if (machine_ptr->type >= MACHINE_TYPE_MAX)
                machine_ptr->type = MACHINE_TYPE_MILL;

            config_var_read(VID_UNITS, 0, sizeof(machine_ptr->units), &machine_ptr->units);
            if (machine_ptr->units >= AXIS_UNITS_MAX)
                machine_ptr->units = AXIS_UNITS_INCH;
            if (machine_ptr->units == AXIS_UNITS_MM)
                display_indicator_set(INDICATOR_MM, true);

            config_var_read(VID_TACH_NUM, 0, sizeof(machine_ptr->tach_num), &machine_ptr->tach_num);
            if (machine_ptr->tach_num >= TACH_MAX)
                machine_ptr->tach_num = 0;
            
            config_var_read(VID_TACH_LINE, 0, sizeof(machine_ptr->tach_line), &machine_ptr->tach_line);
            if (machine_ptr->tach_line >= DISPLAY_LINE_MAX)
                machine_ptr->tach_line = DISPLAY_OFF;

            config_var_read(VID_TACH_SURFACE, 0, sizeof(machine_ptr->tach_surface), &machine_ptr->tach_surface);

            config_var_read(VID_TACH_DIVISOR, 0, sizeof(machine_ptr->tach_divisor), &machine_ptr->tach_divisor);
            if (machine_ptr->tach_divisor > TACH_DIVISOR_MAX)
                machine_ptr->tach_divisor = TACH_DIVISOR_MAX;
            else if (machine_ptr->tach_divisor < 1)
                machine_ptr->tach_divisor = 1;
            
            config_var_read(VID_EDGE_NUM, 0, sizeof(machine_ptr->edge_num), &machine_ptr->edge_num);
            if (machine_ptr->edge_num >= EDGE_MAX &&
                machine_ptr->edge_num != EDGE_NONE &&
                machine_ptr->edge_num != EDGE_TACH)
            {
                machine_ptr->edge_num = 0;
            }

            for (line_num = 0; line_num < DISPLAY_LINE_MAX; line_num++)
            {
                config_var_read(VID_AXIS, line_num,
                            sizeof(machine_ptr->axis[line_num]),
                            &machine_ptr->axis[line_num]);
                if (machine_ptr->axis[line_num] >= AXIS_MAX)
                    machine_ptr->axis[line_num] = AXIS_OFF;
            }

            config_var_read(VID_WARN_THRESH, 0, sizeof(machine_ptr->warn_thresh), &machine_ptr->warn_thresh);
            config_var_read(VID_WARN_UNITS, 0, sizeof(machine_ptr->warn_units), &machine_ptr->warn_units);

            config_var_read(VID_FEED_LINE, 0, sizeof(machine_ptr->feed_line), &machine_ptr->feed_line);
            if (machine_ptr->feed_line >= DISPLAY_LINE_MAX)
                machine_ptr->feed_line = DISPLAY_OFF;
            
            config_var_read(VID_FEED_AXIS, 0, sizeof(machine_ptr->feed_axis), &machine_ptr->feed_axis);

            config_var_read(VID_BEEPER, 0, sizeof(machine_ptr->beeper), &machine_ptr->beeper);
#if PLATFORM_dpu550
            if (machine_ptr->beeper >= TACH_MAX)
                machine_ptr->beeper = BEEPER_NONE;
#else
            if (machine_ptr->beeper != 0)
                machine_ptr->beeper = BEEPER_NONE;
#endif

            config_var_read(VID_AUTO_OFF, 0, sizeof(machine_ptr->auto_off), &machine_ptr->auto_off);
            config_var_read(VID_AUTO_ON, 0, sizeof(machine_ptr->auto_on), &machine_ptr->auto_on);
            config_var_read(VID_AUTO_UNITS, 0, sizeof(machine_ptr->auto_units), &machine_ptr->auto_units);
            config_var_read(VID_EDGE_BEEP, 0, sizeof(machine_ptr->edge_beep), &machine_ptr->edge_beep);
            config_var_read(VID_PROMPT, 0, sizeof(machine_ptr->prompt), &machine_ptr->prompt);

            config_obj_close();
        }
    }

#if PLATFORM_dpu550
    machine_tach_mux();
#else
    // Drive the beeper pin low
    GPIO_LOW(BEEPER_PORT, BEEPER_DRIVE);
    GPIO_OUTPUT(BEEPER_PORT, BEEPER_DRIVE)

    // Enable the PWM peripheral clock
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PWMC);

    // Select MCLK and left alignment
    AT91C_BASE_PWMC->PWMC_CH[3].PWMC_CMR = 0;

    // Set the period to achieve the resonant frequency
    // needed by the piezo beeper
    AT91C_BASE_PWMC->PWMC_CH[3].PWMC_CPRDR = (MCLK / BEEPER_FREQ);

    // 50% duty cycle
    AT91C_BASE_PWMC->PWMC_CH[3].PWMC_CDTYR = (MCLK / BEEPER_FREQ / 2);

    // Enable PWM3
    AT91C_BASE_PWMC->PWMC_ENA = AT91C_PWMC_CHID3;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// L U A
///////////////////////////////////////////////////////////////////////////////

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lrotable.h"
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
#include "luash.h"

static int
machine_active_lua(lua_State* L)
{
    uint8_t machine;
    
    if (lua_gettop(L) == 0)
    {
        lua_pushinteger(L, machine_get_active() + 1);
        return 1;
    }

    machine = luaL_checkinteger(L, 1) - 1;
    if (machine >= MACHINE_MAX)
        return luaL_error(L, "invalid machine");
        
    machine_set_active(machine);
    
    return 0;
}

static int
machine_inch_lua(lua_State* L)
{
    machine_set_units(AXIS_UNITS_INCH);
    return 0;
}

static int
machine_mm_lua(lua_State* L)
{
    machine_set_units(AXIS_UNITS_MM);
    return 0;
}

static int
machine_abs_lua(lua_State* L)
{
    machine_set_zero(AXIS_ZERO_ABS);
    return 0;
}

static int
machine_inc_lua(lua_State* L)
{
    machine_set_zero(AXIS_ZERO_INC);
    return 0;
}

static int
machine_on_lua(lua_State* L)
{
    machine_on();
    return 0;
}
 
static int
machine_off_lua(lua_State* L)
{
    machine_off();
    return 0;
}

static int
machine_stop_lua(lua_State* L)
{
    machine_stop();
    return 0;
}

static void
machine_beep_cb(void* user)
{
    machine_beeper_off();
}

static int
machine_beep_lua(lua_State* L)
{
    static timer_node_t timer;
    double millisecs;

    if (timer_running(&timer))
        return luaL_error(L, "already beeping");

    millisecs = luaL_checknumber(L, 1);
    if (millisecs <= 0.0)
        return luaL_error(L, "invalid time");

    machine_beeper_on();
    timer_start(&timer, millisecs * MSEC_TICKS, TIMER_ONE_SHOT, machine_beep_cb, NULL);

    return 0;
}

static int
machine_prompt_lua(lua_State* L)
{
    if (lua_gettop(L) == 0)
    {
        lua_pushboolean(L, machine_get_prompt());
        return 1;
    }

    machine_set_prompt(lua_toboolean(L, 1));

    return 0;
}

static int
machine_save_lua(lua_State* L)
{
    machine_save();
    return 0;
}

static int
machine_show_lua(lua_State* L)
{
    machine_show(luash_out(L));
    return 0;
}

const LUA_REG_TYPE machine_map_lua[] =
{
    { LSTRKEY("active"), LFUNCVAL(machine_active_lua) },
    { LSTRKEY("inch"), LFUNCVAL(machine_inch_lua) },
    { LSTRKEY("mm"), LFUNCVAL(machine_mm_lua) },
    { LSTRKEY("abs"), LFUNCVAL(machine_abs_lua) },
    { LSTRKEY("inc"), LFUNCVAL(machine_inc_lua) },
    { LSTRKEY("on"), LFUNCVAL(machine_on_lua) },
    { LSTRKEY("off"), LFUNCVAL(machine_off_lua) },
    { LSTRKEY("stop"), LFUNCVAL(machine_stop_lua) },
    { LSTRKEY("beep"), LFUNCVAL(machine_beep_lua) },
    { LSTRKEY("prompt"), LFUNCVAL(machine_prompt_lua) },
    { LSTRKEY("save"), LFUNCVAL(machine_save_lua) },
    { LSTRKEY("show"), LFUNCVAL(machine_show_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
machine_open_lua(lua_State* L)
{
    LREGISTER(L, "machine", machine_map_lua);
}
