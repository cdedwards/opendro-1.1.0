///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Keypad object
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
#include "keypad.h"
#include "irq.h"
#include "display.h"
#include "machine.h"
#include "fifo.h"
#include "gpio.h"
#include "drolim.h"
#include "timer.h"
#include "config.h"
#include "assert.h"
#include "trace.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

/// Size of key press FIFO
#define KEY_FIFO_SIZE           10

/// Size of key debounce filter
#define KEYPAD_DEBOUNCE         5

// Keypad sample period
#define KEYPAD_PERIOD           (SEC_TICKS / 50)

/// Length of keypad beep in microseconds
#define KEYPAD_BEEP_TIME        25000

// Keypad assertion logic
#if PLATFORM_dro375
#  define COLUMN_ASSERT         GPIO_LOW
#  define COLUMN_UNASSERT       GPIO_HIGH
#  define ROW_IS_ASSERTED       GPIO_IS_LOW
#elif PLATFORM_dpu550
#  define COLUMN_ASSERT         GPIO_HIGH
#  define COLUMN_UNASSERT       GPIO_LOW
#  define ROW_IS_ASSERTED       GPIO_IS_HIGH
#elif PLATFORM_dro550
#  define COLUMN_ASSERT         GPIO_LOW
#  define COLUMN_UNASSERT       GPIO_HIGH
#  define ROW_IS_ASSERTED       GPIO_IS_LOW
#endif

/// Keypad states
typedef enum keypad_state
{
    KEYPAD_STATE_INACTIVE,      ///< No key is pressed
    KEYPAD_STATE_DEBOUNCE,      ///< Key is being debounced
    KEYPAD_STATE_PRESSED,       ///< Key was pressed and awaiting release
} keypad_state_t;

/// Keypad FIFO to hold key presses
FIFO(fifo8, key_fifo, KEY_FIFO_SIZE);

/// Keypad debounce state inactive
static void keypad_inactive(void);

/// Keypad debounce state debounce
static void keypad_debounce(void);

/// Keypad debounce state pressed
static void keypad_pressed(void);

/// Keypad debounce processing function
typedef void (*keypad_fn_t)(void); 

/// Configuration variable IDs
typedef enum display_vid
{
    VID_KEY_BEEP,   ///< Beep on key press
    VID_MAX         ///< MUST be last!!!
} keypad_vid_t;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

static keypad_state_t state;    ///< Keypad debounce state
static uint8_t debounce;        ///< Debounce count
static uint8_t row;             ///< Row of key being debounced
static uint8_t column;          ///< Column of key being debounced
static bool key_beep;           ///< Beep on key press
#if PLATFORM_dro550
static timer_node_t sample_timer;   ///< Keypad sample timer
#endif

#if PLATFORM_dro375
/// Keypad row/column matrix to key mapping
static keypad_t matrix[KEYPAD_COLUMN_MAX][KEYPAD_ROW_MAX] =
{
    { KEYPAD_ABS_INCR, KEYPAD_7, KEYPAD_4, KEYPAD_1, KEYPAD_DECIMAL,    KEYPAD_ZERO2, KEYPAD_PRESET2, KEYPAD_ENTER },
    { KEYPAD_MM_INCH,  KEYPAD_8, KEYPAD_5, KEYPAD_2, KEYPAD_0,          KEYPAD_ZERO1, KEYPAD_PRESET1 },
    { KEYPAD_FUNC,     KEYPAD_9, KEYPAD_6, KEYPAD_3, KEYPAD_PLUS_MINUS, KEYPAD_ZERO0, KEYPAD_PRESET0,   KEYPAD_CLEAR },
};
#else
/// Keypad row/column matrix to key mapping
static keypad_t matrix[KEYPAD_COLUMN_MAX][KEYPAD_ROW_MAX] =
{
    { KEYPAD_PRESET0,   KEYPAD_UNKNOWN, KEYPAD_PRESET1, KEYPAD_UNKNOWN, KEYPAD_PRESET2 },
    { KEYPAD_ZERO0,     KEYPAD_CLEAR,   KEYPAD_ZERO1,   KEYPAD_ENTER,   KEYPAD_ZERO2 },
    { KEYPAD_ABS_INCR,  KEYPAD_7,       KEYPAD_4,       KEYPAD_1,       KEYPAD_DECIMAL },
    { KEYPAD_MM_INCH,   KEYPAD_8,       KEYPAD_5,       KEYPAD_2,       KEYPAD_0 },
    { KEYPAD_FUNC,      KEYPAD_9,       KEYPAD_6,       KEYPAD_3,       KEYPAD_PLUS_MINUS },
#if CONFIG_lcd
    { KEYPAD_ZERO4,     KEYPAD_PRESET4, KEYPAD_ZERO3,   KEYPAD_PRESET3, KEYPAD_UNKNOWN },
#endif
};
#endif // PLATFORM_dro375

