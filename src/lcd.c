///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// LCD display
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
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "board.h"
#include "assert.h"
#include "trace.h"
#include "timer.h"
#include "irq.h"
#include "lcd.h"
#include "lcd_dev.h"
#include "font6x8.h"
#include "font12x18.h"
#include "machine.h"
#include "function.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

/// LCD display refresh period
#define REFRESH_PERIOD      (SEC_TICKS / 8)

/// Line state
typedef enum line_state
{
    LINE_STATE_ON,
    LINE_STATE_OFF,
} line_state_t;

/// X,Y coordinate
typedef struct coord
{
    int32_t x;
    int32_t y;
} coord_t;

#define STATUS_FIELD_MAX    6

#define STATUS_FIELD_FONT   &font6x8
#define STATUS_FIELD_PUTS(status, str) \
    lcd_puts(status_coord[status].x, status_coord[status].y, STATUS_FIELD_FONT, str)

#define STATUS_FIELD_WIDTH  8

#define READING_FONT        &font12x18
#define READING_FONT_WIDTH  12
#define READING_FONT_HEIGHT 18
#define READING_PUTS(line, str) \
    lcd_puts(line_coord[line].x, line_coord[line].y, READING_FONT, str)

#define LABEL_WIDTH         3

#define LABEL_BIG_FONT      &font12x18
#define LABEL_SMALL_FONT    &font6x8
#define LABEL_PUTS(line, str) { \
    lcd_putc(label_coord[line][0].x, label_coord[line][0].y, LABEL_BIG_FONT, str[0]);   \
    lcd_putc(label_coord[line][1].x, label_coord[line][1].y, LABEL_SMALL_FONT, str[1]); \
    lcd_putc(label_coord[line][2].x, label_coord[line][2].y, LABEL_SMALL_FONT, str[2]); }

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

/// LCD frambe buffer
static uint8_t      lcd_buffer[LCD_DISPLAY_WIDTH * LCD_DISPLAY_HEIGHT / BITS_PER_BYTE];

/// LCD display refresh timer
static timer_node_t lcd_timer;

/// State of each line
static line_state_t line_state[LCD_LINE_MAX];

/// Saved line text
static char         line_text[LCD_LINE_MAX][LCD_DIGITS_MAX +1];

/// X,Y coordinates of the status fields
static const coord_t status_coord[STATUS_FIELD_MAX] = {
    { 1, 2 }, { 45, 2 }, { 83, 2 }, { 1, 12 }, { 45, 12 }, { 83, 12 }
};

/// X,Y coordinates of the lines
static const coord_t line_coord[LCD_LINE_MAX] = {
    { 2, 24 }, { 2, 46 }
};

/// X,Y coordinates of the label characters
static const coord_t label_coord[LCD_LINE_MAX][LABEL_WIDTH] = {
    { { 104, 24 }, { 116, 34 }, { 122, 34 }, },
    { { 104, 46 }, { 116, 56 }, { 122, 56 }, },
};

/// X,Y coordinates of the line marks
static const coord_t mark_coord[LCD_LINE_MAX][2] = {
    { { 0, 30 }, { 2, 35 }, },
    { { 0, 52 }, { 2, 57 }, },
};

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static inline void
lcd_setpixel(int32_t x,
             int32_t y,
             uint8_t c)
{
    int32_t byte;
    int32_t bit;
    uint8_t mask;

    ASSERT(x >= 0 || x < LCD_DISPLAY_WIDTH);
    ASSERT(y >= 0 || y < LCD_DISPLAY_HEIGHT);

    bit = y * LCD_DISPLAY_WIDTH + x;
    byte = bit / BITS_PER_BYTE;
    mask = 0x80 >> (bit % BITS_PER_BYTE);

    if (c)
        lcd_buffer[byte] |= mask;
    else
        lcd_buffer[byte] &= ~mask;
}

