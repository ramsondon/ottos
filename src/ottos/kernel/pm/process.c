/*
 * Process_c.c
 *
 *  Created on: 07.11.2011
 *      Author: Thomas
 */

#include <stdlib.h>

#include "process.h"

// the process table contains all processes of the
// operating system
process_t* process_table[PROCESS_MAX_COUNT];
// helper variable to find the next free entry in the process table
int process_next_free_entry = 0;
// helper variable to identify the active process (state = running)
int process_active = -1;

void process_update_next_free_entry() {
	int i = 0;
	for(i = 0; i < PROCESS_MAX_COUNT; i++) {
		if(process_table[i] == NULL) {
			process_next_free_entry = i;
			return;
		}
	}
}

void process_table_init() {
	int i = 0;
	for(i = 0; i < PROCESS_MAX_COUNT; i++) {
		process_table[i] = NULL;
	}
}

pid_t process_create(int priority, int initial_address) {

	process_t* p = (process_t*) malloc(sizeof(process_t));
	p->pid = process_next_free_entry;
	p->priority = priority;
	p->initial_address = initial_address;
	p->started = FALSE;
	p->state = READY;

	// set new stack frame
	p->stack_pointer = PROCESS_STACK_START_ADDRESS + p->pid * PROCESS_STACK_SIZE;

	process_table[p->pid] = p;

	// find the next free entry in the process table
	process_update_next_free_entry();

	return p->pid;
}
