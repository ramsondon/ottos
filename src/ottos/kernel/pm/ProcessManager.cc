/* ProcessManager.cpp
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

// allocate space for the global variables which
// are necessary for the context switch
asm("\t .bss _stack_pointer, 4");
asm("\t .bss _kernel_stack_pointer, 4");
asm("\t .bss _func_pointer, 4");
asm("\t .global _stack_pointer");
asm("\t .global _kernel_stack_pointer");
asm("\t .global _func_pointer");


#include <vector>

#include <ottos/const.h>

#include "Process.h"
#include "ProcessManager.h"

ProcessManager::ProcessManager() {
  current_ = PID_INVALID;
}

ProcessManager::~ProcessManager() {
}

void ProcessManager::init() {
  // to access the global variables allocated in the .bss
  // segment, a .field declaration is necessary
  asm("stack_pointer_a .field _stack_pointer, 32");
  asm("kernel_stack_pointer_a .field _kernel_stack_pointer, 32");
  asm("func_pointer_a .field _func_pointer, 32");

  process_table_ = std::vector<Process *>(PROCESS_MAX_COUNT);
}

int ProcessManager::run(function_t function)
{
  return -1;
}

#include <stdio.h>
int ProcessManager::switch_process(pid_t to)
{
  // TODO(fdomig@gmail.com) must use ATOMIC_START

  // at this point we are in the kernel stack!

  if(current_ != PID_INVALID) {
    // set the global stack_pointer variable to switch to the process stack
    stack_pointer = process_table_[current_]->stack_pointer();

    // before saving the state of the current process
    // we need to save the state of the kernel
    SAVE_REGISTERS;

    // save the new stack pointer of the kernel
    asm("\t LDR r0, kernel_stack_pointer_a");
    asm("\t STR sp, [r0, #0]");

    // save the state of the active process
    // push the registers (state) on the stack of the process

    // switch to the process' stack
    asm("\t LDR r0, stack_pointer_a");
    asm("\t LDR r0, [r0, #0]");
    asm("\t MOV sp, r0");

    // save the state of the active process
    SAVE_REGISTERS;

    // save the new stack pointer of the active process
    asm("\t LDR r0, stack_pointer_a");
    asm("\t STR sp, [r0, #0]");

    // switch back to the kernel stack
    asm("\t LDR r0, kernel_stack_pointer_a");
    asm("\t LDR r0, [r0, #0]");
    asm("\t MOV sp, r0");

    // restore the kernel state
    LOAD_REGISTERS;

    // save the new stackpointer of the active process
    process_table_[current_]->set_stack_pointer(stack_pointer);
    process_table_[current_]->set_state(READY);

    //printf("accessing of current process stack pointer in (kernel) mode: %x\n", process_table_[current_]->stack_pointer());
  }

  current_ = to;
  process_table_[current_]->set_state(RUNNING);

  //printf("accessing of new process stack pointer in (kernel) mode: %x\n", process_table_[current_]->stack_pointer());

  if(process_table_[current_]->executed() != 0) {
    // reload the state of the new process

    //printf("accessing of new process stack pointer in (kernel) mode: %x\n", process_table_[current_]->stack_pointer());

    func_pointer = process_table_[current_]->entry();

    //printf("jump to pid:%d\taddr:%x\n", current_, func_pointer);

    //printf("accessing of new process stack pointer in (kernel) mode: %x\n", process_table_[current_]->stack_pointer());

    // load the stack of the new process
    stack_pointer = process_table_[current_]->stack_pointer();

    //printf("stack_pointer: %x\n", stack_pointer);

    // save the kernel state
    SAVE_REGISTERS;

    // save the new stack pointer of the kernel
    asm("\t LDR r0, kernel_stack_pointer_a");
    asm("\t STR sp, [r0, #0]");

    // switch to the new process stack
    asm("\t LDR r0, stack_pointer_a");
    asm("\t LDR r0, [r0, #0]");
    asm("\t MOV sp, r0");

    // restore the process state
    LOAD_REGISTERS;

    // load the process entry address
    asm("\t LDR r0, func_pointer_a");
    asm("\t LDR r0, [r0, #0]");

    // set the return address of the new process to here
    asm("\t MOV lr, pc");

    // jump to process
    asm("\t MOV pc, r0");
  } else {
    // the process hasn't been started yet
    process_table_[current_]->mark_as_executed();

    // save the entry point of the process
    func_pointer = (int)process_table_[current_]->func();

    // load the stack of the new process
    stack_pointer = process_table_[current_]->stack_pointer();

    // save the kernel state
    SAVE_REGISTERS;

    // save the new stack pointer of the kernel
    asm("\t LDR r0, kernel_stack_pointer_a");
    asm("\t STR sp, [r0, #0]");

    // switch to the new process stack
    asm("\t LDR r0, stack_pointer_a");
    asm("\t LDR r0, [r0, #0]");
    asm("\t MOV sp, r0");

    // load the process entry address
    asm("\t LDR r0, func_pointer_a");
    asm("\t LDR r0, [r0, #0]");

    // set the return address of the new process to here
    asm("\t MOV lr, pc");

    // jump to process
    asm("\t MOV pc, r0");
  }

  // this is the return point of a process when the process finished
  // switch back to kernel stack
  asm("\t LDR r0, kernel_stack_pointer_a");
  asm("\t LDR r0, [r0, #0]");
  asm("\t MOV sp, r0");

  // restore the kernel state
  LOAD_REGISTERS;


  // TODO(fdomig@gmail.com) must use ATOMIC_END
  return 0;
}

pid_t ProcessManager::current_process() {
  return current_;
}

pid_t ProcessManager::add(Process *proc)
{
  // TODO(fdomig@gmail.com) must use ATOMIC_START
  for(int i = 0;i<PROCESS_MAX_COUNT; i++) {
    if (process_table_[i] == NULL) {

      process_table_[i] = proc;
      proc->set_pid(static_cast<pid_t>(i));
      proc->set_state(READY);

      // allocate separate stack
      proc->set_stack_pointer(0x8200F000 + i * 0x00010000);

      return proc->pid();
    }
  }
  return PID_INVALID;
  // TODO(fdomig@gmail.com) must use ATOMIC_END
}

std::vector<Process *>* ProcessManager::process_table() {
  return &process_table_;
}


