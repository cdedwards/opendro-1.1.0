@-----------------------------------------------------------------------------
@ OpenDRO Project
@
@ Interrupt processing
@
@ Copyright (C) 2008-2011 ShumaTech http://www.shumatech.com/
@
@ This program is free software: you can redistribute it and/or modify
@ it under the terms of the GNU General Public License as published by
@ the Free Software Foundation, either version 3 of the License, or
@ (at your option) any later version.
@
@ This program is distributed in the hope that it will be useful,
@ but WITHOUT ANY WARRANTY; without even the implied warranty of
@ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@ GNU General Public License for more details.
@
@ You should have received a copy of the GNU General Public License
@ along with this program.  If not, see <http:@www.gnu.org/licenses/>.
@-----------------------------------------------------------------------------

@-----------------------------------------------------------------------------
@ Interrupt processing
@-----------------------------------------------------------------------------

@ Exported symbols
    .global interrupt_fiq
    .global interrupt_irq
    .global interrupt_spu
    .global interrupt_disable
    .global interrupt_enable
    .global gpio_fifo
    .global gpio_fifo_data
    
@ External symbols
    .extern irq_process
    .extern swi_isr
    .extern isr_undf
    .extern isr_pabt
    .extern isr_dabt
    .extern isr_fiq
    .extern isr_spu

@ Pin FIFO size
    .set GPIO_FIFO_SIZE, 256
    
@ ARM processor modes
    .set MODE_USR,  0x10
    .set MODE_FIQ,  0x11
    .set MODE_IRQ,  0x12
    .set MODE_SVC,  0x13
    .set MODE_ABT,  0x17
    .set MODE_UND,  0x1B
    .set MODE_SYS,  0x1F
    .set MODE_MASK, 0x1F

@ Control bits
    .set I_BIT, 0x80        @ interrupt disable
    .set F_BIT, 0x40        @ fast interrupt disable
    .set T_BIT, 0x20        @ thumb mode

@ Advanced interrupt controller
    .set AT91C_BASE_AIC, 0xFFFFF000
    .set AIC_EOICR, 304

    .section .vectors,"ax"
    .arm
    .align 0

