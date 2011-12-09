/* process.h
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

#ifndef PROCESS_C_H_
#define PROCESS_C_H_

#include <ottos/const.h>
#include <ottos/types.h>
#include <ottos/limits.h>

#define PROCESS_STACK_START_ADDRESS  0x8200A000
#define PROCESS_STACK_SIZE           0x00010000

enum ProcessState {
  READY, BLOCKED, RUNNING
};

typedef struct {
    int CPSR;
    int restart_address;
    int R0;
    int R1;
    int R2;
    int R3;
    int R4;
    int R5;
    int R6;
    int R7;
    int R8;
    int R9;
    int R10;
    int R11;
    int R12;
    int R13;
    int R14;
} pcb_t;

struct process {
    pid_t pid;
    int priority;
    enum ProcessState state;
    pcb_t pcb;

    // TODO is child the right name?
    struct process* child;
    // TODO is parent the right name?
    struct process* parent;
};

typedef struct process process_t;

// the process table contains all processes of the
// operating system
EXTERN process_t* process_table[PROCESS_MAX_COUNT];

// helper variable to find the next free entry in the process table
EXTERN int process_next_free_entry;

// helper variable to identify the active process (state = running)
EXTERN int process_active;

// initializes the process table with NULL values
void process_table_init();

// creates a new process and returns the pid of it
pid_t process_create(int priority, int initial_address);

// deletes the active process
void process_delete();

#endif /* PROCESS_C_H_ */
