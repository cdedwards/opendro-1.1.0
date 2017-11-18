///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Low-level hardware initialization
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
#include "board.h"
#include "interrupt.h"

/// Assembler interrupt entry points
extern void fiq_asm_handler();
extern void irq_asm_handler();
extern void spu_asm_handler();

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
lowlevel_init(void)
{
    int n;

    // Remap the SRAM to the top of memory if not already done
    if (*((int *) AT91C_ISRAM) != *((int *) 0x0)) 
        AT91C_BASE_MC->MC_RCR = AT91C_MC_RCB;
    
    // Set the flash mode to 1.5 microseconds with 1 wait state
    AT91C_BASE_MC->MC_FMR = ((MCLK * 3 / 2000000 + 1) << 16) |
        AT91C_MC_FWS_1FWS;

    // Enable the main oscillator and set the start-up time
    AT91C_BASE_PMC->PMC_MOR = ((OSC_STARTUP * SCLK / 8000000 + 1) << 8 ) | AT91C_CKGR_MOSCEN;

    // Wait until the oscillator is stable
    while(!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));

    // Set the PLL multiplier, divider, and start-up time
    AT91C_BASE_PMC->PMC_PLLR = (PLL_DIV << 0) |
        ((PLL_STARTUP * SCLK / 1000000 + 1) << 8) |
        ((PLL_MUL - 1) << 16) |
        (USB_DIV << 28);

    // Wait until the PLL is locked
    while(!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK));

    // Set the clock divider
    AT91C_BASE_PMC->PMC_MCKR = (CLK_DIV << 2);

    // Wait for clock ready
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    // Set the PLL as the master clock source
    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK;

    // Wait for the master clock ready
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));
    
    // Disable all interrupts
    AT91C_BASE_AIC->AIC_IDCR = 0xffffffff;
    AT91C_BASE_AIC->AIC_ICCR = 0xffffffff;

    // Clear any pending interrupts
    for (n = 0; n < 8; n++)
        AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
    
    // Initialize the AIC vectors
    AT91C_BASE_AIC->AIC_SVR[0] = (uint32_t) interrupt_fiq;
    for (n = 1; n < 32; n++) {
        AT91C_BASE_AIC->AIC_SVR[n] = (uint32_t) interrupt_irq;
    }
    AT91C_BASE_AIC->AIC_SPU = (uint32_t) interrupt_spu;
}

void
reset(void)
{
    AT91C_BASE_RSTC->RSTC_RCR = (0xa5 << 24) | AT91C_RSTC_PROCRST;    
}
