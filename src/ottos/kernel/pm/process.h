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
    int initial_address;
    int stack_pointer;
    int started;
} process_t;

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

#endif /* PROCESS_C_H_ */
