///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Multiplexed seven segment LED display object
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
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>
#include "display.h"
#include "led.h"
#include "gpio.h"
#include "drolim.h"
#include "keypad.h"
#include "function.h"
#include "config.h"
#include "timer.h"
#include "assert.h"
#include "trace.h"
#include "machine.h"
#include "scale.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

/// LED column the indicators are on
#define LED_COLUMN_INDICATORS       (LED_COLUMN_MAX)

/// Maximum LED intensity value
#define LED_INTENSITY_MAX           5

/// Special display characters 
#define DISPLAY_MINUS       0x40
#define DISPLAY_UNDERSCORE  0x08
#define DISPLAY_DECIMAL     0x80

/// Configuration variable IDs
typedef enum led_vid
{
    VID_INTENSITY,  ///< Display intensity
    VID_MAX         ///< MUST be last!!!
} led_vid_t;

/// Line state
typedef enum line_state
{
    LINE_STATE_ON,
    LINE_STATE_OFF,
} line_state_t;

/// Indicator state
typedef enum ind_state
{
    IND_STATE_NORMAL,
    IND_STATE_OFF,
} ind_state_t;

/// Menu state
typedef enum menu_state
{
    MENU_STATE_HIDE,
    MENU_STATE_DISPLAY,
} menu_state_t;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

/// Map ASCII values 0x30-0x5A to seven segment display values.
/// Bit positions of the seven segment display:
///   |-0-|
///   5   1
///   |-6-|
///   4   2
///   |-3-| .7
static const uint8_t segment_map[] =
{
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x3d,
    0x74, 0x06, 0x1e, 0x74, 0x38, 0x27, 0x37, 0x5c,
    0x73, 0x67, 0x50, 0x6d, 0x78, 0x3e, 0x3e, 0x1e,
    0x76, 0x6e, 0x5b
};

/// Buffer containing the reading text
static char reading_text[LED_LINE_MAX][LED_COLUMN_MAX + 3]; // Add 2 decimals and nil

/// Buffer containing the menu text
static char menu_text[MENU_MAX][LED_COLUMN_MAX + 3]; // Add 2 decimals and nil

/// Buffer containing the seven LED segment values
static uint8_t segments[LED_LINE_MAX][LED_COLUMN_MAX];

/// Binary indicators state
static uint8_t indicators;

/// Display intensity (0 .. LED_INTENSITY_MAX)
static uint8_t intensity;

/// Timer to refresh the LED display
static timer_node_t refresh_timer;

/// State of each line
static line_state_t line_state[LED_LINE_MAX];

/// Indicator state
static ind_state_t ind_state;

/// Menu state
static menu_state_t menu_state;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

// Handle the LED display refresh.  This function is called periodically via a
// recurring timer.
static void
led_refresh(void* user)
{
    static uint8_t  pulse_width = 0;
    static uint8_t  column = 0;
    uint8_t         line;

#if PLATFORM_dro375
    // For the DRO-375, scan the keypad and check the scale fast sequence 
    // at the start of each cycle since they share the display column bus.
    if (column == 0 && pulse_width == 0)
    {
        scale_check_fast();
        keypad_scan();
    }
#elif PLATFORM_dpu550
    // For the DPU-550, scan the keypad at the start of
    // each cycle.
    if (column == 0 && pulse_width == 0)
    {
        // Move the display to the empty column
        led_platform_column(LED_COLUMN_NONE);
        
        keypad_scan();
    }
#endif
    
    // Handle the column if this is the beginning of the pulse  
    if (pulse_width == 0)
    {
        // Move to the empty column to prevent ghosting     
        led_platform_column(LED_COLUMN_NONE);

        if (column == LED_COLUMN_INDICATORS)
        {
            // Display the indicators for this column
            for (line = 0; line < LED_LINE_MAX; line++)
            {
                if (line == DISPLAY_LINE_IND &&
                    ind_state == IND_STATE_NORMAL)
                {
                    led_platform_line(line, indicators);
                }
                else
                {
                    led_platform_line(line, 0);
                }
            }
        }
        else
        {
            // Display the segments for this column
            for (line = 0; line < LED_LINE_MAX; line++)
            {
                if (line_state[line] == LINE_STATE_ON)
                    led_platform_line(line, segments[line][LED_COLUMN_MAX - 1 - column]);
                else
                    led_platform_line(line, 0);
            }
        }

        // Move to the current column
        led_platform_column(column);
        
        // Advance to the next column and handle wrap around
        column++;
        if (column == LED_COLUMN_NONE)
            column = 0;
        
    }
    // Clear the segments if intensity is reached 
    else if (pulse_width == intensity)
    {
        for (line = 0; line < LED_LINE_MAX; line++)
            led_platform_line(line, 0);
    }

    // Advance the pulse width and handle wrap around
    pulse_width++;
    if (pulse_width >= LED_INTENSITY_MAX)
        pulse_width = 0;
}

