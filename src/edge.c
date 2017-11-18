///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Edge finder object
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
#include "edge.h"
#include "board.h"
#include "drolim.h"
#include "gpio.h"
#include "irq.h"
#include "assert.h"
#include "trace.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

#if EDGE_MAX > 2
#error "Edge finder driver only supports up to two inputs"
#endif

#define ADC_CLOCK           5       // MHz
#define STARTUP_TIME        20      // usec
#define SAMPLE_HOLD_TIME    1000    // nsec

#define DEBOUNCE_LEVEL      3

static bool     running;
static bool     trigger;
static uint32_t debounce;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static void
edge_isr(void)
{
    uint32_t data;
    
    data = AT91C_BASE_ADC->ADC_LCDR & 0xff;

    if (data < 0x80)
        debounce++;
    else
        debounce = 0;
    
    if (debounce >= DEBOUNCE_LEVEL)
    {
        trigger = true;
        edge_stop();
    }
    else
    {
        // Start another ADC conversion
        AT91C_BASE_ADC->ADC_CR = AT91C_ADC_START;
    }
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

bool
edge_trigger(void)
{
    return trigger;
}

void
edge_start(uint8_t edge_num)
{
    ASSERT(edge_num < EDGE_MAX);
    
    // Enable the correct ADC channel
    if (edge_num == 0)
        AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH4;
    else
        AT91C_BASE_ADC->ADC_CHER = AT91C_ADC_CH5;

    running = true;
    trigger = false;
    debounce = 0;
    
    // Start the ADC conversion
    AT91C_BASE_ADC->ADC_CR = AT91C_ADC_START;
}

void
edge_stop(void)
{
    if (running)
    {
        running = false;
        
        // Disable all channels
        AT91C_BASE_ADC->ADC_CHDR = 0xffffffff;
    }
}

void
edge_init(void)
{
    uint32_t prescal = MCLK / (2 * ADC_CLOCK) / 1000000 - 1;
    uint32_t startup = ADC_CLOCK * STARTUP_TIME / 8 - 1;
    uint32_t shtim = ADC_CLOCK * SAMPLE_HOLD_TIME / 1000 - 1;
    
    // Set up ADC timings, disable hardware triggers, and set low-res mode (8-bit)
    AT91C_BASE_ADC->ADC_MR = ((prescal << 8) & AT91C_ADC_PRESCAL) |
                             ((startup << 16) & AT91C_ADC_STARTUP) |
                             ((shtim << 24) & AT91C_ADC_SHTIM) |
                             AT91C_ADC_LOWRES;
    

    // Enable interrupt
    AT91C_BASE_ADC->ADC_IER = AT91C_ADC_DRDY;
    
    // Register the interrupt handler
    irq_register(AT91C_ID_ADC, IRQ_PRIORITY_LOW, IRQ_TYPE_LEVEL, edge_isr);
}

