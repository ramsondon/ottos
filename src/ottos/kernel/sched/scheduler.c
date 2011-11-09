/*
 * Scheduler_c.c
 *
 *  Created on: 07.11.2011
 *      Author: Thomas
 */

#include "../pm/process.h"
#include "scheduler.h"

void schedule_next() {

	static int current = -1;

	int i = 0;
	// scheudle round robin
	for (i = (active_process + 1) % PROCESS_MAX_COUNT; i < PROCESS_MAX_COUNT; i
			= (i + 1) % PROCESS_MAX_COUNT) {

		if (process_table[i] != NULL && process_table[i]->state == READY) {
			active_process = i;
			return;
		}
	}
}
