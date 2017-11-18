///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// The scale object implements a digital measuring device
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
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "scale.h"
#include "gpio.h"
#include "timer.h"
#include "drolim.h"
#include "irq.h"
#include "interrupt.h"
#include "fifo.h"
#include "config.h"
#include "display.h"
#include "keypad.h"
#include "function.h"
#include "assert.h"
#include "trace.h"
#if PLATFORM_dpu550
#  include "led.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

// Input pin mask index to signal
#define INPUT_DATA          0       ///< Serial data input
#define INPUT_CLOCK         1       ///< Serial clock input
#define INPUT_A             0       ///< Quadrature phase A input
#define INPUT_B             1       ///< Quadrature phase B input
#define OUTPUT_DATA         0       ///< Serial data output
#define OUTPUT_CLOCK        1       ///< Serial clock output

// Scale auto-detection
#define AUTO_FAIL_THRESH    8       ///< Total weight to switch protocol
#define AUTO_FAIL_WEIGHT    2       ///< Weight for a scale failure
#define AUTO_PASS_WEIGHT    1       ///< Weight for a scale pass

// Scale CPI values
#define CPI_24BIT           20480   ///< 24bit protocol CPI
#define CPI_BCD7            2540    ///< BCD7 protocol CPI
#define CPI_BIN6            2540    ///< BIN6 protocol CPI
#define CPI_21BIT           2560    ///< 21bit protocol CPI

// Scale bit timeouts
#define BIT_TIMEOUT_24BIT   10000   ///< 24bit protocol bit timeout
#define BIT_TIMEOUT_BCD7    10000   ///< BCD7 protocol bit timeout
#define BIT_TIMEOUT_BIN6    10000   ///< BIN6 protocol bit timeout
#define BIT_TIMEOUT_21BIT   10000   ///< 21bit protocol bit timeout

// Fast mode
#define SCALE_FAST_DELAY    (200 * MSEC_TICKS) ///< Delay between fast mode outputs (milliseconds)
#define SCALE_FAST_TIMEOUT  100000  ///< Scale fast mode timeout (milliseconds)

// Exponential moving average constants
#define FILTER_SLOW_EMA     32.0    ///< Slow movement EMA
#define FILTER_FAST_EMA     2.0     ///< Fast movement EMA

// Scale clock generation
#define SCALE_CLK_DUTY      90      ///< Scale clock low duty time (microseconds)
#define SCALE_CLK_PERIOD    120     ///< Scale clock period (microseconds)
#define SCALE_CLK_INTERVAL  50      ///< Scale clock repeat interval (milliseconds)

/// The scale protocol type
typedef enum scale_type
{
    SCALE_TYPE_OFF,         ///< Special value to indicate that a scale is off
    SCALE_TYPE_24BIT,       ///< 24bit protocol
    SCALE_TYPE_BCD7,        ///< BCD7 protocol
    SCALE_TYPE_BIN6,        ///< BIN6 protocol
    SCALE_TYPE_QUADRATURE,  ///< Quadrature encoder
    SCALE_TYPE_AUTO,        ///< Auto-detect scale (except quadrature)
    SCALE_TYPE_21BIT,       ///< 21bit protocol
    SCALE_TYPE_MAX,         ///< MUST be last!!!
} scale_type_t;

/// The scale object
typedef struct scale
{
    uint8_t         scale_num;              ///< Scale number (0..SCALE_MAX-1)
    uint8_t         state;                  ///< Quadrature state
    uint8_t         filter_pos;             ///< Current position in the filter buffer   
    uint8_t         filter_prev;            ///< Previous position filter calculated on
    uint8_t         bit_num;                ///< Number of bits captured
    int8_t          auto_weight;            ///< Scale auto detect weighting factor
    scale_type_t    config_type;            ///< Configured type of scale
    scale_type_t    active_type;            ///< Active scale type
    pin_t           in_pin[SCALE_IN_MAX];   ///< Scale input pin mask
    pin_t           last_pins;              ///< Last captured set of pins
    bool            fast_mode;              ///< Set if scale is in fast mode
    bool            invert;                 ///< Flag to invert the scale counts
    bool            filter_enable;          ///< Enable the adaptive moving average filter
    int32_t         filter_buf[FILTER_MAX]; ///< Averaging filter count buffer
    int32_t         count;                  ///< Current scale count
    uint32_t        cpi;                    ///< Counts per inch
    uint32_t        bit_values;             ///< Captured bit values
    uint32_t        bit_time;               ///< Time last bit was captured
    double          filter_ama;             ///< Adaptive moving average
} scale_t;

/// Configuration variable IDs
enum scale_vid
{
    VID_TYPE,       ///< Scale type
    VID_CPI,        ///< Counts per inch
    VID_INVERT,     ///< Invert the scale counts
    VID_FILTER,     ///< Adaptive moving average filter
    VID_MAX         ///< MUST be last!!!
};

static const char*
scale_type_str(scale_type_t type);

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

/// Scale object instances
static scale_t scale[SCALE_MAX];

/// Timer for fast mode logic (For 24bit and BCD7 scales)
static timer_node_t fast_timer;

/// State for fast mode sequence
static uint8_t fast_state;

/// Count of scale clock cycles
static uint32_t clk_cycles;

/// Timer for scale clock
static timer_node_t clk_timer;

///////////////////////////////////////////////////////////////////////////////
// F I L T E R
///////////////////////////////////////////////////////////////////////////////

static void
scale_filter_add(scale_t* scale_ptr, uint32_t count)
{
    scale_ptr->filter_buf[scale_ptr->filter_pos] = count;
    scale_ptr->filter_pos++;
    if (scale_ptr->filter_pos >= FILTER_MAX)
        scale_ptr->filter_pos = 0;
}

static bool
scale_filter_active(scale_t* scale_ptr)
{
    switch (scale_ptr->active_type)
    {
    case SCALE_TYPE_24BIT:
    case SCALE_TYPE_BCD7:
        return (scale_ptr->fast_mode == true && scale_ptr->filter_enable);
    case SCALE_TYPE_BIN6:
    case SCALE_TYPE_21BIT:
        return scale_ptr->filter_enable;
    default:
        return false;
    }
}

