/*
 * Process_c.h
 *
 *  Created on: 07.11.2011
 *      Author: Thomas
 */

#ifndef PROCESS_C_H_
#define PROCESS_C_H_

#include <ottos/const.h>
#include <ottos/types.h>
#include <ottos/limits.h>

#define PROCESS_STACK_START_ADDRESS 0x8200A000
#define PROCESS_STACK_SIZE 0x00010000

enum ProcessState {
  READY, BLOCKED, RUNNING
};

typedef struct {
    pid_t pid;
    int priority;
    enum ProcessState state;
    // the initial address of the process (entry point, function pointer)
    int initialAddress;
    int stack_pointer;

    int started;
} Process_t;

// the process table contains all processes of the
// operating system
EXTERN Process_t* process_table[PROCESS_MAX_COUNT];
// helper variable to find the next free entry in the process table
EXTERN int next_free_entry;
// helper variable to identify the active process (state = running)
EXTERN int active_process;

// initializes the process table with NULL values
void init_process_table();

// creates a new process and returns the pid of it
pid_t create_process(int priority, int initialAddress);

#endif /* PROCESS_C_H_ */
