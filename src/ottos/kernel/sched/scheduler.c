/* scheduler.c
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

#include "../ipc/ipc.h"
#include "../pm/process.h"
#include "scheduler.h"

void scheduler_handle_pending_ipc_msg(pid_t pid) {

  if (ipc_lookup_msg_for(pid) == SUCCESS) {
    process_unblock(pid);
  }
}


void scheduler_next() {

	int i = 0;
	// schedule round robin
	for (i = (process_active + 1) % PROCESS_MAX_COUNT; i < PROCESS_MAX_COUNT; i
			= (i + 1) % PROCESS_MAX_COUNT) {

		if (process_table[i] != NULL) {

		  // handle blocked processes
		  if (process_table[i]->state == BLOCKED) {
		    switch(process_table[i]->blockstate) {
		      case IPC_WAIT:
		        scheduler_handle_pending_ipc_msg((pid_t)i);
		        break;
		      default:
		        // strange behaviour - this case should not appear
		        break;
		    }
		  }

		  // choose new process
		  if (process_table[i]->state == READY) {
		    process_active = i;
		    return;
		  }
		}
	}
}
