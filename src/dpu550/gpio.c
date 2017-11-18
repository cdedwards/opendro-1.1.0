///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Microprocessor GPIO pin abstraction
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
#include "types.h"
#include "board.h"
#include "interrupt.h"
#include "timer.h"

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
gpio_latch_data(uint8_t  data,
                pin_t    latch)
{
    GPIO_DATA_SET(DATA_PORT, DATA_ALL, (data << DATA_OFFSET));

    // Assert the latch line
    GPIO_HIGH(DATA_PORT, latch);
    timer_delay();
    GPIO_LOW(DATA_PORT, latch);
    timer_delay();
}

void
gpio_init(void)
{
    // Enable the PIOA clock
    AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, (1 << AT91C_ID_PIOA));

    // Configure the data bus as outputs
    GPIO_NOPULLUP(DATA_PORT, DATA_ALL);
    GPIO_OUTPUT(DATA_PORT, DATA_ALL);
}
