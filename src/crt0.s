@-----------------------------------------------------------------------------
@ OpenDRO Project
@
@ C run-time initialization
@
@ Copyright (C) 2008-2009 ShumaTech http://www.shumatech.com/
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
@ Processor initialization
@-----------------------------------------------------------------------------

@ Exported symbols
    .global start
    .global fiq
    .global irq
    .global spurious
    
@ External symbols
    .extern main
    .extern lowlevel_init
    .extern swi_init

@ Segment addresses defined by the linker
    .extern __stack_end__
    .extern __bss_beg__
    .extern __bss_end__
    .extern __data_beg__
    .extern __data_end__
    .extern __data_beg_src__

@ Stack sizes
    .set UND_STACK_SIZE, (17 * 4)       @ space for register dump
    .set ABT_STACK_SIZE, (17 * 4)       @ space for register dump
    .set FIQ_STACK_SIZE, (1 * 4)
    .set IRQ_STACK_SIZE, (8 * 8 * 4)    @ 8 words per priority
    .set SVC_STACK_SIZE, (3 * 4)

@ ARM processor modes
    .set MODE_USR, 0x10
    .set MODE_FIQ, 0x11
    .set MODE_IRQ, 0x12
    .set MODE_SVC, 0x13
    .set MODE_ABT, 0x17
    .set MODE_UND, 0x1B
    .set MODE_SYS, 0x1F

@ Interrupt disable flags
    .set I_BIT, 0x80
    .set F_BIT, 0x40

@ Reset controller
    .set RSTC_CR, 0xFFFFFD00
    .set RSTC_PROCRST, 0x1
    .set RSTC_PERRST, 0x4
    .set RSTC_KEY, 0xA5000000

@-----------------------------------------------------------------------------
@ Reset interrupt vectors
@-----------------------------------------------------------------------------
    .section .reset,"ax"
    .arm
    .align 0

@ Only the start vector is executed at reset.  The init code will copy
@ the real interrupt vectors to SRAM and remap the SRAM to the base of
@ memory.  Interrupts before that are not expected so reboot the processor
@ if they trigger.
    ldr     pc, =start              @ start
    ldr     pc, =reboot             @ undefined
    ldr     pc, =reboot             @ software interrupt
    ldr     pc, =reboot             @ program abort
    ldr     pc, =reboot             @ data abort
    nop                             @ reserved
    ldr     pc, =reboot             @ advanced interrupt controller (AIC)
    ldr     pc, =reboot             @ fast interrupt (FIQ)
reboot:
    ldr     r0, =RSTC_CR
    ldr     r1, =RSTC_PROCRST|RSTC_PERRST|RSTC_KEY
    str     r1, [r0]
    b       .

@-----------------------------------------------------------------------------
@ Processor start-up
@-----------------------------------------------------------------------------
    .text
    .arm
    .align 0

start:
@ Set up a stack and disable interrupts for each mode

@ Undefined mode
    ldr     r0, =__stack_end__
    msr     CPSR_c, #MODE_UND|I_BIT|F_BIT
    mov     sp, r0

@ Abort mode
    sub     r0, r0, #UND_STACK_SIZE
    msr     CPSR_c, #MODE_ABT|I_BIT|F_BIT
    mov     sp, r0

@ Fast interrupt mode
    sub     r0, r0, #ABT_STACK_SIZE
    msr     CPSR_c, #MODE_FIQ|I_BIT|F_BIT
    mov     sp, r0

@ Fast interrupt initialization
    mov     r8, #0x80000000
    mov     r8, r8, asr #19
    ldr     r9, =gpio_fifo_data
    mov     r10, #0x00000040

@ Interrupt mode
    sub     r0, r0, #FIQ_STACK_SIZE
    msr     CPSR_c, #MODE_IRQ|I_BIT|F_BIT
    mov     sp, r0

@ Supervisor mode
    sub     r0, r0, #IRQ_STACK_SIZE
    msr     CPSR_c, #MODE_SVC|I_BIT|F_BIT
    mov     sp, r0

@ System mode
    sub     r0, r0, #SVC_STACK_SIZE
    msr     CPSR_c, #MODE_SYS|I_BIT|F_BIT
    mov     sp, r0

@ Perform low-level initialization
    bl      lowlevel_init

@ Zero the BSS segment
    ldr     r1, =__bss_beg__
    ldr     r3, =__bss_end__
    subs    r3, r3, r1
    beq     fill_stack
    mov     r2, #0

zero_loop:
    strb    r2, [r1], #1
    subs    r3, r3, #1
    bgt     zero_loop

@ Fill unallocated memory with markers
fill_stack:
    ldr     r1, =__bss_end__
    ldr     r3, =__stack_end__
    subs    r3, r3, r1
    beq     copy_data
    mov     r2, #0x5a

stack_loop:
    strb    r2, [r1], #1
    subs    r3, r3, #1
    bgt     stack_loop

@ Copy the data segment to its final location
copy_data:
    ldr     r1, =__data_beg__
    ldr     r2, =__data_beg_src__
    ldr     r3, =__data_end__
    subs    r3, r3, r1
    beq     call_main

copy_loop:
    ldrb    r4, [r2], #1
    strb    r4, [r1], #1
    subs    r3, r3, #1
    bgt     copy_loop

call_main:
@ Enable interrupts
    msr CPSR_c, #MODE_SYS

@ Initialize the newlib SWI interface
    bl      swi_init

@ Call the C main() function
    mov     r0, #0              @ r0 = argc
    mov     r1, #0              @ r1 = argv
    mov     fp, #0              @ fp = frame pointer in ARM mode
    mov     r7, #0              @ r7 = frame pointer in Thumb mode
    ldr     lr, =main
    bx      lr

@ Do nothing if main ever returns
    b       .