static void
lcd_bitblt(int32_t          x,
           int32_t          y,
           int32_t          width,
           int32_t          height,
           int32_t          src_x,
           int32_t          src_y,
           const uint8_t*   src_buffer,
           int32_t          src_xdim)
{
    uint8_t*        lcd_ptr;
    int32_t         lcd_bit;
    const uint8_t*  src_ptr;
    int32_t         src_bit;
    int32_t         pixels;
    int32_t         xfer_size;
    int32_t         shift;
    uint8_t         mask;

    while (height-- > 0)
    {
        // Initialize byte pointers
        lcd_ptr = lcd_buffer;
        src_ptr = src_buffer;

        // Calculate the bit offsets
        lcd_bit = LCD_DISPLAY_WIDTH * y + x;
        src_bit = src_xdim * src_y + src_x;

        // Number of pixels in the line
        pixels = width;

        // Continue while there are pixels to transfer
        while (pixels > 0)
        {
            // Adjust the byte pointers
            lcd_ptr += (lcd_bit / BITS_PER_BYTE);
            src_ptr += (src_bit / BITS_PER_BYTE);

            // Normalize the bit offsets
            lcd_bit %= BITS_PER_BYTE;
            src_bit %= BITS_PER_BYTE;

            // Check the shift direction
            if (lcd_bit < src_bit)
            {
                // Shifting left
                xfer_size = MIN(pixels, BITS_PER_BYTE - src_bit);
                shift = src_bit - lcd_bit;
                mask = ((1 << xfer_size) - 1) << (BITS_PER_BYTE - lcd_bit - xfer_size);
                *lcd_ptr = (*lcd_ptr & ~mask) | ((*src_ptr << shift) & mask);
            }
            else
            {
                // Shifting right
                xfer_size = MIN(pixels, BITS_PER_BYTE - lcd_bit);
                shift = lcd_bit - src_bit;
                mask = ((1 << xfer_size) - 1) << (BITS_PER_BYTE - lcd_bit - xfer_size);
                *lcd_ptr = (*lcd_ptr & ~mask) | ((*src_ptr >> shift) & mask);
            }

            // Adjust by the bit transfer size
            src_bit += xfer_size;
            lcd_bit += xfer_size;
            pixels -= xfer_size;
        }

        // Move to the next line
        y++;
        src_y++;
    }
}

static void
lcd_hline(int32_t x1,
          int32_t y1,
          int32_t x2,
          uint8_t c)
{
    // TODO: Make this more efficient by setting bytes instead of pixels
    while (x1 < x2)
    {
        lcd_setpixel(x1, y1, c);
        x1++;
    }
}

static void
lcd_vline(int32_t x1,
          int32_t y1,
          int32_t y2,
          uint8_t c)
{
    while (y1 < y2)
    {
        lcd_setpixel(x1, y1, c);
        y1++;
    }
}

static void
lcd_rect(int32_t x1,
         int32_t y1,
         int32_t x2,
         int32_t y2,
         uint8_t c)
{
    lcd_hline(x1, y1, x2, c);
    lcd_hline(x1, y2 - 1, x2, c);
    lcd_vline(x1, y1, y2, c);
    lcd_vline(x2 - 1, y1, y2, c);
}

static void
lcd_fill(int32_t x1,
         int32_t y1,
         int32_t x2,
         int32_t y2,
         uint8_t c)
{
    while (y1 < y2)
    {
        lcd_hline(x1, y1, x2, c);
        y1++;
    }
}

static void
lcd_putc(int32_t        x,
         int32_t        y,
         const font_t*  font,
         char           c)
{
    int32_t     font_xdim;
    int32_t     font_x;
    int32_t     font_y;

    ASSERT(x < LCD_DISPLAY_WIDTH);
    ASSERT(y < LCD_DISPLAY_HEIGHT);

    // If character is a nil, then convert it to a space
    if (c == '\0')
        c = ' ';

    ASSERT(c >= font->first && c <= font->last);

    // Zero index the character
    c -= font->first;

    // Calculate the font parameters
    font_xdim = font->bytes * BITS_PER_BYTE;
    font_x = font_xdim - font->width;
    font_y = c * font->height;

    // Bitblt the character into the LCD
    lcd_bitblt(x,
               y,
               font->width,
               font->height,
               font_x,
               font_y,
               font->data,
               font_xdim);
}

static void
lcd_puts(int32_t        x,
         int32_t        y,
         const font_t*  font,
         const char*    text)
{
    while (*text)
    {
        lcd_putc(x, y, font, *text++);
        x += font->width;
    }
}

static void
lcd_update(void)
{
    lcd_dev.update(lcd_buffer);
}

static void
lcd_timeout(void* user)
{
    irq_trigger(IRQ_LCD);
}

///////////////////////////////////////////////////////////////////////////////
// D R I V E R   A P I
///////////////////////////////////////////////////////////////////////////////

