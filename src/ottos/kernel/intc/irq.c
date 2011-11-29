/* irq.h
 *
 * Copyright (c) 2011 The ottos project.
 *
 * This work is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This work is distributed in the hope that it will be useful, but without
 * any warranty; without even the implied warranty of merchantability or
 * fitness for a particular purpose. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *
 *  Created on: 11.11.2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <ottos/syscalls.h>
#include <ottos/const.h>
#include <ottos/memory.h>
#include <ottos/kernel.h>

#include <arch/arm/omap353x_intc.h>

#include "../pm/process.h"
#include "../sched/scheduler.h"
#include "irq.h"
#include "../timer/timer.h"

asm(" .bss _pcb1, 4 ");
asm(" .bss _pcb2, 4 ");
asm(" .bss _stack_pointer_saved_context, 4 ");

asm(" .global _pcb1 ");
asm(" .global _pcb2 ");
asm(" .global _stack_pointer_saved_context ");

asm("pcb1 .field _pcb1, 32 ");
asm("pcb2 .field _pcb2, 32 ");
asm("stack_pointer_saved_context .field _stack_pointer_saved_context, 32 ");

extern int pcb1;
extern int pcb2;
extern int stack_pointer_saved_context;

static void (*int_handler_[IRQ_MAX_COUNT])();

void context_switch();

void irq_register_context_switch() {
  // register context switch
  timer_add_handler(context_switch, 100);
}

void irq_init() {
  ARRAY_INIT(int_handler_, IRQ_MAX_COUNT, NULL);
}

void irq_enable() {
  _enable_IRQ();
}

void irq_disable() {
  _disable_IRQ();
}

void irq_add_handler(int irq_id, void(*fn)(void)) {
  int register_nb = irq_id / 32;
  int_handler_[irq_id] = fn;

  /* activate the specific interrupt (interrupt mask) */
  *((mem_address_t*) (MPU_INTC + INTCPS_MIR_CLEARn(register_nb))) |= (1
      << (irq_id % 32));
}

void irq_handle_irq(int irq_id) {
  if (int_handler_[irq_id] != NULL) {
    int_handler_[irq_id]();
  }
}

/*
 * Handles Undefined Interrupt Exception
 * This means if you as a programmer did something WRONG!
 */
void irq_handle_udef() {
  _disable_interrupts();
  kernel_panic("undefined\n\r");
}

void irq_handle_dabt() {
  _disable_interrupts();
  kernel_panic("data abort\n\r");
}

void irq_handle_pabt() {
  _disable_interrupts();
  kernel_panic("prefetch abort\n\r");
}

void context_switch() {

  // reload the stack pointer of the interrupt handler to get
  // the saved context
  asm(" LDR     R13, stack_pointer_saved_context");
  asm(" LDR     R13, [R13], #0");

  pcb1 = PID_INVALID;
  if (process_active != PID_INVALID) {
    // Get the TCB's of the processes to switch the context
    pcb1 = (int) &process_table[process_active]->pcb.CPSR;
  }
  scheduler_next();
  pcb2 = (int) &process_table[process_active]->pcb.CPSR;

  // Load addresses of the TCB's of the Tasks to switch into R0 and R1
  if (pcb1 != PID_INVALID) {
    asm(" LDR     R0, pcb1 ;" );
    asm(" LDR     R0, [R0], #0 ;" );
  }
  asm(" LDR     R1, pcb2 ;" );
  asm(" LDR     R1, [R1], #0 ;" );

  if (pcb1 != PID_INVALID) {
    // First store the old precess's User mode state to the PCB pointed to by R0."
    asm(" MRS     R12, SPSR             ; Get CPSR of interrupted process" );
    asm(" STR     R12, [R0], #8         ; Store CPSR to PCB, point R0 at PCB location for R0 value" );
    asm(" LDMFD   R13!, {R2, R3}        ; Reload R0/R1 of interrupted process from stack" );
    asm(" STMIA   R0!, {R2, R3}         ; Store R0/R1 values to PCB, point R0 at PCB location for R2 value" );
    asm(" LDMFD   R13!, {R2, R3, R12, R14} ; Reload remaining stacked values" );
    asm(" STR     R14, [R0, #-12]       ; Store R14_irq, the interrupted process's restart address" );
    asm(" STMIA   R0, {R2-R14}^         ; Store user R2-R14 ");
  } else {
    // restore old stack pointer of the interrupt handler
    // 6 registers are on the stack, therefore we have to add
    // 6 * 4 bytes to restore the stack pointer
    asm(" ADD     R13, R13, #24");
  }

  // Then load the new process's User mode state and return to it.");
  asm(" LDMIA   R1!, {R12, R14}       ; Put interrupted process's CPSR" );
  asm(" MSR     SPSR_fsxc, R12        ; and restart address in SPSR_irq and R14_irq" );
  asm(" LDMIA   R1, {R0-R14}^         ; Load user R0-R14" );
  asm(" NOP                           ; Note: cannot use banked register immediately after User mode LDM" );

  //  // !! The next line is a added to the code in the ARM-book, because
  //  // normally this would be called at the end of the function, but we don't reach that
  //  asm(" ADD     R13, R13, #4 ");

  asm(" MOVS    PC, R14               ; Return to address in R14_irq, with SPSR_irq -> CPSR transfer" );
}

#pragma TASK(irq_handle)
EXTERN void irq_handle() {

  // This will be called before entering the function
  // SUB R13, R13, #4

  asm(" SUB     R14, R14, #4            ; Put return address of the interrupted task into R14 ");
  asm(" STMFD   R13!, {R0-R3, R12, R14} ; Save Process-Registers ");

  asm(" LDR     R0, stack_pointer_saved_context");
  asm(" STR     R13, [R0], #0");

  *((mem_address_t*) (MPU_INTC + INTCPS_CONTROL)) |= 0x1;

  /* forward the interrupt to the handler routine */
  irq_handle_irq(*((mem_address_t*) (MPU_INTC + INTCPS_SIR_IRQ)));

  asm(" LDMFD   R13!, {R0-R3, R12, PC}^");
}

//#pragma TASK(irq_handle_swi)
EXTERN void irq_handle_swi(unsigned r0, unsigned r1, unsigned r2, unsigned r3) {

  // handle interrupts
  switch (r0) {
    case SYS_YIELD:
      context_switch();
      break;
    default:
      // ignore
      break;
  }
}
