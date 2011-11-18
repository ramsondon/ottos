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
#include <arch/arm/omap353x_intc.h>
#include "../pm/process.h"
#include "../sched/scheduler.h"
#include "irq.h"

asm("\t .bss _stack_pointer_interrupted, 4 \n"
    "\t .bss _stack_pointer_restored, 4 \n"
    "\t .bss _stack_pointer_kernel, 4 \n"
    "\t .bss _return_address, 4 \n"
    "\t .bss _function_pointer, 4 \n"
    "\t .bss _irq_started, 4 \n"

    "\t .global _stack_pointer_interrupted \n"
    "\t .global _stack_pointer_restored \n"
    "\t .global _stack_pointer_kernel\n"
    "\t .global _return_address \n"
    "\t .global _function_pointer \n"
    "\t .global _irq_started \n"

    "function_pointer .field _function_pointer, 32 \n"
    "stack_pointer_interrupted .field _stack_pointer_interrupted, 32 \n"
    "stack_pointer_restored .field _stack_pointer_restored, 32 \n"
    "stack_pointer_kernel .field _stack_pointer_kernel, 32 \n"
    "return_address .field _return_address, 32");

extern int stack_pointer_kernel;
extern int stack_pointer_interrupted;
extern int stack_pointer_restored;
extern int return_address;
extern int function_pointer;

static void (*int_handler_[IRQ_MAX_COUNT])();

void irq_init() {
  int i = 0;
  for (i = 0; i < IRQ_MAX_COUNT; i++) {
    int_handler[i] = NULL;
  }
}

void irq_add_handler(int irq_id, void (*fn)(void)) {
  int register_nb = irq_id / 32;
  int_handler_[irq_id] = fn;

  /* activate the specific interrupt (interrupt mask) */
  *((mem_address_t)(MPU_INTC + INTCPS_MIR_CLEARn(register_nb))) |= (1 << (irq_id % 32));
}

void irq_handle_irq(int irq_id) {
  if (int_handler_[irq_id] != NULL) {
    int_handler_[irq_id]();
  }
}

EXTERN void irq_handle() {
  /* get the number of the interrupt */
  int irq_id = *((mem_address_t)(MPU_INTC + INTCPS_SIR_IRQ));

  /* forward the interrupt to the handler routine */
  irq_handle_irq(irq_id);
  *((mem_address_t)(MPU_INTC + INTCPS_CONTROL)) |= 0x1;
}

