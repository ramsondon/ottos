/*
 * Process_c.c
 *
 *  Created on: 07.11.2011
 *      Author: Thomas
 */

#include <stdlib.h>

#include "Process_c.h"

// the process table contains all processes of the
// operating system
Process_t* process_table[PROCESS_MAX_COUNT];
// helper variable to find the next free entry in the process table
int next_free_entry = 0;
// helper variable to identify the active process (state = running)
int active_process = -1;

void update_next_free_entry() {

	int i = 0;
	for(i = 0; i < PROCESS_MAX_COUNT; i++) {
		if(process_table[i] == NULL) {
			next_free_entry = i;
			return;
		}
	}
}

void init_process_table() {

	int i = 0;
	for(i = 0; i < PROCESS_MAX_COUNT; i++) {
		process_table[i] = NULL;
	}
}

pid_t create_process(int priority, int initialAddress) {

	Process_t* p = (Process_t*) malloc(sizeof(Process_t));
	p->pid = next_free_entry;
	p->priority = priority;
	p->initialAddress = initialAddress;
	p->started = FALSE;
	p->state = READY;

	// set new stack frame
	p->stack_pointer = PROCESS_STACK_START_ADDRESS + p->pid * PROCESS_STACK_SIZE;

	process_table[p->pid] = p;

	// find the next free entry in the process table
	update_next_free_entry();

	return p->pid;
}