static int32_t
scale_filter_calc(scale_t* scale_ptr)
{
    int32_t direction;
    uint8_t curr_pos;
    uint8_t first_pos;
    uint8_t last_pos;
    uint8_t prev_pos;
    uint8_t filter_pos;
    int32_t volatility = 0;
    int32_t min_count = INT32_MAX;
    int32_t max_count = INT32_MIN;
    int32_t count_range;
    double  eff_ratio;
    double  fast_sc;
    double  slow_sc;
    double  ssc;
    double  constant;

    // Grab the current filter position in case a new count comes in while calculating the AMA
    curr_pos = scale_ptr->filter_pos;

    // Return the previous AMA if we already calculated it at the current filter position
    if (curr_pos == scale_ptr->filter_prev)
        return lround(scale_ptr->filter_ama);

    // Determine the direction of travel by taking the difference between the first and
    // last counts in the buffer
    last_pos = (curr_pos + FILTER_MAX - 1) % FILTER_MAX;
    first_pos = (curr_pos + FILTER_MAX + 1) % FILTER_MAX;
    direction = scale_ptr->filter_buf[first_pos] - scale_ptr->filter_buf[last_pos];

    // Save the filter position the AMA is calculated on
    scale_ptr->filter_prev = curr_pos;

    // Calculate the volatility in the counts by taking the sum of the differences
    prev_pos = first_pos;
    for (filter_pos = (first_pos + 1) % FILTER_MAX;
         filter_pos != curr_pos;
         filter_pos = (filter_pos + 1) % FILTER_MAX)
    {
        min_count = MIN(min_count, scale_ptr->filter_buf[filter_pos]);
        max_count = MAX(max_count, scale_ptr->filter_buf[filter_pos]);
        volatility += ABS(scale_ptr->filter_buf[filter_pos] - scale_ptr->filter_buf[prev_pos]);
        prev_pos = filter_pos;
    }

    // Just return the count if there is no volatility to avoid divide by 0
    if (volatility == 0)
    {
        scale_ptr->filter_ama = scale_ptr->filter_buf[last_pos];
        return scale_ptr->filter_buf[last_pos];
    }

    // If the last AMA is not within twice the sample range, then assume the position jumped
    // and reset the AMA to the current count
    count_range = max_count - min_count;
    if (scale_ptr->filter_ama > max_count + count_range + 1 ||
        scale_ptr->filter_ama < min_count - count_range - 1)
    {
        scale_ptr->filter_ama = (double) scale_ptr->filter_buf[last_pos];
        return scale_ptr->filter_buf[last_pos];
    }

    // Calculate the smoothing constant
    eff_ratio = fabs((double) direction / (double) (volatility));
    fast_sc = 2.0 / (FILTER_FAST_EMA + 1.0);
    slow_sc = 2.0 / (FILTER_SLOW_EMA + 1.0);
    ssc = eff_ratio * (fast_sc - slow_sc) + slow_sc;
    constant = ssc * ssc;

    // Calculate the new average
    scale_ptr->filter_ama = (scale_ptr->filter_ama + constant *
        ((double) scale_ptr->filter_buf[last_pos] - scale_ptr->filter_ama));

    return lround(scale_ptr->filter_ama);
}

///////////////////////////////////////////////////////////////////////////////
// Q U A D R A T U R E   E N C O D E R
///////////////////////////////////////////////////////////////////////////////

// FSM action function
typedef void (*fsm_action_t) (scale_t *scale_ptr);

// FSM action to increment the count by one.
static void 
increment(scale_t *scale_ptr)
{
    scale_ptr->count++;
}

// FSM action to decrement the count by one.
static void 
decrement(scale_t *scale_ptr)
{
    scale_ptr->count--;
}

// FSM do nothing action
static void 
do_nothing(scale_t *scale_ptr)
{}

// Quadrature scale FSM where the state is represented by
// combining the two quadrature phase signals A & B
static const fsm_action_t quadrature_fsm[4][4] =
{
    // Current state: ~A ~B
    {
        do_nothing,     // ~A ~B  
        increment,      //  A ~B
        decrement,      // ~A  B
        do_nothing,     //  A  B
    },  
    // Current state: A ~B
    {
        decrement,      // ~A ~B
        do_nothing,     //  A ~B 
        do_nothing,     // ~A  B
        increment,      //  A  B 
    },  
    // Current state: ~A B
    {
        increment,      // ~A ~B 
        do_nothing,     //  A ~B 
        do_nothing,     // ~A  B
        decrement,      //  A  B 
    },  
    // Current state: A B
    {
        do_nothing,     // ~A ~B
        decrement,      //  A ~B 
        increment,      // ~A  B
        do_nothing,     //  A  B 
    },  
};

// Quadrature scale signal processing
static void
scale_quadrature(scale_t*   scale_ptr,
                 pin_t      pins)
{
    uint8_t new_state;

    // Determine the new quadrature state   
    new_state = 0;
    if (pins & scale_ptr->in_pin[INPUT_A])
        new_state |= 1;
    if (pins & scale_ptr->in_pin[INPUT_B])
        new_state |= 2;

    // Process the state change     
    quadrature_fsm[scale_ptr->state][new_state](scale_ptr);
    
    // Save the new state
    scale_ptr->state = new_state;
}

///////////////////////////////////////////////////////////////////////////////
// A U T O - D E T E C T   P R O T O C O L
///////////////////////////////////////////////////////////////////////////////