/// Debounce finite state machine
static keypad_fn_t keypad_fsm[] =
{
    [KEYPAD_STATE_INACTIVE] = keypad_inactive,  
    [KEYPAD_STATE_DEBOUNCE] = keypad_debounce,  
    [KEYPAD_STATE_PRESSED]  = keypad_pressed,    
};

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static pin_t
keypad_row_pin(uint8_t row)
{
    static pin_t gpio_map[KEYPAD_ROW_MAX] =
    {
        KEYPAD_ROW0,
        KEYPAD_ROW1,
        KEYPAD_ROW2,
        KEYPAD_ROW3,
        KEYPAD_ROW4,
#if PLATFORM_dro375
        KEYPAD_ROW5,
        KEYPAD_ROW6,
        KEYPAD_ROW7,
#endif // PLATFORM_dro375
    };
    
    ASSERT(row < KEYPAD_ROW_MAX);

    return gpio_map[row];
}

static port_t
keypad_row_port(uint8_t row)
{
    return KEYPAD_ROW_PORT;
}

static pin_t
keypad_column_pin(uint8_t column)
{
    static pin_t gpio_map[KEYPAD_COLUMN_MAX] =
    {
        KEYPAD_COLUMN0,
        KEYPAD_COLUMN1,
        KEYPAD_COLUMN2,
#if !PLATFORM_dro375
        KEYPAD_COLUMN3,
        KEYPAD_COLUMN4,
#if CONFIG_lcd
        KEYPAD_LCD_COLUMN,
#endif
#endif // PLATFORM_dro375
    };

    ASSERT(column < KEYPAD_COLUMN_MAX);

    return gpio_map[column];
}

static port_t
keypad_column_port(uint8_t column)
{
#if CONFIG_lcd
    // The last column is the LCD column
    if (column == KEYPAD_COLUMN_MAX - 1)
        return KEYPAD_LCD_PORT;
    else
        return KEYPAD_COLUMN_PORT;
#else
    return KEYPAD_COLUMN_PORT;
#endif
}

static void
keypad_inactive(void)
{
    for (column = 0; column < KEYPAD_COLUMN_MAX; column++)
    {
        COLUMN_ASSERT(keypad_column_port(column), keypad_column_pin(column));

        timer_delay();
        
        for (row = 0; row < KEYPAD_ROW_MAX; row++)
        {
            if (ROW_IS_ASSERTED(keypad_row_port(row), keypad_row_pin(row)))
                break;
        }
        
        if (row != KEYPAD_ROW_MAX)
            break;

        COLUMN_UNASSERT(keypad_column_port(column), keypad_column_pin(column));
    }
    
    // If an active key is found, then try to debounce it
    if (column != KEYPAD_COLUMN_MAX)
    {
        TRACE_DEBUG("keypad C%d/R%d debounce\n", column, row);
        debounce = 1;
        state = KEYPAD_STATE_DEBOUNCE;
    }
}