EXTERN void irq_handle_swi(unsigned r0, unsigned r1, unsigned r2, unsigned r3) {

  // TODO HANDLE INTERRUPT
  // TODO when starting a new process, what's the return register?

  // we are now in the supervisor mode 0x13
  // we must save the return address of the supervisor mode
  // (lr) to return to the user process

  // then we save all registers on the process stack
  // then we schedule the next process

  // if the scheduled process hasn't been started yet
  // 		start the process
  // otherwise
  //		restore the context of the process (registers)

  // the stack frame looks like this:
  // TOP OF STACK 		return address
  // TOP OF STACK -1		cpsr
  // IN BETWEEN			r0 - r11
  // BOTTOM OF STACK		r12


  // ******************************
  // ****** INTERRUPT STACK *******
  // ******************************

  // store the return address
  asm("\t PUSH {r0} \n"
      "\t LDR r0, return_address \n"
      "\t STR lr, [r0] \n"
      "\t POP {r0}");

  // to save the process context we can switch in the system
  // mode, because system mode has the same stack
  asm("\t CPS 0x1F");

  // ******************************
  // ****** PROCESS STACK *********
  // ******************************

  if (irq_started == TRUE) {

    // now save all registers inclusive CPSR
    asm("\t STMFD sp!, {r0-r12} \n"
        "\t MRS r0, cpsr \n"
        "\t STMFD sp!, {r0}");

    // save the new return address
    asm("\t LDR r0, return_address \n"
        "\t LDR r0, [r0] \n"
        "\t STMFD sp!, {r0}");

    // the new stack pointer of the process has to be saved
    // to restore the process
    asm("\t LDR r0, stack_pointer_interrupted \n"
        "\t STR sp, [r0]");

    // the register r0 to r12 and the cpsr, plus the new return address
    // are now on the process stack
  }

  // switch to interrupt handler stack
  asm("\t CPS 0x13");

  // handle interrupts
  switch (r0) {
    case SYS_YIELD:
      // if we need a context switch do switch_context

      // switch to system mode and kernel stack
      asm("\t CPS 0x1F");

      if(irq_started == TRUE) {
        // to switch to the next process we have to switch
        // to the kernel stack and the restore the context
        asm("\t LDR sp, stack_pointer_kernel \n"
            "\t LDR sp, [sp] \n"
            "\t LDMFD sp!, {r0-r12}");
      } else {
        irq_started = TRUE;
      }

      // ******************************
      // ****** KERNEL STACK **********
      // ******************************

      // now we save the new stack pointer of the interrupted process and
      // set the state to ready
      if (process_active != PID_INVALID) {
        process_table[process_active]->stack_pointer
            = stack_pointer_interrupted;
        process_table[process_active]->state = READY;
      }

      // now we schedule the next process
      scheduler_next();

      // mark the new process as running
      process_table[process_active]->state = RUNNING;

      // check if the process has been started
      if (process_table[process_active]->started == FALSE) {
        process_table[process_active]->started = TRUE;

        // the process hasn't been started yet
        // we have to start the process in the user mode
        // therefore we can't just call the function pointer
        // and we have to leave the interrupt handler correctly
        // (either with return or with the LDMFD assembler instruction

        // to start the process we simply set the return address
        // of the handler to the process start (function pointer address)
        function_pointer = process_table[process_active]->initial_address;

        // the new process has its own stack and we have to set
        // the stack pointer of it
        stack_pointer_interrupted
            = process_table[process_active]->stack_pointer;

        // save the kernel context
        // TODO restore r0
        asm("\t STMFD sp!, {r0-r12} \n"
            "\t LDR r0, stack_pointer_kernel \n"
            "\t STR sp, [r0]");

        // load the stack pointer of the process
        asm("\t LDR sp, stack_pointer_interrupted \n"
            "\t LDR sp, [sp]");

        // switch back to the interrupt handler
        asm("\t CPS 0x13");

        // ******************************
        // ****** INTERRUPPT STACK ******
        // ******************************

        // set the return address of the interrupt handler to the entry
        // point of the process
        asm("\t LDR lr, function_pointer \n"
            "\t LDR lr, [lr]");

        // jump to process and leave the interrupt
        asm("\t STM sp, {lr} \n"
            "\t LDM sp, {pc}^");

      } else {

        // restore the context of the next process
        // to restore the context we have to switch to the
        // process stack
        // therefore we set the stack pointer of the process
        stack_pointer_restored = process_table[process_active]->stack_pointer;

        // save the kernel context
        asm("\t STMFD sp!, {r0-r12} \n"
            "\t LDR r0, stack_pointer_kernel \n"
            "\t STR sp, [r0]");

        // load the stack pointer of the process
        asm("\t LDR sp, stack_pointer_restored \n"
            "\t LDR sp, [sp]");

        // the return address is at the top of the stack so we need
        // to read it first
        asm("\t LDR r0, function_pointer \n"
            "\t LDR r1, [sp, #0] \n"
            "\t STR r1, [r0, #0] \n"
            // move stack pointer to the register r0-r12
            "\t ADD sp, sp, #4");

        // now read the cpsr register
        asm("\t LDMFD sp!, {r0} \n"
            "\t MSR SPSR_cxsf, r0");

        // now read the registers r0-r12
        asm("\t LDMFD sp!, {r0-r12}");

        // switch back to the interrupt handler
        asm("\t CPS 0x13");

        // ******************************
        // ****** INTERRUPPT STACK ******
        // ******************************

        // set the return address of the interrupt handler to the entry
        // point of the process
        asm("\t LDR lr, function_pointer \n"
            "\t LDR lr, [lr]");

        // jump to process and leave the interrupt
        asm("\t STM sp, {lr} \n"
            "\t LDM sp, {pc}^");
      }

      break;
    default:
      // ignore
      break;
  }
}