// Show the display intensity
static void
led_show_intensity(uint8_t  obj_inst,
                   uint8_t  var_inst)
{
    display_menu_printf(MENU_BOTTOM, "%d", intensity * (100 / LED_INTENSITY_MAX));
}

// Iterate through the display intensities
static void
led_select_intensity(uint8_t    obj_inst,
                     uint8_t    var_inst)
{
    intensity++;
    if (intensity > LED_INTENSITY_MAX)
        intensity = 1;
}

static void
led_puts(uint8_t      line,
         const char*  string)
{
    uint8_t column;
    uint8_t segment;
    char    c;

    ASSERT(line < LED_LINE_MAX);

    for (column = 0; column < LED_COLUMN_MAX; column++)
    {
        c = toupper(*string);

        // Stop if we reach the end of the string
        if (c == '\0')
            break;

        // Special handling for 'M' and 'W'
        if (c == 'M')
            segments[line][column++] = 0x33;
        else if (c == 'W')
            segments[line][column++] = 0x3c;

        if (column == LED_COLUMN_MAX)
            break;

        // Map the minus sign and alphanumeric characters to a segment value
        if (c == '-')
            segment = DISPLAY_MINUS;
        else if (c == '_')
            segment = DISPLAY_UNDERSCORE;
        else if (isalnum(c))
            segment = segment_map[c - '0'];
        else
            segment = 0;

        // Advance to the next character
        string++;

        // If the next character is a decimal, then set the decimal
        if (*string == '.')
        {
            segment |= DISPLAY_DECIMAL;
            string++;
        }

        segments[line][column] = segment;
    }

    // Blank any remaining segments
    while (column < LED_COLUMN_MAX)
    {
        segments[line][column] = 0;
        column++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
led_stop(void)
{
    led_platform_column(LED_COLUMN_NONE);
    timer_stop(&refresh_timer);
}

void
led_start(void)
{
    timer_start(&refresh_timer, 1, TIMER_RECURRING, led_refresh, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// D R I V E R   A P I
///////////////////////////////////////////////////////////////////////////////

static void
led_reading_set(uint8_t   line,
                double    number,
                uint8_t   precision,
                bool      mark)
{
    char string[LED_COLUMN_MAX + 4];  // Includes nil, two decimal points, and extra digit
    uint8_t pos;
    uint8_t width;

    ASSERT(line <  LED_LINE_MAX);

    // Save space for the mark
    pos = 1;

    if (precision == PRECISION_FRACTION)
    {
        int32_t     whole;
        int32_t     numer;
        int32_t     denom;
        char        frac[LED_COLUMN_MAX + 3]; // Includes nil and two decimal points

        machine_dec2frac(number, 64, &whole, &numer, &denom);
        if (whole && numer)
        {
            width = LED_COLUMN_MAX + 2;
            snprintf(frac, sizeof(frac), "%"PRId32".%"PRId32".%"PRId32, whole, numer, denom);
        }
        else if (numer)
        {
            width = LED_COLUMN_MAX + 1;
            snprintf(frac, sizeof(frac), "%"PRId32".%"PRId32, numer, denom);
        }
        else
        {
            width = LED_COLUMN_MAX;
            snprintf(frac, sizeof(frac), "%"PRId32, whole);
        }
        snprintf(&string[pos], sizeof(string) - pos, "%*s", width, frac);
    }
    else
    {
        // Calculate the display width
        width = LED_COLUMN_MAX + (precision % 2);

        // Add one to the width if there will be a decimal point
        if (precision)
            width++;

        // Convert the floating point number into a string
        snprintf(&string[pos], sizeof(string) - pos, "%*.*f",
                 width, (precision + 1) / 2, number);
        
        // If the last digit is >=5, then replace it with a decimal else remove it
        if (string[LED_COLUMN_MAX + 2] >= '5')
            string[LED_COLUMN_MAX + 2] = '.';
        else
            string[LED_COLUMN_MAX + 2] = '\0';
    }

    // If the mark is set, then insert a decimal after the first character
    if (mark)
    {
        pos--;
        string[pos] = string[pos + 1];
        string[pos + 1] = '.';
    }

    // Save a copy of the string to the reading buffer
    strlcpy(reading_text[line], &string[pos], sizeof(reading_text[line]));

    // Menu is implicitly hidden
    menu_state = MENU_STATE_HIDE;

    // Display the string
    led_puts(line, &string[pos]);
}

static const char *
led_reading_get(uint8_t line)
{
    ASSERT(line < LED_LINE_MAX);

    return reading_text[line];
}

static void
led_reading_on(uint8_t line)
{
    ASSERT(line < LED_LINE_MAX);

    line_state[line] = LINE_STATE_ON;
}

static void
led_reading_off(uint8_t line)
{
    ASSERT(line < LED_LINE_MAX);

    line_state[line] = LINE_STATE_OFF;
}

static void
led_label_set(uint8_t       line,
              const char*   format,
              va_list       args)
{
    // The LED doesn't have a label display so just return
}

static void
led_menu_puts(menu_t       menu,
              const char*  string)
{
    uint8_t line;
    
    ASSERT(menu < MENU_MAX);

    // Map the menu
    if (menu == MENU_TOP)
        line = 0;
    else if (menu == MENU_MIDDLE)
        line = 1;
    else
        line = 2;

    // Save a copy of the string to the menu buffer
    strlcpy(menu_text[menu], string, sizeof(menu_text[line]));

    // Menu is implicitly displayed
    menu_state = MENU_STATE_DISPLAY;

    // Display the string
    led_puts(line, string);
}

static void
led_menu_printf(menu_t       menu,
                const char*  format,
                va_list      args)
{
    char string[LED_COLUMN_MAX + 3]; // Include nil and 2 decimal points

    ASSERT(menu < MENU_MAX);

    vsnprintf(string, sizeof(string), format, args);
    led_menu_puts(menu, string);
}

static const char*
led_menu_get(menu_t menu)
{
    ASSERT(menu < MENU_MAX);

    return menu_text[menu];
}

static void
led_menu_clear(void)
{
    uint32_t line;
    uint32_t column;

    for (line = 0; line < LED_LINE_MAX; line++)
        for (column = 0; column < LED_COLUMN_MAX; column++)
            segments[line][column] = 0;
}

static bool
led_menu_state(void)
{
    return (menu_state == MENU_STATE_DISPLAY);
}

static void
led_status_set(status_t status,
                  va_list  args)
{
    // The LED doesn't have a status display so just return
}

static void
led_indicator_set(indicator_t   indicator,
                  bool          state)
{
    if (state)
        indicators |= (1 << indicator);
    else
        indicators &= ~(1 << indicator);
}

static bool
led_indicator_get(indicator_t   indicator)
{
    return (indicators & (1 << indicator));
}

static void
led_off(void)
{
    uint8_t line;

    for (line = 0; line < LED_LINE_MAX; line++)
        line_state[line] = LINE_STATE_OFF;
    ind_state = IND_STATE_OFF;
}

static void
led_on(void)
{
    uint8_t line;

    for (line = 0; line < LED_LINE_MAX; line++)
        line_state[line] = LINE_STATE_ON;
    ind_state = IND_STATE_NORMAL;
}

static void
led_save(void)
{
    if (config_obj_write(CONFIG_OID_DISPLAY, 0))
    {
        config_var_write(VID_INTENSITY, 0, sizeof(intensity), &intensity);
        config_obj_close();
    }   
}

static void
led_setup(void)
{
    static const var_list_t var_list[] = {
        { "intens", 0, led_show_intensity, led_select_intensity },
    };

    if (config_setup("disply", 0, ARRAY_SIZE(var_list), var_list))
        led_save();
}

static void
led_init(void)
{
    led_platform_init();

    intensity = LED_INTENSITY_MAX;

    if (config_obj_read(CONFIG_OID_DISPLAY, 0))
    {
        config_var_read(VID_INTENSITY, 0, sizeof(intensity), &intensity);
        config_obj_close();
    }

    led_start();
}

///////////////////////////////////////////////////////////////////////////////
// G L O B A L   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

const display_dev_t led_display_dev = {
    .reading_set    = led_reading_set,
    .reading_get    = led_reading_get,
    .reading_on     = led_reading_on,
    .reading_off    = led_reading_off,

    .label_set      = led_label_set,

    .menu_printf    = led_menu_printf,
    .menu_puts      = led_menu_puts,
    .menu_get       = led_menu_get,
    .menu_clear     = led_menu_clear,
    .menu_state     = led_menu_state,

    .status_set     = led_status_set,

    .indicator_set  = led_indicator_set,
    .indicator_get  = led_indicator_get,

    .off            = led_off,
    .on             = led_on,
    .save           = led_save,
    .setup          = led_setup,
    .init           = led_init,
};