static void
keypad_debounce(void)
{
    COLUMN_ASSERT(keypad_column_port(column), keypad_column_pin(column));

    timer_delay();

    if (ROW_IS_ASSERTED(keypad_row_port(row), keypad_row_pin(row)))
    {
        debounce++;
        if (debounce == KEYPAD_DEBOUNCE)
        {
            // Write the pressed key into the keypad fifo
            fifo8_write(&key_fifo, matrix[column][row]);

            TRACE_DEBUG("keypad C%d/R%d pressed\n", column, row);
            state = KEYPAD_STATE_PRESSED;
        }
    }
    else
    {
        debounce--;
        if (debounce == 0)
        {
            TRACE_DEBUG("keypad C%d/R%d inactive\n", column, row);
            state = KEYPAD_STATE_INACTIVE;
        }
    }

    COLUMN_UNASSERT(keypad_column_port(column), keypad_column_pin(column));
}

static void
keypad_pressed(void)
{
    COLUMN_ASSERT(keypad_column_port(column), keypad_column_pin(column));

    timer_delay();

    if (ROW_IS_ASSERTED(keypad_row_port(row), keypad_row_pin(row)))
    {
        if (debounce != KEYPAD_DEBOUNCE)
            debounce++;
    }
    else
    {
        debounce--;
        if (debounce == 0)
        {
            TRACE_DEBUG("keypad C%d/R%d inactive\n", column, row);
            state = KEYPAD_STATE_INACTIVE;
        }
    }

    COLUMN_UNASSERT(keypad_column_port(column), keypad_column_pin(column));
}

// Show the key beep
static void
keypad_show_key_beep(uint8_t  obj_inst,
                     uint8_t  var_inst)
{
    display_menu_printf(MENU_BOTTOM, "%s", key_beep ? "on" : "off");
}

// Toggle the key beep
static void
keypad_select_key_beep(uint8_t    obj_inst,
                       uint8_t    var_inst)
{
    if (key_beep)
        key_beep = false;
    else
        key_beep = true;
}

#if PLATFORM_dro550
static void
keypad_timeout(void* user)
{
    irq_trigger(IRQ_KEYPAD);
}

static void
keypad_isr(void)
{
    keypad_scan();
}
#endif

