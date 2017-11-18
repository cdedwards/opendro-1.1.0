///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// DPU-550 LED functions
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
#include "gpio.h"
#include "drolim.h"
#include "assert.h"
#include "trace.h"
#include "led.h"

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
led_platform_line(uint8_t line, uint8_t data)
{
    static pin_t gpio_map[LED_LINE_MAX] =
    {
        DISPLAY_LINE0,
        DISPLAY_LINE1,
        DISPLAY_LINE2,
    };

    ASSERT(line < LED_LINE_MAX);

    gpio_latch_data(data, gpio_map[line]);
}

void
led_platform_column(uint8_t column)
{
    ASSERT(column <= LED_COLUMN_NONE);

    GPIO_DATA_SET(DISPLAY_COLUMN_PORT, DISPLAY_COLUMN_ALL, (column << DISPLAY_COLUMN_OFFSET));
}

void
led_platform_init(void)
{
    // Set all latches high so there are no glitches
    GPIO_HIGH(
        DISPLAY_LINE_PORT,
        DISPLAY_LINE0 |
        DISPLAY_LINE1 |
        DISPLAY_LINE2);

    // Configure the line outputs
    GPIO_OUTPUT(
        DISPLAY_LINE_PORT,
        DISPLAY_LINE0 |
        DISPLAY_LINE1 |
        DISPLAY_LINE2);

    // Configure the column outputs
    GPIO_OUTPUT(
        DISPLAY_COLUMN_PORT,
        DISPLAY_LINE0 |
        DISPLAY_LINE1 |
        DISPLAY_LINE2 |
        DISPLAY_COLUMN0 |
        DISPLAY_COLUMN1 |
        DISPLAY_COLUMN2);

    // Disable line pull-ups
    GPIO_NOPULLUP(
        DISPLAY_LINE_PORT,
        DISPLAY_LINE0 |
        DISPLAY_LINE1 |
        DISPLAY_LINE2);

    // Disable column pull-ups
    GPIO_NOPULLUP(
        DISPLAY_COLUMN_PORT,
        DISPLAY_COLUMN0 |
        DISPLAY_COLUMN1 |
        DISPLAY_COLUMN2);
}
