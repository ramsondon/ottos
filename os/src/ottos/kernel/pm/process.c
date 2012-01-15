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

#include <ottos/const.h>
#include <ottos/drivers/driver.h>

#include "../intc/irq.h"
#include "../sched/scheduler.h"
#include "../mmu/mmu.h"
#include "../loader/loader.h"

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

void process_close_open_files(process_t* process) {
	process_file_descriptor_t* current = process->open_file_list;
	process_file_descriptor_t* next = NULL;

	while (current != NULL) {
		switch (current->type) {
		case DEVICE_FILE:
			// the file descriptor is the device id
			driver_get(current->fd)->close(current->fd);
			break;
		case NON_DEVICE_FILE:
			// TODO close the file
			break;
		}

		next = current->next;
		free(current);
		current = next;
	}
}

void process_delete() {

	if (process_active == PID_INVALID) {
		return;
	}

	if (process_table[process_active]->parent != NULL) {

		// remove child from parent
		process_table[process_active]->parent->child = NULL;

		// unblock parent
		if (process_table[process_active]->parent->state == BLOCKED) {
			process_table[process_active]->parent->state = READY;
		}
	}

	// close all open files of the process
	process_close_open_files(process_table[process_active]);

	//delete mastertable entries for process
	mmu_delete_process_memory(process_table[process_active]);

	// delete the process
	free(process_table[process_active]);

	// remove the active process from process table
	process_table[process_active] = NULL;

	process_update_next_free_entry();

	// set the process_active to invalid
	// so the context switch won't save
	// the PCB of the active process (this one)
	process_active = PID_INVALID;
}

pid_t process_create(int priority, code_bytes_t* code_bytes) {

	process_t* p = (process_t*) malloc(sizeof(process_t));
	p->master_table_address = (address) 0;
	p->code_location = (address) 0;

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

	p->pcb.restart_address = PROCESS_MEMORY_START;
	p->pcb.CPSR = 0x80000110;

	// set new stack frame
	p->pcb.R13 = PROCESS_STACK_START + PROCESS_STACK_SIZE;
	p->pcb.R14 = 0;

	// load the process code
	loader_load(p, code_bytes);

	p->open_file_list = NULL;

	process_table[p->pid] = p;

	// find the next free entry in the process table
	process_update_next_free_entry();

	return p->pid;
}

process_file_descriptor_t* process_get_file_descriptor(int fd) {

	if (process_active != PID_INVALID) {
		process_file_descriptor_t* current = process_table[process_active]->open_file_list;
		while (current != NULL) {
			if (current->fd == fd) {
				return current;
			}
			current = current->next;
		}
	}

	return NULL;
}

void process_create_file_descriptor(process_file_descriptor_t* fd_process, int fd, void* file, enum system_file_type file_type) {
	fd_process->fd = fd;
	fd_process->file = file;
	fd_process->type = file_type;
	fd_process->next = NULL;
}

process_file_descriptor_t* process_add_file_descriptor_(int fd, void* file, enum system_file_type file_type) {

	if (process_active != PID_INVALID && process_get_file_descriptor(fd) == NULL) {
		process_file_descriptor_t* current = process_table[process_active]->open_file_list;
		process_file_descriptor_t* previous = NULL;

		while (current != NULL) {
			if (current->next == NULL) {
				previous = current;
			}
			current = current->next;
		}

		current = malloc(sizeof(process_file_descriptor_t));
		process_create_file_descriptor(current, fd, file, file_type);
		if (previous != NULL) {
			previous->next = current;
		} else {
			// head of list
			process_table[process_active]->open_file_list = current;
		}

		return current;

	}

	return NULL;
}

process_file_descriptor_t* process_add_device_descriptor(int fd) {

	return process_add_file_descriptor_(fd, NULL, DEVICE_FILE);
}

#define PROCESS_START_FD	100
static int process_get_next_fd() {
	int i = 0;

	process_file_descriptor_t* current = process_table[process_active]->open_file_list;
	while(current != NULL) {
		if(current->type == NON_DEVICE_FILE) {
			i++;
		}
		current = current->next;
	}

	return i + PROCESS_START_FD;
}

process_file_descriptor_t* process_add_file_descriptor(void* file) {

	return process_add_file_descriptor_(process_get_next_fd(), file, NON_DEVICE_FILE);
}

void process_remove_file_descriptor(int fd) {

	if (process_active != PID_INVALID) {
		process_file_descriptor_t* current = process_table[process_active]->open_file_list;
		process_file_descriptor_t* previous = NULL;

		while (current != NULL) {
			if (current->fd == fd) {
				break;
			}

			previous = current;
			current = current->next;
		}

		if (current != NULL) {
			if (previous != NULL) {
				previous->next = current->next;
			} else {
				// head of list
				process_table[process_active]->open_file_list = NULL;
			}
			free(current->file);
			free(current);
		}
	}
}