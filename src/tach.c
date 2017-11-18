///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Tachometer driver
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
#include "tach.h"
#include "board.h"
#include "drolim.h"
#include "gpio.h"
#include "assert.h"
#include "trace.h"

#if TACH_MAX > 2
#error "Tach driver only supports up to two inputs"
#endif

pin_t
tach_pin(uint8_t tach_num)
{
    static pin_t pin_map[TACH_MAX] =
    {
        TACH0_INPUT,
        TACH1_INPUT,
    };

    ASSERT(tach_num < TACH_MAX);

    return pin_map[tach_num];
}

double
tach_get_reading(uint8_t tach_num)
{
    AT91S_TC*   tc_ptr;
    double      rpm;
    uint32_t    status;
    uint32_t    period;
    
    ASSERT(tach_num < TACH_MAX);

#if PLATFORM_dpu550
    tc_ptr = (tach_num == 0) ? AT91C_BASE_TC1 : AT91C_BASE_TC0;
#else
    tc_ptr = (tach_num == 0) ? AT91C_BASE_TC0 : AT91C_BASE_TC1;
#endif

    // Grab the TC status
    status = tc_ptr->TC_SR;

    // Check for counter overflow
    if (status & AT91C_TC_COVFS)
        return 0.0;

    // Make sure the counter loaded
    if (!(status & AT91C_TC_LDRAS))
        return 0.0;

    // Get the counter value
    period = tc_ptr->TC_RA;
    if (period == 0)
        return 0.0;

    // Calculate and return the RPM
    rpm = (60.0 * MCLK / 1024.0) / period;
    
    return rpm;
}

void
tach_init(void)
{
    // Enable the TC0 & TC1 peripheral clocks
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_TC0) | (1 << AT91C_ID_TC1);
    
    // Assign the tach pins to the TC peripheral
#if PLATFORM_dpu550
    // TACH1 output is multiplexed with the scale clock in the machine object
    GPIO_PERIPH_B(TACH_PORT, TACH0_INPUT);
#elif PLATFORM_dro375
    GPIO_PERIPH_B(TACH_PORT, TACH0_INPUT | TACH1_INPUT);
#elif PLATFORM_dro550
    GPIO_PERIPH_A(TACH_PORT, TACH0_INPUT | TACH1_INPUT);
#endif
    
    // Set capture mode, load RA on falling edge, MCLK / 1024 clock
    AT91C_BASE_TC0->TC_CMR = AT91C_TC_ETRGEDG_FALLING | AT91C_TC_ABETRG | AT91C_TC_CLKS_TIMER_DIV5_CLOCK | AT91C_TC_LDRA_FALLING;
    AT91C_BASE_TC1->TC_CMR = AT91C_TC_ETRGEDG_FALLING | AT91C_TC_ABETRG | AT91C_TC_CLKS_TIMER_DIV5_CLOCK | AT91C_TC_LDRA_FALLING;
    
    // Enable the clock
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN;    
    AT91C_BASE_TC1->TC_CCR = AT91C_TC_CLKEN;    
}

