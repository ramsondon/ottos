/*
 * Scheduler_c.c
 *
 *  Created on: 07.11.2011
 *      Author: Thomas
 */

#include "../pm/process.h"
#include "scheduler.h"

void scheduler_next() {

	int i = 0;
	// scheudle round robin
	for (i = (process_active + 1) % PROCESS_MAX_COUNT; i < PROCESS_MAX_COUNT; i
			= (i + 1) % PROCESS_MAX_COUNT) {

		if (process_table[i] != NULL && process_table[i]->state == READY) {
			process_active = i;
			return;
		}
	}
}
