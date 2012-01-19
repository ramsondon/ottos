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
#include <ottos/platform.h>
#include <ottos/timer.h>
#include <ottos/drivers/driver.h>

#include "../pm/process.h"
#include "../sched/scheduler.h"
#include "../ipc/ipc.h"

#include "../mmu/mmu.h"
#include "../loader/loader.h"

#include "irq.h"

#define SAVED_REGISTERS_SPACE (14 * 4)
#define SWI_PARAMETERS_SPACE (4 * 4)

asm(" .bss _pcb_old, 4 ");
asm(" .bss _pcb_new, 4 ");
asm(" .bss _stack_pointer_saved_context, 4 ");
asm(" .bss _stack_pointer_original, 4 ");

asm(" .global _pcb_old ");
asm(" .global _pcb_new ");
asm(" .global _stack_pointer_saved_context ");
asm(" .global _stack_pointer_original ");

asm("pcb_old .field _pcb_old, 32 ");
asm("pcb_new .field _pcb_new, 32 ");
asm("stack_pointer_saved_context .field _stack_pointer_saved_context, 32 ");
asm("stack_pointer_original .field _stack_pointer_original, 32 ");

extern int pcb_old;
extern int pcb_new;
extern int stack_pointer_saved_context;
extern int stack_pointer_original;

#define SAVE_CONTEXT_IRQ \
		asm(" SUB     R14, R14, #4            ; Put return address of the interrupted task into R14 "); \
		asm(" STMFD   R13!, {R0-R12, R14}     ; Save Process-Registers "); \
		asm(" LDR     R0, stack_pointer_saved_context");	\
		asm(" STR     R13, [R0], #0");	\
		stack_pointer_original = stack_pointer_saved_context + SAVED_REGISTERS_SPACE;

#define SAVE_CONTEXT_DABT \
		asm(" SUB     R14, R14, #8"); \
		asm(" STMFD   R13!, {R0-R12, R14}     ; Save Process-Registers "); \
		asm(" LDR     R0, stack_pointer_saved_context");	\
		asm(" STR     R13, [R0], #0");	\
		stack_pointer_original = stack_pointer_saved_context + SAVED_REGISTERS_SPACE;

#define SAVE_CONTEXT_PABT \
    asm(" SUB     R14, R14, #4"); \
    asm(" STMFD   R13!, {R0-R12, R14}     ; Save Process-Registers "); \
    asm(" LDR     R0, stack_pointer_saved_context");  \
    asm(" STR     R13, [R0], #0");  \
    stack_pointer_original = stack_pointer_saved_context + SAVED_REGISTERS_SPACE;

#define SAVE_CONTEXT_SWI \
		asm(" STMFD   R13!, {R0-R12, R14} ; Save Process-Registers ");	\
		asm(" LDR     R0, stack_pointer_saved_context");	\
		asm(" STR     R13, [R0], #0");	\
		stack_pointer_original = stack_pointer_saved_context + SAVED_REGISTERS_SPACE + SWI_PARAMETERS_SPACE;

#define RESTORE_AND_SWITCH_CONTEXT \
		asm(" LDMFD   R13!, {R0-R12, PC}^");

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

#pragma TASK(irq_handle_dabt)
void irq_handle_dabt() {
  SAVE_CONTEXT_DABT;

  //kernel_print("data abort\r\n");

  if (mmu_handle_data_abort() == TRUE) {
    context_switch();
  }

  RESTORE_AND_SWITCH_CONTEXT;
  //asm(" LDMFD   R13!, {R0-R12, R14}");
  //asm(" SUBS    PC, R14, #8");
}

#pragma TASK(irq_handle_pabt)
void irq_handle_pabt() {
  SAVE_CONTEXT_PABT;

  //kernel_print("prefetch abort\r\n");

  if (mmu_handle_prefetch_abort() == TRUE) {
    context_switch();
  }

  RESTORE_AND_SWITCH_CONTEXT;
  //asm(" LDMFD   R13!, {R0-R12, R14}");
  //asm(" SUBS    PC, R14, #4");
}