@-----------------------------------------------------------------------------
@ Interrupt Vectors
@-----------------------------------------------------------------------------
    ldr     pc, =start              @ start
    ldr     pc, =interrupt_undf     @ undefined
    ldr     pc, =interrupt_swi      @ software interrupt
    ldr     pc, =interrupt_pabt     @ program abort
    ldr     pc, =interrupt_dabt     @ data abort
    nop                             @ reserved
    ldr     pc, [pc, #-0xF20]       @ advanced interrupt controller (AIC)
    ldr     pc, [pc, #-0xF20]       @ fast interrupt (FIQ)
    
@-----------------------------------------------------------------------------
@ Save all registers for the exception to the stack except the pc
@-----------------------------------------------------------------------------
register_save:
@ Save working registers to the stack
    push    {r0-r12}

@ Push the saved CPSR to the stack
    mrs     r0, SPSR
    push    {r0}

@ Switch to the mode that caused the exception
    mrs     r2, CPSR
    and     r0, r0, #MODE_MASK
    msr     CPSR_c, r0

@ Get the sp and lr register values
    mov     r0, sp
    mov     r1, lr

@ Switch back to the exception mode
    msr     CPSR_c, r2

@ Save the sp and lr to the stack
    push    {r0-r1} 

@ Return to the caller
    bx      lr
    
@-----------------------------------------------------------------------------
@ Undefined Instruction Interrupt
@-----------------------------------------------------------------------------
interrupt_undf:
@ Save the pc of the exception to the stack
    sub     lr, lr, #4
    push    {lr}

@ Save the exception registers
    bl      register_save

@ Call the C ISR with the instruction address
    sub     r0, lr, #4
    msr     CPSR_c, #MODE_SYS
    bl      isr_undf

@ interrupt is fatal so spin forever
    b       .                       

@-----------------------------------------------------------------------------
@ Software Interrupt
@-----------------------------------------------------------------------------
interrupt_swi:
@ Save LR on SVC stack 
    stmfd   sp!, {r14}

@ Save SPSR on SVC stack
    mrs     r14, SPSR
    stmfd   sp!, {r14}

@ Enable interrupts and switch to SYS mode
    msr     CPSR_c, #MODE_SYS

@ Save the C IP register on SYS stack 
    stmfd   sp!, {r12}

@ Call the C SWI ISR
    bl      swi_isr

@ Restore C IP register from SYS stack
    ldmfd   sp!, {r12}

@ Disable interrupts and switch back to SVC mode 
    msr     CPSR_c, #MODE_SVC | I_BIT

@ Restore the SPSR from IRQ stack
    ldmfd   sp!, {r14}
    msr     SPSR_cxsf, r14

@ Restore CPSR and return from interrupt
    ldmfd   sp!, {pc}^

@-----------------------------------------------------------------------------
@ Prefetch Abort Interrupt
@-----------------------------------------------------------------------------
interrupt_pabt:
@ Save the pc of the exception to the stack
    sub     lr, lr, #4
    push    {lr}

@ Save the exception registers
    bl      register_save

@ Call the C ISR with the aborted address
    sub     r0, lr, #4
    msr     CPSR_c, #MODE_SYS
    bl      isr_pabt

@ interrupt is fatal so spin forever
    b       .                       
    
@-----------------------------------------------------------------------------
@ Data Abort Interrupt
@-----------------------------------------------------------------------------
interrupt_dabt:
@ Save the pc of the exception to the stack
    sub     lr, lr, #8
    push    {lr}

@ Save the exception registers
    bl      register_save
    
@ Call the C ISR with the saved registers
    mov     r0, sp
    msr     CPSR_c, #MODE_SYS
    bl      isr_dabt

@ interrupt is fatal so spin forever
    b       .                       

@-----------------------------------------------------------------------------
@ FIQ Interrupt
@ r8  = AT91 registers (0xfffff000)
@ r9  = fifo_data address
@ r10 = IRQ to signal 
@-----------------------------------------------------------------------------
interrupt_fiq:
@ Clear the interrupt
    ldr     r11, [r8, #0x44c]       @ PIO_ISR (0xfffff44c)

@ Capture the PIO data
    ldr     r11, [r8, #0x43c]       @ PIO_PDSR (0xfffff43c)

@ Store the PIO data in the fifo
    ldr     r12, gpio_fifo_write
    str     r11, [r9, r12, lsl #2]

@ Increment and wrap the fifo write index   
    add     r12, r12, #1
    ldr     r11, gpio_fifo_size
    cmp     r11, r12
    moveq   r12, #0

@ Check end of fifo and update write index  
    ldr     r11, gpio_fifo_read
    cmp     r11, r12
    strne   r12, gpio_fifo_write

@ Signal the PIO fifo reader
    str     r10, [r8, #0x12c]       @ AIC_ISCR (0xfffff12c)
            
@ Return from the interrupt
    subs    pc, lr, #4

@ PIO FIFO
gpio_fifo:
gpio_fifo_read:
    .word 0
gpio_fifo_write:
    .word 0
gpio_fifo_size:
    .word GPIO_FIFO_SIZE
gpio_fifo_data:
    .fill GPIO_FIFO_SIZE, 4, 0

@-----------------------------------------------------------------------------
@ IRQ Interrupt
@-----------------------------------------------------------------------------
interrupt_irq:
@ Save adjusted LR on IRQ stack
    sub     r14, r14, #4
    stmfd   sp!, {r14}
    
@ Save SPSR on IRQ stack
    mrs     r14, SPSR
    stmfd   sp!, {r14}
    
@ Enable interrupts and switch to SYS mode
    msr     CPSR_c, #MODE_SYS

@ Save unpreserved C registers in SYS stack 
    stmfd   sp!, {r0-r3, r12, r14}

@ Call the C IRQ ISR
    bl      irq_process

@ Restore the C registers from SYS stack
    ldmfd   sp!, {r0-r3, r12, r14}

@ Disable interrupts and switch back to IRQ mode 
    msr     CPSR_c, #MODE_IRQ | I_BIT

@ Signal to AIC interrupt processing is done
    ldr     r14, =AT91C_BASE_AIC
    str     r14, [r14, #AIC_EOICR]
    
@ Restore the SPSR from IRQ stack
    ldmfd   sp!, {r14}
    msr     SPSR_cxsf, r14

@ Restore CPSR and return from interrupt
    ldmfd   sp!, {pc}^

@-----------------------------------------------------------------------------
@ Spurious Interrupt
@-----------------------------------------------------------------------------
interrupt_spu:
@ Return from the interrupt
    subs    pc, lr, #4

@-----------------------------------------------------------------------------
@ Disable Interrupts
@-----------------------------------------------------------------------------
interrupt_disable:
    mrs     r0, CPSR
    orr     r0, r0, #F_BIT | I_BIT
    msr     CPSR_c, r0

@ Return to the caller
    bx      lr

@-----------------------------------------------------------------------------
@ Enable Interrupts
@-----------------------------------------------------------------------------
interrupt_enable:
    mrs     r0, CPSR
    and     r0, r0, #!(F_BIT | I_BIT)
    msr     CPSR_c, r0

@ Return to the caller
    bx      lr
