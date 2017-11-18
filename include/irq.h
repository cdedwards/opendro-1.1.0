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
#ifndef IRQ_H_
#define IRQ_H_

#include "types.h"
#include "board.h"

/// Interrupt type
typedef enum irq_type
{
    IRQ_TYPE_LEVEL,
    IRQ_TYPE_EDGE,
} irq_type_t;

/// Relative interrupt priorities
typedef enum irq_priority
{
    IRQ_PRIORITY_LOWEST,
    IRQ_PRIORITY_LOW,
    IRQ_PRIORITY_MEDIUM,
    IRQ_PRIORITY_HIGH,
    IRQ_PRIORITY_HIGHEST,
} irq_priority_t;

typedef struct registers
{
    // Order below MUST match the order in interrupt.s
    uint32_t sp;
    uint32_t lr;
    uint32_t cpsr;
    uint32_t r[13];
    uint32_t pc;
} registers_t;

/// Software interrupt to process scale data
#define IRQ_SCALE_PROCESS   AT91C_ID_US0

/// Software interrupt to send scale fast sequence
#define IRQ_SCALE_FAST      AT91C_ID_US1

/// GPIO change interrupt for scale inputs
#define IRQ_SCALE_IN        AT91C_ID_PIOA

/// Software interrupt to process the Lua shell
#define IRQ_LUASH           AT91C_ID_SSC

/// Software interrupt to process the keypad
#define IRQ_KEYPAD          AT91C_ID_TC2

/// Software interrupt for LCD refresh
#define IRQ_LCD             AT91C_ID_TC1

/// Software interrupt for SD init
#define IRQ_SD_INIT         AT91C_ID_TC0

/// GPIO change interrupt for SD card detect
#define IRQ_SD_DETECT       AT91C_ID_PIOC

// \brief   Called to handle an ARM7 data abort exception
//
// \param   regs    Saved registers
void
isr_dabt(registers_t* regs);

// \brief   Called to handle an ARM7 program abort exception
//
// \param   regs    Saved registers
void
isr_pabt(registers_t* regs);

// \brief   Called to handle an ARM7 undefined exception
//
// \param   regs    Saved registers
void
isr_undf(registers_t* regs);

// \brief   Called to handle the ARM7 IRQ interrupt
void
irq_process(void);

// \brief   Register to handle one of the prioritized interrupts
//
// \param   irq     Interrupt number
// \param   irq     Interrupt priority
// \param   type    Interrupt type
// \param   isr     Interupt service routine to call
void
irq_register(uint32_t       irq,
             irq_priority_t priority,
             irq_type_t     type,
             void           (*isr)(void));

// \brief   Cause a manual trigger of one of the prioritized interrupts
//
// \param   irq     Interrupt number
void
irq_trigger(uint32_t irq);

// \brief   Register a fast interrupt
//
// \param   irq     Interrupt number
// \param   type    Interrupt type
void
irq_fast(uint32_t       irq,
         irq_type_t     type);

#endif // IRQ_H_