void context_switch() {
  // reload the stack pointer of the interrupt handler to get
  // the saved context
  asm(" LDR     R13, stack_pointer_saved_context");
  asm(" LDR     R13, [R13], #0");

  pcb_old = PID_INVALID;
  if (process_active != PID_INVALID) {
    if (process_table[process_active]->state == RUNNING) {
      process_table[process_active]->state = READY;
    }
    // Get the TCB's of the processes to switch the context
    pcb_old = (int) &process_table[process_active]->pcb.CPSR;
  }
  scheduler_next();

  // switch to active process memory
  mmu_init_memory_for_process(process_table[process_active]);

  process_table[process_active]->state = RUNNING;

  pcb_new = (int) &process_table[process_active]->pcb.CPSR;

  // Load addresses of the TCB's of the Tasks to switch into R0 and R1
  if (pcb_old != PID_INVALID) {
    asm(" LDR     R0, pcb_old ;" );
    asm(" LDR     R0, [R0], #0 ;" );
  }
  asm(" LDR     R1, pcb_new ;" );
  asm(" LDR     R1, [R1], #0 ;" );

  if (pcb_old != PID_INVALID) {
    //	  // reload the stack pointer of the interrupt handler to get
    //	  // the saved context
    //	  asm(" LDR     R13, stack_pointer_saved_context");
    //	  asm(" LDR     R13, [R13], #0");

    // First store the old precess's User mode state to the PCB pointed to by R0."
    asm(" MRS     R12, SPSR             ; Get CPSR of interrupted process" );
    asm(" STR     R12, [R0], #8         ; Store CPSR to PCB, point R0 at PCB location for R0 value" );
    asm(" LDMFD   R13!, {R2, R3}        ; Reload R0/R1 of interrupted process from stack" );
    asm(" STMIA   R0!, {R2, R3}         ; Store R0/R1 values to PCB, point R0 at PCB location for R2 value" );
    asm(" LDMFD   R13!, {R2-R12, R14} ; Reload remaining stacked values" );
    asm(" STR     R14, [R0, #-12]       ; Store R14_irq, the interrupted process's restart address" );
    asm(" STMIA   R0, {R2-R14}^         ; Store user R2-R14 ");
  }

  // restore the stack pointer of the interrupt
  asm(" LDR     R13, stack_pointer_original");
  asm(" LDR     R13, [R13], #0");

  // Then load the new process's User mode state and return to it.");
  asm(" LDMIA   R1!, {R12, R14}       ; Put interrupted process's CPSR" );
  asm(" MSR     SPSR_fsxc, R12        ; and restart address in SPSR_irq and R14_irq" );
  asm(" LDMIA   R1, {R0-R14}^         ; Load user R0-R14" );
  asm(" NOP                           ; Note: cannot use banked register immediately after User mode LDM" );

  asm(" MOVS    PC, R14               ; Return to address in R14_irq, with SPSR_irq -> CPSR transfer" );
}

#pragma TASK(irq_handle)
EXTERN void irq_handle() {

  SAVE_CONTEXT_IRQ;

  mmu_switch_to_kernel();

  *((mem_address_t*) (MPU_INTC + INTCPS_CONTROL)) |= 0x1;

  /* forward the interrupt to the handler routine */
  irq_handle_irq(*((mem_address_t*) (MPU_INTC + INTCPS_SIR_IRQ)));

  if (process_active != PID_INVALID) {
    mmu_init_memory_for_process(process_table[process_active]);
  }

  RESTORE_AND_SWITCH_CONTEXT;
}

void irq_swi_handle_sys_open(unsigned int device) {
  driver_get(device).open(device);
}

void irq_swi_handle_sys_write(unsigned int device, unsigned int count,
                              unsigned int buffer_pointer) {

  //char* buffer = (char*)mmu_get_physical_address(process_table[process_active], buffer_pointer);

  char casted_buffer = (char) buffer_pointer;
  //driver_get(device).write(device, count, buffer);
  driver_get(device).write(device, count, &casted_buffer);
}

void irq_swi_handle_sys_print(int length, unsigned int output_buffer) {
  // output_buffer is an address

  char output[1024];
  char* temp = (char*) mmu_get_physical_address(process_table[process_active],
                                                output_buffer);
  int i = 0;

  for (i = 0; i < length; i++) {
    output[i] = temp[i];
  }
  output[length] = '\0';

  kernel_print(output);
}

