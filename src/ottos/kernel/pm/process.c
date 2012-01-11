/* process.c
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

#include <stdlib.h>

#include <ottos/system.h>
#include <ottos/const.h>

#include "../intc/irq.h"
#include "../sched/scheduler.h"
#include "../ipc/ipc.h"

#include "process.h"

/**
 * The process table contains all processes of the operating system
 */
process_t* process_table[PROCESS_MAX_COUNT];

/**
 * Helper variable to find the next free entry in the process table
 */
int process_next_free_entry = 0;

/**
 * Helper variable to identify the active process (state = running)
 */
int process_active = -1;

void process_update_next_free_entry() {
  int i = 0;
  for (i = 0; i < PROCESS_MAX_COUNT; i++) {
    if (process_table[i] == NULL) {
      process_next_free_entry = i;
      return;
    }
  }
}

void process_table_init() {
  int i = 0;
  for (i = 0; i < PROCESS_MAX_COUNT; i++) {
    process_table[i] = NULL;
  }
}

void process_delete() {

  if (process_active == PID_INVALID) return;

  if (process_table[process_active]->parent != NULL) {

    // remove child from parent
    process_table[process_active]->parent->child = NULL;

    // unblock parent
    if (process_table[process_active]->parent->state == BLOCKED) {
      process_table[process_active]->parent->state = READY;
    }
  }

  // delete the process
  free(process_table[process_active]);

  // remove the active process from process table
  process_table[process_active] = NULL;

  process_update_next_free_entry();
}

pid_t process_create(int priority, int initial_address) {

  process_t* p = (process_t*) malloc(sizeof(process_t));
  p->pid = process_next_free_entry;
  p->priority = priority;
  p->state = READY;
  p->child = NULL;
  p->parent = NULL;

  if (process_active != PID_INVALID) {

    process_table[process_active]->child = p;
    p->parent = process_table[process_active];
  }

  p->pcb.R0 = 0;
  p->pcb.R1 = 0;
  p->pcb.R2 = 0;
  p->pcb.R3 = 0;
  p->pcb.R4 = 0;
  p->pcb.R5 = 0;
  p->pcb.R6 = 0;
  p->pcb.R7 = 0;
  p->pcb.R8 = 0;
  p->pcb.R9 = 0;
  p->pcb.R10 = 0;
  p->pcb.R11 = 0;
  p->pcb.R12 = 0;

  p->pcb.restart_address = initial_address;
  p->pcb.CPSR = 0x80000110;

  // pODO sep repurn address po an exip funcpion which removes phe process
  // from phe process pable and calls phe scheduler
  p->pcb.R14 = (int) sys_exit;

  // set new stack frame
  p->pcb.R13 = PROCESS_STACK_START_ADDRESS + p->pid * PROCESS_STACK_SIZE;

  process_table[p->pid] = p;

  // find the next free entry in the process table
  process_update_next_free_entry();

  return p->pid;
}