static void
lcd_reading_set(uint8_t   line,
                double    number,
                uint8_t   precision,
                bool      mark)
{
    char    str[LCD_DIGITS_MAX + 1];
    double  factor;

    ASSERT(line < LCD_LINE_MAX);

    if (precision == PRECISION_FRACTION)
    {
        int32_t     whole;
        int32_t     numer;
        int32_t     denom;
        char        frac[LCD_DIGITS_MAX + 1];

        machine_dec2frac(number, 64, &whole, &numer, &denom);
        if (whole && numer)
            snprintf(frac, sizeof(frac), "%"PRId32" %"PRId32"/%"PRId32, whole, numer, denom);
        else if (numer)
            snprintf(frac, sizeof(frac), "%"PRId32"/%"PRId32, numer, denom);
        else
            snprintf(frac, sizeof(frac), "%"PRId32, whole);
        snprintf(str, sizeof(str), "%*s", LCD_DIGITS_MAX, frac);
    }
    else
    {
        // Check if the precision has a half digit
        if (precision % 1)
        {
            // Round to the nearest half digit
            factor = 2.0 * pow(10.0, precision / 2);
            number = floor(factor * number + 0.5) / factor;
        }

        // Convert the floating point number into a string
        snprintf(str, sizeof(str), "%*.*f",
                 LCD_DIGITS_MAX, (precision + 1) / 2, number);
    }

    // Set mark symbol
    lcd_rect(mark_coord[line][0].x, mark_coord[line][0].y,
             mark_coord[line][1].x, mark_coord[line][1].y, mark ? 1 : 0);

    // Save a copy of the string to the line buffer
    strlcpy(line_text[line], &str[1], sizeof(line_text[line]));

    // Display the string if the line is on
    if (line_state[line] == LINE_STATE_ON)
        READING_PUTS(line, str);
}

static const char *
lcd_reading_get(uint8_t line)
{
    ASSERT(line < LCD_LINE_MAX);

    return line_text[line];
}

static void
lcd_reading_on(uint8_t line)
{
    ASSERT(line < LCD_LINE_MAX);
    if (line_state[line] == LINE_STATE_OFF)
    {
        READING_PUTS(line, line_text[line]);
        line_state[line] = LINE_STATE_ON;
    }
}

static void
lcd_reading_off(uint8_t line)
{
    ASSERT(line < LCD_LINE_MAX);
    if (line_state[line] == LINE_STATE_ON)
    {
        // Blank the text area
        lcd_fill(line_coord[line].x, line_coord[line].y,
                 line_coord[line].x + LCD_DIGITS_MAX * READING_FONT_WIDTH,
                 line_coord[line].y + READING_FONT_HEIGHT, 0);
        line_state[line] = LINE_STATE_OFF;
    }
}

static void
lcd_label_set(uint8_t       line,
              const char*   format,
              va_list       args)
{
    char    str[LABEL_WIDTH + 1] = { [0 ... LABEL_WIDTH] = '\0' };

    vsnprintf(str, sizeof(str), format, args);
    LABEL_PUTS(line, str);
}

static void
lcd_menu_printf(menu_t       menu,
                const char*  format,
                va_list      args)
{
    // No LCD menu for now
}

static void
lcd_menu_puts(menu_t       menu,
              const char*  text)
{
    // No LCD menu for now
}

static const char*
lcd_menu_get(menu_t menu)
{
    // No LCD menu for now
    return NULL;
}

static void
lcd_menu_clear(void)
{
    // No LCD menu for now
}