static void
scale_set_active(scale_t* scale_ptr)
{
    // If not auto, set the active type to the configured type
    if (scale_ptr->config_type != SCALE_TYPE_AUTO)
    {
        scale_ptr->active_type = scale_ptr->config_type;
        return;
    }

    // Switch to the next scale type
    switch (scale_ptr->active_type)
    {
    case SCALE_TYPE_24BIT:
        scale_ptr->active_type = SCALE_TYPE_BCD7;
        scale_ptr->cpi = CPI_BCD7;
        break;
    case SCALE_TYPE_BCD7:
        scale_ptr->active_type = SCALE_TYPE_BIN6;
        scale_ptr->cpi = CPI_BIN6;
        break;
    case SCALE_TYPE_BIN6:
        scale_ptr->active_type = SCALE_TYPE_21BIT;
        scale_ptr->cpi = CPI_21BIT;
        break;
    case SCALE_TYPE_21BIT:
    default:
        scale_ptr->active_type = SCALE_TYPE_24BIT;
        scale_ptr->cpi = CPI_24BIT;
        break;
    }

    scale_ptr->auto_weight = 0;

    TRACE_DEBUG("active scale %d type %s\n",
                scale_ptr->scale_num,
                scale_type_str(scale_ptr->active_type));
}

static void
scale_auto_fail(scale_t* scale_ptr)
{
    // Check if we have enough weight for a switch
    scale_ptr->auto_weight += AUTO_FAIL_WEIGHT;
    if (scale_ptr->auto_weight < AUTO_FAIL_THRESH)
        return;

    TRACE_DEBUG("fail scale %d bit_num %d\n",
                scale_ptr->scale_num,
                scale_ptr->bit_num);

    scale_set_active(scale_ptr);
}

static void
scale_auto_pass(scale_t* scale_ptr)
{
    scale_ptr->auto_weight -= AUTO_PASS_WEIGHT;
    if (scale_ptr->auto_weight < 0)
        scale_ptr->auto_weight = 0;
}

///////////////////////////////////////////////////////////////////////////////
// 2 4 B I T   P R O T O C O L
///////////////////////////////////////////////////////////////////////////////