static void
keypad_set_blink(void *user)
{
    uint32_t* set_phase = user;
    (*set_phase)++;
    display_indicator_set(INDICATOR_SET, (*set_phase % 2));
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

bool
keypad_display_line(keypad_t    key,
                    uint8_t*    line_num)
{
    bool success = true;
    
    switch (key)
    {
        case KEYPAD_PRESET0:
        case KEYPAD_ZERO0:
            *line_num = 0;
            break;
        case KEYPAD_PRESET1:
        case KEYPAD_ZERO1:
            *line_num = 1;
            break;
        case KEYPAD_PRESET2:
        case KEYPAD_ZERO2:
            *line_num = 2;
            break;
#if CONFIG_lcd
        case KEYPAD_PRESET3:
        case KEYPAD_ZERO3:
            *line_num = 3;
            break;
        case KEYPAD_PRESET4:
        case KEYPAD_ZERO4:
            *line_num = 4;
            break;
#endif
        default:
            success = false;
            break;
    }
    
    return success;
}

bool
keypad_to_digit(keypad_t    key,
                uint8_t*    digit)
{
    bool success = true;
    
    switch (key)
    {
        case KEYPAD_0:
            *digit = 0;
            break;
        case KEYPAD_1:
            *digit = 1;
            break;
        case KEYPAD_2:
            *digit = 2;
            break;
        case KEYPAD_3:
            *digit = 3;
            break;
        case KEYPAD_4:
            *digit = 4;
            break;
        case KEYPAD_5:
            *digit = 5;
            break;
        case KEYPAD_6:
            *digit = 6;
            break;
        case KEYPAD_7:
            *digit = 7;
            break;
        case KEYPAD_8:
            *digit = 8;
            break;
        case KEYPAD_9:
            *digit = 9;
            break;
        default:
            success = false;
            break;
    }
    
    return success;
}

bool
keypad_get_number(menu_t    menu,
                  int32_t   min_value,
                  int32_t   max_value,
                  int32_t*  number)
{
    keypad_t    key;
    int32_t     value = 0;
    uint8_t     digit;
    bool        success;
    bool        empty = true;
    bool        start;
    char        string[MENU_WIDTH + 1];

    display_indicator_set(INDICATOR_SET, true);
    
    start = (*number >= min_value && *number <= max_value);
    
    while (1)
    {
        if (start)
        {
            snprintf(string, sizeof(string), "%*" PRId32, MENU_WIDTH, *number);
        }
        else if (empty)
        {
            if (snprintf(string, sizeof(string), "%" PRId32 "-%" PRId32, min_value, max_value) >= sizeof(string))
                snprintf(string, sizeof(string), "input");
        }
        else
        {
            snprintf(string, sizeof(string), "%*" PRId32, MENU_WIDTH, value);
        }
        display_menu_puts(menu, string);

        key = keypad_get_press();
        
        if (key == KEYPAD_ENTER)
        {
            if (start)
            {
                value = *number;
                success = true;
                break;
            }
            else if (!empty)
            {
                if (value >= min_value && value <= max_value)
                {
                    success = true;
                    break;
                }
                empty = true;
                value = 0;
            }
        }
        else if (key == KEYPAD_CLEAR)
        {
            if (empty || start)
            {
                success = false;
                break;
            }

            value /= 10;
            if (value == 0)
            {
                empty = true;
                if (*number >= min_value && *number <= max_value)
                    start = true;
            }
        }
        else if (key == KEYPAD_PLUS_MINUS)
        {
            value = -value;
            empty = false;
            start = false;
        }
        else if (keypad_to_digit(key, &digit))
        {
            value = (value * 10) + digit;
            empty = false;
            start = false;
        }
    }
    
    if (success)
    {
        *number = atoi(string);
        TRACE_INFO("input number = %d\n", *number);
    }

    display_indicator_set(INDICATOR_SET, false);
        
    return success;
}

bool
keypad_get_float(menu_t     menu,
                 double*    number,
                 keypad_t*  stop_key)
{
    keypad_t    key;
    uint32_t    whole_value = 0;
    uint8_t     whole_digits = 1;
    uint32_t    decimal_value = 0;
    uint8_t     decimal_digits = 0;
    uint32_t    fraction_value = 0;
    uint8_t     fraction_digits = 0;
    bool        fraction_set = false;
    bool        decimal_set = false;
    bool        negative_set = false;
    uint8_t     digit;
    char        string[MENU_WIDTH + 3];
    bool        valid = false;
    bool        success = false;
    bool        incr_mode = false;
    uint32_t    set_phase;
    timer_node_t set_timer;
    double      value;
    
    // If there isn't a stop key return then we turn on the SET
    // indicator and display the value.  Otherwise, we wait until
    // a numeric, sign, or decimal key is pressed.
    if (!stop_key)
    {
        display_indicator_set(INDICATOR_SET, true);
        display_menu_printf(menu, "%*f", MENU_WIDTH, *number);
    }
    
    while (1)
    {
        // Wait for a key press
        key = keypad_get_press();
        
        // Stop on the ENTER key
        if (key == KEYPAD_ENTER)
        {
            success = true;
            break;
        }
        // On the CLEAR key, we delete the last entered number or decimal
        else if (key == KEYPAD_CLEAR)
        {
            // If there is a fraction, then delete a fraction digit
            if (fraction_set)
            {
                if (fraction_digits == 0)
                {
                    fraction_set = false;
                }
                else
                {
                    fraction_value /= 10;
                    fraction_digits--;
                }   
            }
            // Else if there is a decimal, then delete a decimal digit
            else
            {
                if (decimal_set)
                {
                    if (decimal_digits == 0)
                    {
                        decimal_set = false;
                    }
                    else
                    {
                        decimal_value /= 10;
                        decimal_digits--;
                    }   
                }
                // Else there is no decimal so delete a whole digit
                else
                {
                    // Delete the whole digit
                    if (whole_digits > 1)
                    {
                        whole_value /= 10;
                        if (whole_value > 0)
                            whole_digits--;
                    }
                    else
                    {
                        if (stop_key || !valid)
                            break;

                        whole_value = 0;
                        valid = false;
                    }
                }
            }
        }
        // The PLUS/MINUS key negates the number
        else if (key == KEYPAD_PLUS_MINUS)
        {
            valid = true;
            if (negative_set)
                negative_set = false;
            else
                negative_set = true;
        }
        // Handle the DECIMAL point
        else if (key == KEYPAD_DECIMAL)
        {
            valid = true;
            // dp pressed, if already in decimal mode AND we've entered some digits then set fraction mode
            if (decimal_set && decimal_digits)
                fraction_set = true;
            else
                decimal_set = true;
        }
        // Handle the digits
        else if (keypad_to_digit(key, &digit))
        {
            valid = true;
            // If there is a fraction, then add a fractional value to the end
            if (fraction_set)
            {
                if (fraction_digits < (MENU_WIDTH - (whole_digits + decimal_digits)))
                {
                    fraction_value *= 10;
                    fraction_value += digit;
                    fraction_digits++;
                }
            }
            else
            {
                // Else If there is a decimal, then add a decimal value on the end
                if (decimal_set)
                {
                    if (decimal_digits < (MENU_WIDTH - whole_digits))
                    {
                        decimal_value *= 10;
                        decimal_value += digit;
                        decimal_digits++;
                    }
                }
                // Else there is no decimal so shift in a whole value
                else
                {
                    if (whole_digits < MENU_WIDTH)
                    {
                        whole_value *= 10;
                        whole_value += digit;
                        if (whole_value >= 10)
                            whole_digits++;
                    }   
                }
            }
        }
        // If there is a stop key return, then fill it in and exit the loop
        else if (stop_key)
        {
            *stop_key = key;
            success = true;
            break;
        }
        // ABS/INCR changes to incremental mode
        else if (key == KEYPAD_ABS_INCR)
        {
            incr_mode = (incr_mode ? false : true);
            if (incr_mode)
            {
                set_phase = 0;
                display_indicator_set(INDICATOR_SET, false);
                timer_start(&set_timer, SEC_TICKS / 4, TIMER_RECURRING, keypad_set_blink, &set_phase);
            }
            else
            {
                timer_stop(&set_timer);
                display_indicator_set(INDICATOR_SET, true);
            }
        }
        
        // Turn the SET indicator on if there is a stop key
        if (stop_key)
            display_indicator_set(INDICATOR_SET, true);

        if (valid)
        {
            if (fraction_set)
            {
                snprintf(string, sizeof(string),
                    "%*s%*"PRIu32".%0*"PRIu32".%0*"PRIu32,
                    MENU_WIDTH - whole_digits - decimal_digits - fraction_digits,
                    negative_set ? "-" : "",
                    whole_digits, whole_value,
                    decimal_digits, decimal_value,
                    fraction_digits, fraction_value);
            }    
            else
            {
                snprintf(string, sizeof(string),
                    decimal_set ?  "%*s%*"PRIu32".%0*"PRIu32 : "%*s%*"PRIu32,
                    MENU_WIDTH - whole_digits - decimal_digits,
                    negative_set ? "-" : "",
                    whole_digits, whole_value,
                    decimal_digits, decimal_value);
            }
        }
        else
        {
            snprintf(string, sizeof(string), "%*f", MENU_WIDTH, *number);
        }
        
        display_menu_puts(menu, string);
    }
    
    // If the numeric value is valid then return it
    if (valid && success)
    {
        // If we've entered a fraction then simply calculate final value
        if (fraction_set)
        {
            // Can't have zero divisor
            if (fraction_value)
            {
                value = (double) whole_value + ((double) decimal_value / (double) fraction_value);
                if (negative_set)
                    value = -value;
                    
                if (incr_mode)
                    *number += value;
                else
                    *number = value;
            }
            else
            {
                valid = false;
            }
        }
        // Else extract it from string
        else
        {
            value = atof(string);
            if (incr_mode)
                *number += value;
            else
                *number = value;
        }
        TRACE_INFO("input float = %f\n", *number);
    }

    // Stop the set blink timer
    if (incr_mode)
        timer_stop(&set_timer);
    
    // Turn off the SET indicator
    display_indicator_set(INDICATOR_SET, false);
        
    return success;
}

bool
keypad_to_nav(keypad_t    key,
              menu_t*     menu,
              menu_nav_t* nav)
{
    bool success = true;

    switch (key)
    {
    case KEYPAD_PREV_TOP:
        *menu = MENU_TOP;
        *nav = MENU_NAV_PREV;
        break;
    case KEYPAD_NEXT_TOP:
        *menu = MENU_TOP;
        *nav = MENU_NAV_NEXT;
        break;
    case KEYPAD_PREV_MIDDLE:
        *menu = MENU_MIDDLE;
        *nav = MENU_NAV_PREV;
        break;
    case KEYPAD_NEXT_MIDDLE:
        *menu = MENU_MIDDLE;
        *nav = MENU_NAV_NEXT;
        break;
    case KEYPAD_PREV_BOTTOM:
        *menu = MENU_BOTTOM;
        *nav = MENU_NAV_PREV;
        break;
    case KEYPAD_NEXT_BOTTOM:
        *menu = MENU_BOTTOM;
        *nav = MENU_NAV_NEXT;
        break;
    default:
        success = false;
        break;
    }

    return success;
}

bool
keypad_get_selection(menu_t         menu,
                     uint32_t*      selection,
                     uint32_t       max,
                     const char*    strings[])
{
    int32_t     index = 0;
    keypad_t    key;
    menu_t      line;
    menu_nav_t  nav;
    bool        success = true;

    ASSERT(selection);
    ASSERT(max > 0);
    ASSERT(strings);

    if (*selection >= 0 && *selection < max)
        index = *selection;
        
    do
    {
        ASSERT(strings[index]);
        display_menu_puts(menu, strings[index]);

        // Wait for a key press
        key = keypad_get_press();

        // Handle navigation keys
        if (keypad_to_nav(key, &line, &nav))
        {
            if (line == menu)
            {
                if (nav == MENU_NAV_NEXT)
                {
                    index++;
                    if (index == max)
                        index = 0;
                }
                else if (nav == MENU_NAV_PREV)
                {
                    index--;
                    if (index < 0)
                        index = max - 1;;
                }
            }
        }
        else if (key == KEYPAD_CLEAR)
        {
            success = false;
            break;
        }
    } while (key != KEYPAD_ENTER);

    if (success)
        *selection = index;

    return success;
}

void
keypad_scan(void)
{
#if PLATFORM_dro375
    // On the DRO-375, the keypad row pins are shared with
    // the data bus so configure the row pins as inputs
    GPIO_INPUT(KEYPAD_ROW_PORT, KEYPAD_ALL_ROWS);
    COLUMN_UNASSERT(KEYPAD_COLUMN_PORT, KEYPAD_ALL_COLUMNS)
#elif PLATFORM_dpu550
    // On the DPU-550, the keypad row pins are shared with
    // the data bus so configure the row pins as inputs
    GPIO_INPUT(KEYPAD_ROW_PORT, KEYPAD_ALL_ROWS);
    COLUMN_UNASSERT(KEYPAD_COLUMN_PORT, KEYPAD_ALL_COLUMNS)
#endif

    keypad_fsm[state]();

#if PLATFORM_dro375
    // Configure the keypad row pins back as outputs
    GPIO_OUTPUT(KEYPAD_ROW_PORT, KEYPAD_ALL_ROWS);
#elif PLATFORM_dpu550
    // Configure the keypad row pins back as outputs
    GPIO_OUTPUT(KEYPAD_ROW_PORT, KEYPAD_ALL_ROWS);
#endif
}

bool
keypad_empty(void)
{
    return (fifo8_empty(&key_fifo));
}

void
keypad_flush(void)
{
    while (!fifo8_empty(&key_fifo))
        fifo8_read(&key_fifo);
}

keypad_t
keypad_get_press(void)
{
    uint32_t key;
    
    while (keypad_empty())
        machine_idle(false);
    machine_busy();
    
    key = fifo8_read(&key_fifo);
    if (key_beep)
    {
        machine_beeper_on();
        timer_sleep(KEYPAD_BEEP_TIME);
        machine_beeper_off();
    }
    TRACE_INFO("keypad press %s\n", keypad_to_string(key));
    
    return key; 
}

keypad_t
keypad_peek_press(void)
{
    uint32_t key;
    
    while (keypad_empty())
        machine_idle(false);
    machine_busy();

    key = fifo8_peek(&key_fifo);
    TRACE_INFO("keypad peek %s\n", keypad_to_string(key));
    
    return key; 
}

void
keypad_add_press(keypad_t key)
{
    fifo8_write(&key_fifo, key);
}

const char *
keypad_to_string(keypad_t key)
{
    static const char *key_names[KEYPAD_MAX] =
    {
        [KEYPAD_0]          = "0",
        [KEYPAD_1]          = "1",
        [KEYPAD_2]          = "2",
        [KEYPAD_3]          = "3",
        [KEYPAD_4]          = "4",
        [KEYPAD_5]          = "5",
        [KEYPAD_6]          = "6",
        [KEYPAD_7]          = "7",
        [KEYPAD_8]          = "8",
        [KEYPAD_9]          = "9",
        [KEYPAD_PLUS_MINUS] = "+-",
        [KEYPAD_DECIMAL]    = ".",
        [KEYPAD_FUNC]       = "func",
        [KEYPAD_MM_INCH]    = "mminch",
        [KEYPAD_ABS_INCR]   = "absincr",
        [KEYPAD_CLEAR]      = "clear",
        [KEYPAD_ENTER]      = "enter",
        [KEYPAD_ZERO0]      = "zero1",
        [KEYPAD_PRESET0]    = "preset1",
        [KEYPAD_ZERO1]      = "zero2",
        [KEYPAD_PRESET1]    = "preset2",
        [KEYPAD_ZERO2]      = "zero3",
        [KEYPAD_PRESET2]    = "preset3",
#if CONFIG_lcd
        [KEYPAD_ZERO3]      = "zero4",
        [KEYPAD_PRESET3]    = "preset4",
        [KEYPAD_ZERO4]      = "zero5",
        [KEYPAD_PRESET4]    = "preset5",
#endif
        [KEYPAD_UNKNOWN]    = "unknown",
    };

    if (key >= KEYPAD_MAX)
        return NULL;

    return key_names[key];
}

void
keypad_save(void)
{
    if (config_obj_write(CONFIG_OID_KEYPAD, 0))
    {
        config_var_write(VID_KEY_BEEP, 0, sizeof(key_beep), &key_beep);
        config_obj_close();
    }
}

void
keypad_setup(void)
{
    static const var_list_t var_list[] = {
        { "beep", 0, keypad_show_key_beep, keypad_select_key_beep },
    };

    if (config_setup("keypad", 0, ARRAY_SIZE(var_list), var_list))
        keypad_save();
}

void
keypad_init(void)
{
    fifo8_init(&key_fifo, KEY_FIFO_SIZE);
    state = KEYPAD_STATE_INACTIVE;

    if (config_obj_read(CONFIG_OID_KEYPAD, 0))
    {
        config_var_read(VID_KEY_BEEP, 0, sizeof(key_beep), &key_beep);
        config_obj_close();
    }

#if CONFIG_lcd
    // Configure the LCD column pin as an open-drain output
    GPIO_OPENDRAIN(KEYPAD_LCD_PORT, KEYPAD_LCD_COLUMN);
    COLUMN_UNASSERT(KEYPAD_LCD_PORT, KEYPAD_LCD_COLUMN);
    GPIO_OUTPUT(KEYPAD_LCD_PORT, KEYPAD_LCD_COLUMN);
#endif

#if PLATFORM_dro550
    // Configure the row pins as inputs
    GPIO_INPUT(KEYPAD_ROW_PORT, KEYPAD_ALL_ROWS);

    // Configure the column pins as open-drain outputs
    GPIO_OPENDRAIN(KEYPAD_COLUMN_PORT, KEYPAD_ALL_COLUMNS);
    COLUMN_UNASSERT(KEYPAD_COLUMN_PORT, KEYPAD_ALL_COLUMNS);
    GPIO_OUTPUT(KEYPAD_COLUMN_PORT, KEYPAD_ALL_COLUMNS);

    irq_register(IRQ_KEYPAD, IRQ_PRIORITY_LOW, IRQ_TYPE_EDGE, keypad_isr);

    timer_start(&sample_timer, KEYPAD_PERIOD, TIMER_RECURRING, (void (*)(void*)) keypad_timeout, NULL);
#endif

#if PLATFORM_dro375
    // Configure the column pins as open-drain outputs
    GPIO_OPENDRAIN(KEYPAD_COLUMN_PORT, KEYPAD_ALL_COLUMNS);
    COLUMN_UNASSERT(KEYPAD_COLUMN_PORT, KEYPAD_ALL_COLUMNS);
    GPIO_OUTPUT(KEYPAD_COLUMN_PORT, KEYPAD_ALL_COLUMNS);
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
#include <string.h>

static int
keypad_press_lua(lua_State* L)
{
    const char* name;
    keypad_t    key;
    
    name = luaL_checkstring(L, 1);
    
    for (key = 0; key < KEYPAD_MAX; key++)
        if (strcasecmp(name, keypad_to_string(key)) == 0)
            break;

    if (key == KEYPAD_MAX)
        return luaL_error(L, "invalid keypad key");

    keypad_add_press(key);
    return 0;
}

static int
keypad_get_lua(lua_State* L)
{
    keypad_t    key;
    
    key = keypad_get_press();
    lua_pushstring(L, keypad_to_string(key));
    
    return 1;
}

static int
keypad_wait_lua(lua_State* L)
{
    while (keypad_empty())
        machine_idle(true);
    machine_busy();

    return 0;
}

static int
keypad_float_lua(lua_State* L)
{
    menu_t  menu;
    double number = 0.0;
    
    menu = luaL_checkinteger(L, 1) - 1;
    if (menu >= MENU_MAX)
        return luaL_error(L, "invalid menu position");


    if (!keypad_get_float(menu, &number, NULL))
        return 0;

    lua_pushnumber(L, number);
    
    return 1;
}

static int
keypad_number_lua(lua_State* L)
{
    menu_t  menu;
    int32_t number;
    int32_t min;
    int32_t max;
    
    menu = luaL_checkinteger(L, 1) - 1;
    if (menu >= MENU_MAX)
        return luaL_error(L, "invalid menu position");

    min = luaL_checkinteger(L, 2);
    max = luaL_checkinteger(L, 3);
    if (min > max)
        return luaL_error(L, "min larger than max");
    
    number = INT_MAX;
    if (!keypad_get_number(menu, min, max, &number))
        return 0;

    lua_pushinteger(L, number);
    
    return 1;
}

static int
keypad_select_lua(lua_State* L)
{
    menu_t  menu;
    uint32_t select = 0;
    uint32_t max;
    const char* strings[10];
    
    menu = luaL_checkinteger(L, 1) - 1;
    if (menu >= MENU_MAX)
        return luaL_error(L, "invalid menu position");

    max = 0;
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_pushnil(L);
    while (lua_next(L, 2) != 0 &&
           max < ARRAY_SIZE(strings))
    {
        strings[max++] = lua_tostring(L, -1);
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    if (max == 0 || !keypad_get_selection(menu, &select, max, strings))
        return 0;

    lua_pushinteger(L, select + 1);
    
    return 1;
}
                     
const LUA_REG_TYPE keypad_map_lua[] =
{
    { LSTRKEY("press"), LFUNCVAL(keypad_press_lua) },
    { LSTRKEY("get"), LFUNCVAL(keypad_get_lua) },
    { LSTRKEY("wait"), LFUNCVAL(keypad_wait_lua) },
    { LSTRKEY("float"), LFUNCVAL(keypad_float_lua) },
    { LSTRKEY("number"), LFUNCVAL(keypad_number_lua) },
    { LSTRKEY("select"), LFUNCVAL(keypad_select_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
keypad_open_lua(lua_State* L)
{
    LREGISTER(L, "keypad", keypad_map_lua);
}
