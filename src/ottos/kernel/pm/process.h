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
#include <ottos/code.h>

#define PROCESS_MEMORY_START 0x20000
#define PROCESS_MEMORY_END 0x1000000

#define PROCESS_STACK_START 0x10000000
#define PROCESS_STACK_SIZE  0x00002000

#define PROCESS_SYSMEM_START 0x10002000
#define PROCESS_SYSMEM_SIZE 0x00002000

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

struct process;
typedef struct process process_t;

struct process {
    address master_table_address;
    address code_location;
    int page_count;

    pid_t pid;
    int priority;
    enum ProcessState state;
    pcb_t pcb;

    process_t* child;
    process_t* parent;
};

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
pid_t process_create(int priority, code_bytes_t* code_bytes);

// deletes the active process
void process_delete();

#endif /* PROCESS_C_H_ */