static void
lcd_status_set(status_t status,
               va_list  args)
{
    char        str[STATUS_FIELD_WIDTH];
    static char ws_str[STATUS_FIELD_WIDTH];

    ASSERT(status < STATUS_MAX);

    switch (status)
    {
    case STATUS_MACHINE_NUM:
        // Don't display
        break;
    case STATUS_MACHINE_TYPE:
    {
        machine_type_t type = va_arg(args, int);
        char *text;
        if (type == MACHINE_TYPE_MILL)
            text = "Mill";
        else if (type == MACHINE_TYPE_LATHE)
            text = "Lathe";
        else
            text = "Unknown";
        snprintf(str, sizeof(str), "%-7s", text);
        STATUS_FIELD_PUTS(0, str);
        break;
    }
    case STATUS_FUNC_STATE:
    {
        func_state_t state = va_arg(args, int);
        if (state == STATE_NORMAL) {
            // Restore the workspace display
            STATUS_FIELD_PUTS(2, ws_str);
            snprintf(str, sizeof(str), "Normal");
        } else if (state == STATE_BOLTHOLE) {
            uint8_t num = va_arg(args, int);
            snprintf(str, sizeof(str), "Blt %02d", num);
        } else if (state == STATE_GRID) {
            uint8_t num = va_arg(args, int);
            snprintf(str, sizeof(str), "Grd %02d", num);
        } else if (state == STATE_RADIUS) {
            uint8_t num = va_arg(args, int);
            snprintf(str, sizeof(str), "Rad %02d", num);
        } else if (state == STATE_CALC) {
            snprintf(str, sizeof(str), "Calc  ");
        } else {
            snprintf(str, sizeof(str), "Unknwn");
        }
        STATUS_FIELD_PUTS(1, str);
        break;
    }
    case STATUS_FUNC_STEP:
        vsnprintf(str, sizeof(str), "Step %02d", args);
        STATUS_FIELD_PUTS(2, str);
        break;
    case STATUS_WORKSPACE:
    {
        // Save the workspace display
        uint8_t wspace = va_arg(args, int);
        snprintf(ws_str, sizeof(ws_str), "WSpc %02d", wspace);
        STATUS_FIELD_PUTS(2, ws_str);
        break;
    }
    case STATUS_COMPOUND:
        vsnprintf(str, sizeof(str), "C %5.1f", args);
        STATUS_FIELD_PUTS(5, str);
        break;
    case STATUS_DIAMETER:
        break;
    case STATUS_TOOL_NUM:
    {
        uint8_t tool = va_arg(args, int);
        if (tool == 0)
            snprintf(str, sizeof(str), "No Tool");
        else
            snprintf(str, sizeof(str), "Tool %02d", tool);
        STATUS_FIELD_PUTS(3, str);
        break;
    }
    case STATUS_TOOL_OFFSET:
        vsnprintf(str, sizeof(str), "%6.4f", args);
        STATUS_FIELD_PUTS(4, str);
        break;
    case STATUS_TOOL_EDGE:
    {
        edge_t edge = va_arg(args, int);
        char *text;
        if (edge == EDGE_CENTER)
            text = "Center";
        else if (edge == EDGE_FRONT)
            text = "Front";
        else if (edge == EDGE_REAR)
            text = "Back";
        else if (edge == EDGE_LEFT)
            text = "Left";
        else if (edge == EDGE_RIGHT)
            text = "Right";
        else
            text = "Unknwn";
        snprintf(str, sizeof(str), "%-7s", text);
        STATUS_FIELD_PUTS(5, str);
        break;
    }
    default:
        ASSERT(0);
        break;
    }
}

static void
lcd_indicator_set(indicator_t   indicator,
                  bool          on)
{
    // No indicators
}

static bool
lcd_indicator_get(indicator_t indicator)
{
    // No indicators
    return false;
}

static void
lcd_off(void)
{
    lcd_dev.off();
}

/// \brief  Turn the display on if it was off before
static void
lcd_on(void)
{
    lcd_dev.on();
}

/// \brief  Save all display configuration to non-volatile memory
static void
lcd_save(void)
{
    // No configuration
}

static void
lcd_setup(void)
{
    // No configuration
}

static void
lcd_init(void)
{
    // Initialize the LCD device
    if (!lcd_dev.init()) {
        TRACE_ERROR("Unable to initialize LCD display\n");
        return;
    }

    // Draw status display frame
    lcd_rect(0, 0, LCD_DISPLAY_WIDTH, 21, 1);
    lcd_hline(0, 10, LCD_DISPLAY_WIDTH, 1);
    lcd_vline(44, 0, 21, 1);
    lcd_vline(82, 0, 21, 1);

    timer_start(&lcd_timer, REFRESH_PERIOD, TIMER_RECURRING, lcd_timeout, NULL);
    irq_register(IRQ_LCD, IRQ_PRIORITY_LOW, IRQ_TYPE_EDGE, lcd_update);
}

///////////////////////////////////////////////////////////////////////////////
// G L O B A L   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

const display_dev_t lcd_display_dev = {
    .reading_set    = lcd_reading_set,
    .reading_get    = lcd_reading_get,
    .reading_off    = lcd_reading_off,
    .reading_on     = lcd_reading_on,

    .label_set      = lcd_label_set,

    .menu_printf    = lcd_menu_printf,
    .menu_puts      = lcd_menu_puts,
    .menu_get       = lcd_menu_get,
    .menu_clear     = lcd_menu_clear,

    .status_set     = lcd_status_set,

    .indicator_set  = lcd_indicator_set,
    .indicator_get  = lcd_indicator_get,

    .off            = lcd_off,
    .on             = lcd_on,
    .save           = lcd_save,
    .setup          = lcd_setup,
    .init           = lcd_init,
};
