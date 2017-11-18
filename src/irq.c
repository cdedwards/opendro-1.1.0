///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// IRQ and interrupt handling functions
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
#include "types.h"
#include "irq.h"
#include "interrupt.h"
#include "board.h"
#include "display.h"
#include "mem.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

static void (*irq_vectors[32])(void);

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static void
register_dump(registers_t* regs)
{
    uint32_t    n;
    uint32_t*   stack;
    
    for (n = 0; n <= 12; n++)
    {
        printf("r%-2"PRIu32": 0x%08"PRIx32" ", n, regs->r[n]);
        if (n % 4 == 3)
            printf("\n");
    }
    printf("sp : 0x%08"PRIx32" ", regs->sp);
    printf("lr : 0x%08"PRIx32" ", regs->lr);
    printf("pc : 0x%08"PRIx32"\n", regs->pc);
    printf("cpsr: 0x%08"PRIx32"\n", regs->cpsr);
    
    stack = (uint32_t *) regs->sp;
    if (stack >= &__data_beg__)
    {
        printf("stack:");
        for (n = 0; n < 32; n++)
        {
            if (&stack[n] > (uint32_t*) &__stack_end__)
                break;
            if (n % 4 == 0)
                printf("\n%p:", &stack[n]);
            printf(" 0x%08"PRIx32, stack[n]);
        }
        printf("\n");
    }
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
isr_dabt(registers_t* regs)
{
    printf("\n-- data abort exception --\n");
    register_dump(regs);
    display_menu_puts(MENU_TOP, "error");
    display_menu_puts(MENU_MIDDLE, "dabt");
    display_menu_printf(MENU_BOTTOM, "%"PRIx32, regs->pc);
}

void
isr_pabt(registers_t* regs)
{
    printf("\n-- prefetch abort exception --\n");
    register_dump(regs);
    display_menu_puts(MENU_TOP, "error");
    display_menu_puts(MENU_MIDDLE, "pabt");
    display_menu_printf(MENU_BOTTOM, "%"PRIx32, regs->pc);
}

void
isr_undf(registers_t* regs)
{
    printf("\n-- undefined instruction exception --\n");
    register_dump(regs);
    display_menu_puts(MENU_TOP, "error");
    display_menu_puts(MENU_MIDDLE, "undf");
    display_menu_printf(MENU_BOTTOM, "%"PRIx32, regs->pc);
}

void
irq_process(void)
{
    uint32_t    irq;
    void        (*isr)(void);
    
    // Get the interrupt from the AIC
    irq = AT91C_BASE_AIC->AIC_ISR & 0x1f;
    
    // Call the interrupt vector
    isr = irq_vectors[irq];
    if (isr)
        (*isr)();
        
    // Clear the interrupt in the AIC
    AT91F_AIC_ClearIt(AT91C_BASE_AIC, irq);
}

void
irq_register(uint32_t       irq,
             irq_priority_t priority,
             irq_type_t     type,
             void           (*isr)(void))
{
    AT91F_AIC_ConfigureIt(
        AT91C_BASE_AIC,
        irq,
        priority,
        (type == IRQ_TYPE_LEVEL) ? AT91C_AIC_SRCTYPE_HIGH_LEVEL :
            AT91C_AIC_SRCTYPE_POSITIVE_EDGE,
        interrupt_irq); 

    irq_vectors[irq] = isr;
    
    AT91F_AIC_EnableIt(AT91C_BASE_AIC, irq);
}

void
irq_trigger(uint32_t irq)
{
    AT91F_AIC_Trig(AT91C_BASE_AIC, irq);
}

void
irq_fast(uint32_t       irq,
         irq_type_t     type)
{
    // Configure the PIO interrupt
    AT91F_AIC_ConfigureIt(
        AT91C_BASE_AIC,
        irq,
        AT91C_AIC_PRIOR_HIGHEST,
        (type == IRQ_TYPE_LEVEL) ? AT91C_AIC_SRCTYPE_HIGH_LEVEL :
            AT91C_AIC_SRCTYPE_POSITIVE_EDGE,
        interrupt_irq);

    // Set the interrupt into fast forcing mode
    AT91C_BASE_AIC->AIC_FFER = (1 << irq);

    // Enable the PIO interrupt
    AT91F_AIC_EnableIt(AT91C_BASE_AIC, irq);
}
