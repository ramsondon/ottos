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

#ifndef OTTOS_KERNEL_PM_PROCESS_H_
#define OTTOS_KERNEL_PM_PROCESS_H_

#include <ottos/const.h>
#include <ottos/types.h>
#include <ottos/limits.h>
#include <ottos/code.h>

#define PROCESS_MEMORY_START 0x20000
#define PROCESS_MEMORY_END 0x1000000

#define PROCESS_STACK_START 0x10000000
#define PROCESS_STACK_SIZE  0x00002000

#define PROCESS_SYSMEM_START 0x10002000
#define PROCESS_SYSMEM_SIZE 0x00004000

enum ProcessState {
  READY, BLOCKED, RUNNING
};

enum BlockState {
  NONE, IPC_WAIT
};

// TODO find a better name for this
struct process_file_descriptor;
typedef struct process_file_descriptor process_file_descriptor_t;
struct process_file_descriptor {
    int fd;
    void* file;
    enum system_file_type type;
    process_file_descriptor_t* next;
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

    const char* cmd; /* the command line for this process (proc name)*/
    uint64_t starttime;

    pid_t pid;
    int priority;
    enum ProcessState state;
    enum BlockState blockstate;
    pcb_t pcb;

    process_file_descriptor_t* open_file_list;

    process_t* child;
    process_t* parent;

    int argc;
    char** argv;
};

// the process table contains all processes of the
// operating system
EXTERN process_t* process_table[PROCESS_MAX_COUNT];

// helper variable to find the next free entry in the process table
EXTERN int process_next_free_entry;

// helper variable to identify the active process (state = running)
EXTERN int process_active;

// initializes the process table with NULL values
EXTERN void process_table_init();

// creates a new process and returns the pid of it
pid_t process_create(int priority, code_bytes_t* code_bytes, const char* cmd, int argc, char** argv);

// deletes the active process
EXTERN void process_delete();

// returns the process file descriptor for the given file descriptor id
EXTERN process_file_descriptor_t* process_get_file_descriptor(int fd);

// creates a new process file descriptor and returns it
EXTERN process_file_descriptor_t* process_add_device_descriptor(int fd);

// creates a new process file descriptor and returns it
EXTERN process_file_descriptor_t* process_add_file_descriptor(void* file);

// removes the process file descriptor for the given file descriptor id
EXTERN void process_remove_file_descriptor(int fd);

// returns the current process pid
EXTERN pid_t process_pid();

// sets the ProcessState of process with pid_t pid to BLOCKED
EXTERN void process_block(pid_t pid);

// sets the process to READY state and blockstate to NONE
EXTERN void process_unblock(pid_t pid);

EXTERN int process_count();

// returns the number of actual read pinfo_ts
EXTERN unsigned int process_pinfo(pinfo_t pinfo[], int count);

#endif /* OTTOS_KERNEL_PM_PROCESS_H_ */