void irq_swi_handle_sys_bind(unsigned int ns, unsigned int success) {
  const char* namespace =
      (const char*) mmu_get_physical_address(process_table[process_pid()], ns);

  int* result = (int*)mmu_get_physical_address(process_table[process_pid()], success);

  *result = ipc_bind(namespace, process_pid());
  // TODO: ramsondon@gmail.com check if works
}

void irq_swi_handle_sys_send(unsigned int ns, unsigned int msg, unsigned int success) {
  const char* namespace =
      (const char*) mmu_get_physical_address(process_table[process_pid()], ns);
  message_t* message =
      (message_t*) mmu_get_physical_address(process_table[process_pid()], msg);

  int* result = (int*)mmu_get_physical_address(process_table[process_pid()], success);

  *result = ipc_send_msg(namespace, *message, process_pid());
  // TODO: ramsondon@gmail.com check if works
}

void irq_handle_sys_wait_msg(unsigned int ns) {
  const char* namespace =
        (const char*) mmu_get_physical_address(process_table[process_pid()], ns);

  if (ipc_lookup_msg_concrete(namespace, process_pid()) == WAITING) {
    process_table[process_pid()]->state = BLOCKED;
    process_table[process_pid()]->blockstate = IPC_WAIT;

    // TODO: ramsondon@gmail.com check if works
    context_switch();
  }
}

void irq_swi_handle_sys_receive(unsigned int ns, unsigned int msg, unsigned int success) {
  const char* namespace =
      (const char*) mmu_get_physical_address(process_table[process_pid()], ns);
  message_t* message =
      (message_t*) mmu_get_physical_address(process_table[process_pid()], msg);
  int* result = (int*)mmu_get_physical_address(process_table[process_pid()], success);

  *result = ipc_receive_msg(namespace, message, process_pid());
  // TODO: ramsondon@gmail.com check if works
}

#pragma TASK(irq_handle_swi)
EXTERN void irq_handle_swi(unsigned r0, unsigned r1, unsigned r2, unsigned r3) {

  SAVE_CONTEXT_SWI;

  mmu_switch_to_kernel();

  // handle interrupts
  switch (r0) {
    case SYS_YIELD:
      context_switch();
      break;
    case SYS_EXIT:
      kernel_print("sys_exit\r\n");
      // delete the active process
      process_delete();

      // set the process_active to invalid
      // to tell the context switch, that no
      // old pcb has to be saved
      process_active = PID_INVALID;
      context_switch();
    case SYS_MMU_TEST:
      // TODO r1 = buffer pointer for output
      kernel_print("sys_mmu_test\r\n");
      break;
    case SYS_CREATE_PROCESS:
      // r1 = priority
      // r2 = code bytes
      // r3 = wait_for_exit
      /*load_process_code(process_table[process_create(r1)], r2);
       if (r3 != FALSE) {
       // the current process will be blocked until the
       // child exited

       // TODO is blocked the correct state?
       process_table[process_active]->state = BLOCKED;

       // block current process
       // switch to next process
       context_switch();

       }*/
      break;
    case SYS_OPEN:
      // r1 = device_t
      irq_swi_handle_sys_open(r1);
      break;
    case SYS_READ:
      // TODO implement return value
      break;
    case SYS_WRITE:
      // r1 = device_t
      // r2 = size of buffer
      // r3 = buffer (int)
      irq_swi_handle_sys_write(r1, r2, r3);
      break;
    case SYS_CLOSE:
      // TODO implement
      break;
    case SYS_PRINT:
      // r1 = length
      // r2 = output buffer
      irq_swi_handle_sys_print(r1, r2);
      break;
    case SYS_BIND_NAMESPACE:
      irq_swi_handle_sys_bind(r1, r2);
      break;
    case SYS_SEND:
      irq_swi_handle_sys_send(r1, r2, r3);
      break;
    case SYS_WAIT_MSG:
      irq_handle_sys_wait_msg(r1);
      break;
    case SYS_RECEIVE:
      irq_swi_handle_sys_receive(r1, r2, r3);
      break;
    default:
      // ignore
      break;
  }

  if (process_active != PID_INVALID) {
    mmu_init_memory_for_process(process_table[process_active]);
  }

  // reload process context
  asm(" LDMFD R13!, {R0-R12, R14}");

  // restore the interrupt stack pointer
  asm(" LDR R13, stack_pointer_original");
  asm(" LDR R13, [R13], #0");

  // switch to process
  asm(" MOVS PC, R14");

}
