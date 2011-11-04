/* SyscallHandler.cpp
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
 *  Created on: 27 Oct 2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <ottos/syscalls.h>
#include <ottos/types.h>

#include <vector>

#include "../pm/Process.h"
#include "../pm/ProcessManager.h"
#include "../sched/Scheduler.h"
#include "SyscallHandler.h"

asm("\t .bss _return_register, 4");
asm("\t .bss _temp_pointer, 4");
asm("\t .bss _temp_pointer2, 4");
asm("\t .global _return_register");
asm("\t .global _temp_pointer");
asm("\t .global _temp_pointer2");
asm("return_register_a .field _return_register, 32");
asm("temp_pointer_a .field _temp_pointer, 32");
asm("temp_pointer2_a .field _temp_pointer2, 32");

extern int temp_pointer;
extern int temp_pointer2;
extern int return_register;

void handle_swi(unsigned r0, unsigned r1, unsigned r2, unsigned r3) {

  // save the return address of the swi interrupt
  // so that the process has the correct return address
  // if a process switch happens
  asm("\t PUSH {r0}");
  asm("\t LDR r0, return_register_a");
  asm("\t STR lr, [r0, #0]");
  asm("\t POP {r0}");

  // save the stack pointer of the process stack
  // switch back to the process stack
  asm("\t MRS r0, cpsr");
  //asm("\t BIC r0, r0, #0x1F  ; CLEAR MODES");
  //asm("\t ORR r0, r0, #0x1F  ; SET Super-User MODE");
  asm("\t BIC r0, r0, #0x1F  ; CLEAR MODES");
  asm("\t ORR r0, r0, #0x1F  ; SET User MODE");
  asm("\t MSR cpsr_cf, r0");
  // TODO: GOODBYE r0!!!!

  // save the stack pointer in the temp_pointer variable
  asm("\t LDR r0, temp_pointer_a");
  asm("\t STR sp, [r0, #0]");

  temp_pointer2 = kernel_stack_pointer;

  // switch to kernel stack
  asm("\t LDR r0, temp_pointer2_a");
  asm("\t LDR r0, [r0, #0]");
  asm("\t MOV sp, r0");

  // load saved registers of the kernel
  LOAD_REGISTERS;

  // save the new stack pointer of the process
  Process* currentProcess = kernel->process_manager()->process_table()->at(kernel->process_manager()->current_process());
  currentProcess->set_stack_pointer(temp_pointer);
  currentProcess->set_entry(return_register);


  //printf("pid:%d\tlr:%x\tnew stack pointer: %x\n", currentProcess->pid(), return_register, currentProcess->stack_pointer());

  int call = SYS_YIELD; // (SystemCall)r0

  switch (call) {
    case SYS_YIELD:
      // execute the system call
      kernel->scheduler()->run();
      break;
    default:
      // error
      break;
  }

}




