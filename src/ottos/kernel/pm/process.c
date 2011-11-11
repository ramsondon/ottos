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