// 24 bit protocol signal processing
static void
scale_24bit(scale_t*    scale_ptr,
            pin_t       pins)
{
    uint32_t time;
    uint32_t period;
    
    // Process the scale only if the clock is falling
    if ((scale_ptr->last_pins & scale_ptr->in_pin[INPUT_CLOCK]) &&
       !(pins & scale_ptr->in_pin[INPUT_CLOCK]))
    {
        // Get the system timer
        time = timer_get() * TIMER_PERIOD;

        // Calculate the period between bits
        period = time - scale_ptr->bit_time;

        // Reset to the first bit if over the bit timeout
        if (period > BIT_TIMEOUT_24BIT)
        {
            // Check for an auto-detection switch
            // Allow an extra bit for 24 bit scales since they glitch
            if (scale_ptr->config_type == SCALE_TYPE_AUTO &&
                scale_ptr->bit_num != 49 &&
                scale_ptr->bit_num != 50)
                scale_auto_fail(scale_ptr);
            else
                scale_auto_pass(scale_ptr);

            scale_ptr->bit_num = 0;
            scale_ptr->bit_values = 0;

            // Assume the scale is in fast mode if the period is less than 100ms
            if (period < SCALE_FAST_TIMEOUT)
                scale_ptr->fast_mode = true;
            else
                scale_ptr->fast_mode = false;
        }

        // Save the bit time
        scale_ptr->bit_time = time;

        // Process 48 bits
        if (scale_ptr->bit_num < 48)
        {
            // Capture the first 24 bits
            if (scale_ptr->bit_num < 24)
            {
                // Store the captured bit
                if (pins & scale_ptr->in_pin[INPUT_DATA])
                    scale_ptr->bit_values |= (1 << scale_ptr->bit_num);
            }

            // On the 48th bit, move the bit values to the count
            if (scale_ptr->bit_num == 47)
            {
                // Sign extend the if the MSB is set
                if (scale_ptr->bit_values & (1 << 23))
                    scale_ptr->bit_values |= (0xff << 24);

                // Save the scale count
                scale_ptr->count = scale_ptr->bit_values;

                // Add count to the filter
                scale_filter_add(scale_ptr, scale_ptr->count);
            }
        }

        // Move to the next bit
        scale_ptr->bit_num++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// B C D 7   P R O T O C O L
///////////////////////////////////////////////////////////////////////////////

// Convert a 6 digit BCD value to a 24 bit binary number
static uint32_t
bcd_to_bin24(uint32_t bcd)
{
    // The ARM7 has a hardware multiplier so this should be
    // reasonably efficient
    return ((bcd >> 0) & 0xf)
         + (((bcd >> 4) & 0xf)  * 10)
         + (((bcd >> 8) & 0xf)  * 100)
         + (((bcd >> 12) & 0xf) * 1000)
         + (((bcd >> 16) & 0xf) * 10000)
         + (((bcd >> 20) & 0xf) * 100000);
}

// BCD7 protocol signal processing
static void
scale_bcd7(scale_t* scale_ptr,
           pin_t    pins)
{
    uint32_t time;
    uint32_t period;

    // Process the scale only if the clock is falling
    if ((scale_ptr->last_pins & scale_ptr->in_pin[INPUT_CLOCK]) &&
       !(pins & scale_ptr->in_pin[INPUT_CLOCK]))
    {
        // Get the system timer
        time = timer_get() * TIMER_PERIOD;

        // Calculate the period between bits
        period = time - scale_ptr->bit_time;

        // Reset to the first bit if over the bit timeout
        if (period > BIT_TIMEOUT_BCD7)
        {
            // Check for an auto-detection switch
            if (scale_ptr->config_type == SCALE_TYPE_AUTO &&
                scale_ptr->bit_num != 29)
                scale_auto_fail(scale_ptr);
            else
                scale_auto_pass(scale_ptr);

            scale_ptr->bit_num = 0;
            scale_ptr->bit_values = 0;

            // Assume the scale is in fast mode if the period is less than 100ms
            if (period < SCALE_FAST_TIMEOUT)
                scale_ptr->fast_mode = true;
            else
                scale_ptr->fast_mode = false;
        }

        // Save the bit time
        scale_ptr->bit_time = time;

        // Capture 28 bits
        if (scale_ptr->bit_num < 28)
        {
            // Store the captured bit
            if (pins & scale_ptr->in_pin[INPUT_DATA])
                scale_ptr->bit_values |= (1 << scale_ptr->bit_num);

            // If this is the 28th bit, then process the bit values
            if (scale_ptr->bit_num == 27)
            {
                // Convert the first 24 bit values from BCD to binary
                scale_ptr->count = bcd_to_bin24(scale_ptr->bit_values & 0xffffff);

                // Complement the count if the negative bit is set
                if (scale_ptr->bit_values & (1 << 24))
                    scale_ptr->count = -scale_ptr->count;

                // Add count to the filter
                scale_filter_add(scale_ptr, scale_ptr->count);
            }
        }

        // Move to the next bit
        scale_ptr->bit_num++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// B I N 6   P R O T O C O L
///////////////////////////////////////////////////////////////////////////////

// BIN6 protocol signal processing
static void
scale_bin6(scale_t* scale_ptr,
           pin_t    pins)
{
    uint32_t time;
    uint32_t period;

    // Process the scale only if the clock is rising
    if (!(scale_ptr->last_pins & scale_ptr->in_pin[INPUT_CLOCK]) &&
        (pins & scale_ptr->in_pin[INPUT_CLOCK]))
    {
        // Get the system timer
        time = timer_get() * TIMER_PERIOD;

        // Calculate the period between bits
        period = time - scale_ptr->bit_time;

        // Reset to the first bit if over the bit timeout
        if (period > BIT_TIMEOUT_BIN6)
        {
            // Check for an auto-detection switch
            if (scale_ptr->config_type == SCALE_TYPE_AUTO &&
                scale_ptr->bit_num != 24)
                scale_auto_fail(scale_ptr);
            else
                scale_auto_pass(scale_ptr);

            scale_ptr->bit_num = 0;
            scale_ptr->bit_values = 0;
        }

        // Save the bit time
        scale_ptr->bit_time = time; 

        // Capture 24 bits
        if (scale_ptr->bit_num < 24)
        {
            // Store the captured bit
            if (pins & scale_ptr->in_pin[INPUT_DATA])
                scale_ptr->bit_values |= (1 << scale_ptr->bit_num);

            // If this is the 24th bit, then process the bit values
            if (scale_ptr->bit_num == 23)
            {
                // Mask off the bottom 20 bits
                scale_ptr->count = scale_ptr->bit_values & 0xfffff;

                // Complement the count if the negative bit is set
                if (scale_ptr->bit_values & (1 << 20))
                    scale_ptr->count = -scale_ptr->count;

                // Add count to the filter
                scale_filter_add(scale_ptr, scale_ptr->count);
            }
        }

        // Move to the next bit
        scale_ptr->bit_num++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// 2 1 B I T   P R O T O C O L
///////////////////////////////////////////////////////////////////////////////

// 21 bit protocol signal processing
static void
scale_21bit(scale_t*    scale_ptr,
            pin_t       pins)
{
    uint32_t time;
    uint32_t period;

    // Process the scale only if the clock is falling
    if ((scale_ptr->last_pins & scale_ptr->in_pin[INPUT_CLOCK]) &&
       !(pins & scale_ptr->in_pin[INPUT_CLOCK]))
    {
        // Get the system timer
        time = timer_get() * TIMER_PERIOD;

        // Calculate the period between bits
        period = time - scale_ptr->bit_time;

        // Reset to the first bit if over the bit timeout
        if (period > BIT_TIMEOUT_21BIT)
        {
            // Check for an auto-detection switch
            if (scale_ptr->config_type == SCALE_TYPE_AUTO &&
                scale_ptr->bit_num != 21)
                scale_auto_fail(scale_ptr);
            else
                scale_auto_pass(scale_ptr);

            scale_ptr->bit_num = 0;
            scale_ptr->bit_values = 0;
        }

        // Save the bit time
        scale_ptr->bit_time = time;

        // Process 21 bits
        if (scale_ptr->bit_num < 21)
        {
            // Capture the first 21 bits
            if (scale_ptr->bit_num < 21)
            {
                // Store the captured bit
                if (pins & scale_ptr->in_pin[INPUT_DATA])
                    scale_ptr->bit_values |= (1 << scale_ptr->bit_num);
            }

            // On the 21st bit, move the bit values to the count
            if (scale_ptr->bit_num == 20)
            {
                // Sign extend the if the MSB is set
                if (scale_ptr->bit_values & (1 << 20))
                    scale_ptr->bit_values |= (0x7ff << 21);

                // Save the scale count
                scale_ptr->count = scale_ptr->bit_values;

                // Add count to the filter
                scale_filter_add(scale_ptr, scale_ptr->count);
            }
        }

        // Move to the next bit
        scale_ptr->bit_num++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

#if PLATFORM_dpu550
void
scale_switch(uint8_t  scale,
             pin_t*   clk,
             pin_t*   data,
             pin_t*   latch)
{
    static pin_t clk_map[SCALE_MAX] =
    {
        DATA0,
        DATA2,
        DATA4,
#if !CONFIG_lite
        DATA0,
        DATA2
#endif
    };
    static pin_t data_map[SCALE_MAX] =
    {
        DATA1,
        DATA3,
        DATA5,
#if !CONFIG_lite
        DATA1,
        DATA3
#endif
    };
    static pin_t latch_map[SCALE_MAX] =
    {
        SWITCH_LATCH0,
        SWITCH_LATCH0,
        SWITCH_LATCH0,
#if !CONFIG_lite
        SWITCH_LATCH1,
        SWITCH_LATCH1
#endif
    };

    ASSERT(scale < SCALE_MAX);

    *clk = clk_map[scale];
    *data = data_map[scale];
    *latch = latch_map[scale];
}
#endif

#if PLATFORM_dro550
static pin_t
scale_out_pin(uint8_t scale,
              uint8_t output)
{
    static pin_t pin_map[SCALE_MAX][SCALE_OUT_MAX] =
    {
        { SCALE0_OUT0, SCALE0_OUT1 },
        { SCALE1_OUT0, SCALE1_OUT1 },
        { SCALE2_OUT0, SCALE2_OUT1 },
        { SCALE3_OUT0, SCALE3_OUT1 },
        { SCALE4_OUT0, SCALE4_OUT1 },
    };

    ASSERT(scale < SCALE_MAX);
    ASSERT(output < SCALE_OUT_MAX);

    return pin_map[scale][output];
}
#endif

static pin_t
scale_in_pin(uint8_t scale,
             uint8_t input)
{
    static pin_t pin_map[SCALE_MAX][SCALE_IN_MAX] =
    {
        { SCALE0_IN0, SCALE0_IN1 },
        { SCALE1_IN0, SCALE1_IN1 },
        { SCALE2_IN0, SCALE2_IN1 },
#if !CONFIG_lite && !PLATFORM_dro375
        { SCALE3_IN0, SCALE3_IN1 },
        { SCALE4_IN0, SCALE4_IN1 },
#endif
    };

    ASSERT(scale < SCALE_MAX);
    ASSERT(input < SCALE_IN_MAX);

    return pin_map[scale][input];
}

static const char*
scale_type_str(scale_type_t type)
{
    static const char *type_names[SCALE_TYPE_MAX] = {
        [SCALE_TYPE_OFF]        = "off",
        [SCALE_TYPE_24BIT]      = "24bit",
        [SCALE_TYPE_BCD7]       = "bcd7",
        [SCALE_TYPE_BIN6]       = "bin6",
        [SCALE_TYPE_QUADRATURE] = "quadrature",
        [SCALE_TYPE_AUTO]       = "auto",
        [SCALE_TYPE_21BIT]      = "21bit",
    };

    ASSERT(type < SCALE_TYPE_MAX);

    return type_names[type];
}

static void
scale_display_type(uint8_t obj_inst,
                   uint8_t var_inst)
{
    scale_t*    scale_ptr = &scale[obj_inst];

    display_menu_puts(MENU_BOTTOM, scale_type_str(scale_ptr->config_type));
}

static void
scale_select_type(uint8_t obj_inst,
                  uint8_t var_inst)
{
    scale_t* scale_ptr = &scale[obj_inst];

    scale_ptr->config_type++;
    if (scale_ptr->config_type == SCALE_TYPE_MAX)
        scale_ptr->config_type = 0;

    // Set the active scale type
    scale_set_active(scale_ptr);
}

static void
scale_display_cpi(uint8_t obj_inst,
                  uint8_t var_inst)
{
    scale_t* scale_ptr = &scale[obj_inst];

    display_menu_printf(MENU_BOTTOM, "%"PRId32, scale_ptr->cpi);
}

static void
scale_select_cpi(uint8_t obj_inst,
                 uint8_t var_inst)
{
    scale_t*    scale_ptr = &scale[obj_inst];
    int32_t cpi;
    
    cpi = scale_ptr->cpi;
    if (keypad_get_number(MENU_BOTTOM, 1, 999999, &cpi))
        scale_ptr->cpi = cpi;
}

static void
scale_display_invert(uint8_t obj_inst,
                     uint8_t var_inst)
{
    scale_t* scale_ptr = &scale[obj_inst];

    display_menu_puts(MENU_BOTTOM, scale_ptr->invert ? "yes" : "no");
}

static void
scale_select_invert(uint8_t obj_inst,
                    uint8_t var_inst)
{
    scale_t *scale_ptr = &scale[obj_inst];

    scale_ptr->invert = (scale_ptr->invert ? false : true);
}

static void
scale_display_filter(uint8_t obj_inst,
                     uint8_t var_inst)
{
    scale_t *scale_ptr = &scale[obj_inst];

    display_menu_printf(MENU_BOTTOM, scale_ptr->filter_enable ? "yes" : "no");
}

static void
scale_select_filter(uint8_t obj_inst,
                    uint8_t var_inst)
{
    scale_t *scale_ptr = &scale[obj_inst];

    scale_ptr->filter_enable = !scale_ptr->filter_enable;
}

///////////////////////////////////////////////////////////////////////////////
// I N T E R R U P T   H A N D L E R
///////////////////////////////////////////////////////////////////////////////

static void
scale_isr(void)
{
    uint8_t         scale_num;
    scale_t*        scale_ptr;
    pin_t           pins;
    pin_t           change;
#ifdef FIFO_WATERMARK
    static uint32_t high_water = 0;
    uint32_t        count;
#endif

    while (!fifo32_empty(&gpio_fifo))
    {
#ifdef FIFO_WATERMARK
        count = fifo32_count(&gpio_fifo);
        if (count > high_water)
            high_water = count;
#endif

        // Read the GPIO pins from the FIFO
        pins = fifo32_read(&gpio_fifo);

#ifdef INVERTING_INPUTS
        // The inputs have inverting logic so invert them back
        pins = ~pins;
#endif

        for (scale_num = 0; scale_num < SCALE_MAX; scale_num++)
        {
            scale_ptr = &scale[scale_num];
            if (scale_ptr->active_type != SCALE_TYPE_OFF)
            {
                // XOR the pins with the last ones to determine which changed
                change = scale_ptr->last_pins ^ pins;

                if (change & (scale_ptr->in_pin[0] | scale_ptr->in_pin[1]))
                {
                    switch (scale_ptr->active_type)
                    {
                    case SCALE_TYPE_QUADRATURE:
                        scale_quadrature(scale_ptr, pins);
                        break;  
                    case SCALE_TYPE_24BIT:
                        scale_24bit(scale_ptr, pins);
                        break;  
                    case SCALE_TYPE_BCD7:
                        scale_bcd7(scale_ptr, pins);
                        break;
                    case SCALE_TYPE_BIN6:
                        scale_bin6(scale_ptr, pins);
                        break;
                    case SCALE_TYPE_21BIT:
                        scale_21bit(scale_ptr, pins);
                        break;
                    default:
                        ASSERT(0);
                        break;
                    }
                }
                scale_ptr->last_pins = pins;
            }
        }
    }
}

static void
scale_fast_timeout(void* user)
{
    irq_trigger(IRQ_SCALE_FAST);
}

static bool
scale_fast_capable(scale_t* scale_ptr)
{
    return (scale_ptr->active_type == SCALE_TYPE_24BIT ||
            scale_ptr->active_type == SCALE_TYPE_BCD7);
}

#if PLATFORM_dro375

/// Flag to proceed to next fast state
static bool next_state;

void
scale_check_fast(void)
{
    uint8_t        scale_num;
    scale_t*       scale_ptr;
    
    if (!next_state)
        return;

    scale_num = fast_state / 2;
    scale_ptr = &scale[scale_num];
    
    GPIO_HIGH(SWITCH_PORT, SWITCH_SELECT0);
    GPIO_HIGH(SWITCH_PORT, SWITCH_SELECT1);
    GPIO_HIGH(SWITCH_PORT, SWITCH_SELECT2);
    
    if (scale_num < SCALE_MAX && scale_fast_capable(scale_ptr) && scale_ptr->fast_mode == false)
    {
        uint8_t select_map[2 * SCALE_MAX] = { 0x0, 0x3, 0x1, 0x4, 0x2, 0x5 };
        
        if (!(select_map[fast_state] & 0x1))
            GPIO_LOW(SWITCH_PORT, SWITCH_SELECT0)

        if (!(select_map[fast_state] & 0x2))
            GPIO_LOW(SWITCH_PORT, SWITCH_SELECT1)

        if (!(select_map[fast_state] & 0x4))
            GPIO_LOW(SWITCH_PORT, SWITCH_SELECT2)
    }
    
    // Latch the selection
    GPIO_HIGH(SWITCH_PORT, SWITCH_LATCH);
    GPIO_LOW(SWITCH_PORT, SWITCH_LATCH);
    
    fast_state++;
    next_state = false;
    
    if (fast_state <= 2 * SCALE_MAX)
        timer_start(&fast_timer, SCALE_FAST_DELAY, TIMER_ONE_SHOT, scale_fast_timeout, NULL);
}

static void
scale_fast_isr(void)
{
    next_state = true;
}

#elif PLATFORM_dpu550
static void
scale_fast_isr(void)
{
    pin_t       data;
    pin_t       clk;
    pin_t       latch;
    uint8_t     scale_num;
    scale_t*    scale_ptr;
    uint8_t     search_scale;
    pin_t       search_clk;
    pin_t       search_data;
    pin_t       search_latch;
    bool        send_fast[SCALE_MAX];

    // Find scales that need a fast mode signal sent
    for (scale_num = 0; scale_num < SCALE_MAX; scale_num++)
    {
        scale_ptr = &scale[scale_num];
        
        if (scale_fast_capable(scale_ptr) && scale_ptr->fast_mode == false)
            send_fast[scale_num] = true;
        else
            send_fast[scale_num] = false;
    }

    for (scale_num = 0; scale_num < SCALE_MAX; scale_num++)
    {
        // Check if fast signal should be sent to scale
        if (send_fast[scale_num])
        {
            send_fast[scale_num] = false;

            // Get the switch parameters for the scale
            scale_switch(scale_num, &clk, &data, &latch);

            // Search for scales that use the same latch
            for (search_scale = scale_num + 1; search_scale < SCALE_MAX; search_scale++)
            {
                if (send_fast[search_scale])
                {
                    scale_switch(search_scale, &search_clk, &search_data, &search_latch);

                    // If this scale uses the same latch, then combine the clock and data pins
                    if (search_latch == latch)
                    {
                        send_fast[search_scale] = false;
                        clk |= search_clk;
                        data |= search_data;
                    }
                }
            }

            led_stop();

            GPIO_DATA_SET(SWITCH_PORT, SWITCH_ALL, data);
            GPIO_LOW(SWITCH_PORT, latch);
            timer_sleep(SCALE_FAST_DELAY * TIMER_PERIOD);
            GPIO_HIGH(SWITCH_PORT, latch);

            timer_sleep(SCALE_FAST_DELAY * TIMER_PERIOD);

            GPIO_DATA_SET(SWITCH_PORT, SWITCH_ALL, clk);
            GPIO_LOW(SWITCH_PORT, latch);
            timer_sleep(SCALE_FAST_DELAY * TIMER_PERIOD);
            GPIO_HIGH(SWITCH_PORT, latch);

            led_start();
        }
    }
}
#elif PLATFORM_dro550
static void
scale_fast_isr(void)
{
    uint8_t     scale_num;
    pin_t       data_pins = 0;
    pin_t       clock_pins = 0;
    scale_t*    scale_ptr;

    // Find scales that need a fast mode signal sent
    for (scale_num = 0; scale_num < SCALE_MAX; scale_num++)
    {
        scale_ptr = &scale[scale_num];
        
        if (scale_fast_capable(scale_ptr) && scale_ptr->fast_mode == false)
        {
            data_pins |= scale_out_pin(scale_num, OUTPUT_DATA);
            clock_pins |= scale_out_pin(scale_num, OUTPUT_CLOCK);
        }
    }

    if (fast_state == 0)
        GPIO_LOW(SCALE_OUT_PORT, data_pins)
    else if (fast_state == 1)
        GPIO_HIGH(SCALE_OUT_PORT, data_pins)
    else if (fast_state == 2)
        GPIO_LOW(SCALE_OUT_PORT, clock_pins)
    else
        GPIO_HIGH(SCALE_OUT_PORT, clock_pins)

    if (fast_state++ < 3)
        timer_start(&fast_timer, SCALE_FAST_DELAY, TIMER_ONE_SHOT, scale_fast_timeout, NULL);
}
#endif

static void
scale_clk_isr(void)
{
    uint32_t isr;
    
    // Disable all PWM channels if we reach our cycle count
    if (++clk_cycles >= 20)
        AT91C_BASE_PWMC->PWMC_DIS = AT91C_PWMC_CHID2 | AT91C_PWMC_CHID1 | AT91C_PWMC_CHID0;
    
    // Clear the interrupt
    isr = AT91C_BASE_PWMC->PWMC_ISR;
}

static void
scale_clk_timeout(void* user)
{
    // Clear the cycle count
    clk_cycles = 0;
    
    // Enable all PWM channels
    AT91C_BASE_PWMC->PWMC_ENA = AT91C_PWMC_CHID2 | AT91C_PWMC_CHID1 | AT91C_PWMC_CHID0;
}

static void
scale_clk_init(void)
{
    uint32_t duty;
    uint32_t period;
    
    // Enable the PWM peripheral clock
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PWMC);
    
#if PLATFORM_dro375
    // Disable pullup on the scale clock pin
    GPIO_NOPULLUP(SCALE_CLK_PORT, SCALE_CLK_OUT);

    // Assign the scale clock pin to the PWM peripheral
    GPIO_PERIPH_A(SCALE_CLK_PORT, SCALE_CLK_OUT);
#elif PLATFORM_dro550
    // Disable pullups on the PWM pins
    GPIO_NOPULLUP(PWM_OUT_PORT, PWM_OUT_ALL);

    // Assign the PWM pins to the PWM peripheral
    GPIO_PERIPH_A(PWM_OUT_PORT, PWM_OUT_ALL);
#elif PLATFORM_dpu550
    // The scale clock output is multiplexed with TACH1 in the machine object
#endif

    // Use MCLK, left aligned, low polarity for all channels
    AT91C_BASE_PWMC->PWMC_CH[0].PWMC_CMR = 0;
    AT91C_BASE_PWMC->PWMC_CH[1].PWMC_CMR = 0;
    AT91C_BASE_PWMC->PWMC_CH[2].PWMC_CMR = 0;

    // Set the clock duty
    duty = (MCLK / 1000000) * SCALE_CLK_DUTY;
    AT91C_BASE_PWMC->PWMC_CH[0].PWMC_CDTYR = duty;
    AT91C_BASE_PWMC->PWMC_CH[1].PWMC_CDTYR = duty;
    AT91C_BASE_PWMC->PWMC_CH[2].PWMC_CDTYR = duty;

    // Set the clock period
    period = (MCLK / 1000000) * SCALE_CLK_PERIOD;
    AT91C_BASE_PWMC->PWMC_CH[0].PWMC_CPRDR = period;
    AT91C_BASE_PWMC->PWMC_CH[1].PWMC_CPRDR = period;
    AT91C_BASE_PWMC->PWMC_CH[2].PWMC_CPRDR = period;

    // Register the interrupt handle
    irq_register(AT91C_ID_PWMC, IRQ_PRIORITY_HIGHEST, IRQ_TYPE_LEVEL, scale_clk_isr);
    
    // Enable interrupts for all channels
    AT91C_BASE_PWMC->PWMC_IER = AT91C_PWMC_CHID2 | AT91C_PWMC_CHID1 | AT91C_PWMC_CHID0;
    
    // Start the clock repeat interval timer
    timer_start(&clk_timer, SCALE_CLK_INTERVAL * MSEC_TICKS, TIMER_RECURRING, scale_clk_timeout, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
scale_send_fast(void)
{
    fast_state = 0;
    irq_trigger(IRQ_SCALE_FAST);
}

int32_t
scale_get_count(uint8_t scale_num)
{
    int32_t count;
    scale_t *scale_ptr;

    ASSERT(scale_num < SCALE_MAX);

    scale_ptr = &scale[scale_num];

    // Apply filter if it is active
    if (scale_filter_active(scale_ptr))
        count = scale_filter_calc(scale_ptr);
    else
        count = scale_ptr->count;

    if (scale_ptr->invert)
        count = -count;

    return count;
}

bool
scale_get_stats(uint8_t scale_num,
                double* mean,
                double* variance)
{
    uint8_t     filter_pos;
    double      diff;
    scale_t*    scale_ptr;

    ASSERT(scale_num < SCALE_MAX);

    scale_ptr = &scale[scale_num];

    if (!scale_filter_active(scale_ptr))
        return false;

    // Calculate the mean value  
    *mean = 0.0;
    for (filter_pos = 0; filter_pos < FILTER_MAX; filter_pos++)
        *mean += scale_ptr->filter_buf[filter_pos];
    *mean /= FILTER_MAX;

    // Calculate the variance
    *variance = 0.0;
    for (filter_pos = 0; filter_pos < FILTER_MAX; filter_pos++)
    {
        diff = scale_ptr->filter_buf[filter_pos] - *mean;
        *variance += diff * diff;
    }
    *variance /= FILTER_MAX;

    return true;
}

uint32_t
scale_get_cpi(uint8_t scale_num)
{
    ASSERT(scale_num < SCALE_MAX);

    return scale[scale_num].cpi;
}

void
scale_save(void)
{
    scale_t*    scale_ptr;
    uint8_t     scale_num;

    for (scale_num = 0; scale_num < SCALE_MAX; scale_num++)
    {
        scale_ptr = &scale[scale_num];

        if (config_obj_write(CONFIG_OID_SCALE, scale_num))
        {
            config_var_write(VID_TYPE, 0, sizeof(scale_ptr->config_type), &scale_ptr->config_type);
            config_var_write(VID_CPI, 0, sizeof(scale_ptr->cpi), &scale_ptr->cpi);
            config_var_write(VID_INVERT, 0, sizeof(scale_ptr->invert), &scale_ptr->invert);
            config_var_write(VID_FILTER, 0, sizeof(scale_ptr->filter_enable), &scale_ptr->filter_enable);
            config_obj_close();
        }
    }
}

void
scale_setup(void)
{
    static const var_list_t var_list[] = {
        { "type", 0, scale_display_type, scale_select_type },
        { "cpi", 0, scale_display_cpi, scale_select_cpi },
        { "invert", 0, scale_display_invert, scale_select_invert },
        { "filter", 0, scale_display_filter, scale_select_filter },
    };

    if (config_setup("scale%d", SCALE_MAX, ARRAY_SIZE(var_list), var_list))
        scale_save();
}

void
scale_show(FILE* out, uint8_t scale_num)
{
    scale_t* scale_ptr;

    ASSERT(scale_num < SCALE_MAX);

#define FIELD_WIDTH     12

    scale_ptr = &scale[scale_num];
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Scale", scale_num);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Config Type", scale_type_str(scale_ptr->config_type));
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Active Type", scale_type_str(scale_ptr->active_type));
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Auto Weight", scale_ptr->auto_weight);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Invert", scale_ptr->invert ? "yes" : "no");
    fprintf(out, "%-*s : %" PRId32 "\n", FIELD_WIDTH, "Count", scale_ptr->count);
    fprintf(out, "%-*s : %" PRId32 "\n", FIELD_WIDTH, "CPI", scale_ptr->cpi);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Filter", scale_ptr->filter_enable ? "yes" : "no");
    if (scale_filter_active(scale_ptr))
        fprintf(out, "%-*s : %f\n", FIELD_WIDTH, "AMA Count", scale_ptr->filter_ama);
    if (scale_fast_capable(scale_ptr))
        fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Fast Mode", scale_ptr->fast_mode ? "yes" : "no");
}

void
scale_init(void)
{
    uint8_t     scale_num;
    scale_t*    scale_ptr;
    uint8_t     input_num;
    pin_t       in_pin;

#if PLATFORM_dro375
    // Configure switch latch as output
    GPIO_LOW(SWITCH_PORT, SWITCH_LATCH);
    GPIO_NOPULLUP(SWITCH_PORT, SWITCH_LATCH);
    GPIO_OUTPUT(SWITCH_PORT, SWITCH_LATCH);
#elif PLATFORM_dpu550
    // Configure switch latches as outputs
    GPIO_HIGH(SWITCH_PORT, SWITCH_LATCH0 | SWITCH_LATCH1);
    GPIO_NOPULLUP(SWITCH_PORT, SWITCH_LATCH0 | SWITCH_LATCH1);
    GPIO_OUTPUT(SWITCH_PORT, SWITCH_LATCH0 | SWITCH_LATCH1);

    // Disable pull-ups on scales 1-3 since they have external ones
    GPIO_NOPULLUP(SCALE_IN_PORT, SCALE0_IN0 | SCALE0_IN1 |
                                 SCALE1_IN0 | SCALE1_IN1 |
                                 SCALE2_IN0 | SCALE2_IN1);
#elif PLATFORM_dro550
    // Set all scale outputs high
    GPIO_HIGH(SCALE_OUT_PORT, SCALE_OUT_ALL);
    GPIO_NOPULLUP(SCALE_OUT_PORT, SCALE_OUT_ALL);
    GPIO_OUTPUT(SCALE_OUT_PORT, SCALE_OUT_ALL);
#endif

    for (scale_num = 0; scale_num < SCALE_MAX; scale_num++)
    {
        scale_ptr = &scale[scale_num];
        scale_ptr->scale_num = scale_num;
        scale_ptr->cpi = CPI_24BIT;
        scale_ptr->config_type = SCALE_TYPE_AUTO;
        scale_ptr->filter_enable = true;
        
        // Configure the input pin masks assigned to each scale
        for (input_num = 0; input_num < SCALE_IN_MAX; input_num++)
        {
            in_pin = scale_in_pin(scale_num, input_num);
            scale_ptr->in_pin[input_num] = in_pin;
            GPIO_INPUT(SCALE_IN_PORT, in_pin);
            GPIO_INTERRUPT(SCALE_IN_PORT, in_pin);
        }

        if (config_obj_read(CONFIG_OID_SCALE, scale_num))
        {
            config_var_read(VID_TYPE, 0, sizeof(scale_ptr->config_type), &scale_ptr->config_type);
            if (scale_ptr->config_type >= SCALE_TYPE_MAX)
                scale_ptr->config_type = SCALE_TYPE_OFF;

            config_var_read(VID_CPI, 0, sizeof(scale_ptr->cpi), &scale_ptr->cpi);
            config_var_read(VID_INVERT, 0, sizeof(scale_ptr->invert), &scale_ptr->invert);
            config_var_read(VID_FILTER, 0, sizeof(scale_ptr->filter_enable), &scale_ptr->filter_enable);
            config_obj_close();
        }

        // Set the active scale type
        scale_set_active(scale_ptr);
    }

    timer_start(&fast_timer, 15 * SEC_TICKS, TIMER_ONE_SHOT, scale_fast_timeout, NULL);

    irq_register(IRQ_SCALE_PROCESS, IRQ_PRIORITY_MEDIUM, IRQ_TYPE_EDGE, scale_isr); 
    irq_register(IRQ_SCALE_FAST, IRQ_PRIORITY_LOW, IRQ_TYPE_EDGE, scale_fast_isr);  
    irq_fast(IRQ_SCALE_IN, IRQ_TYPE_LEVEL);

    // Initialize the scale clock
    scale_clk_init();
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
scale_count_lua(lua_State* L)
{
    uint8_t scale_num = luaL_checkinteger(L, 1) - 1;
    if (scale_num >= SCALE_MAX)
        return luaL_error(L, "scale num invalid");
    lua_pushinteger(L, scale_get_count(scale_num));
    return 1;
}
 
static int
scale_cpi_lua(lua_State* L)
{
    uint8_t scale_num = luaL_checkinteger(L, 1) - 1;
    if (scale_num >= SCALE_MAX)
        return luaL_error(L, "scale num invalid");
    lua_pushinteger(L, scale_get_cpi(scale_num));
    return 1;
}
 
static int
scale_show_lua(lua_State* L)
{
    uint8_t scale_num = luaL_checkinteger(L, 1) - 1;
    if (scale_num >= SCALE_MAX)
        return luaL_error(L, "scale num invalid");
    scale_show(luash_out(L), scale_num);
    return 0;
}

const LUA_REG_TYPE scale_map_lua[] =
{
    { LSTRKEY("count"), LFUNCVAL(scale_count_lua) },
    { LSTRKEY("cpi"), LFUNCVAL(scale_cpi_lua) },
    { LSTRKEY("show"), LFUNCVAL(scale_show_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
scale_open_lua(lua_State* L)
{
    LREGISTER(L, "scale", scale_map_lua);
}

