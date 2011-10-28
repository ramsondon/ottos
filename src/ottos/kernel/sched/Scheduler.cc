/* Scheduler.cpp
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
 *s
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *
 *  Created on: Oct 21, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <ottos/limits.h>
#include <ottos/const.h>
#include <ottos/types.h>

#include "Scheduler.h"

#include "../pm/Process.h"
#include "../pm/ProcessManager.h"

Scheduler::Scheduler(ProcessManager* process_manager) {
  current_ = -1; /* no proc has started yet; current_ is -1 */
  process_manager_ = process_manager;
}

Scheduler::~Scheduler() {
}

void Scheduler::init() {

}

pid_t Scheduler::next() {

  // get process table
  Process** process_table = process_manager_->process_table();

  // schedule round robin
  for (int i = (current_ + 1) % PROCESS_MAX_COUNT; i < PROCESS_MAX_COUNT; i
      = (i + 1) % PROCESS_MAX_COUNT) {

    // if next process is ready
    if (process_table[i] != 0 && process_table[i]->state() == READY) {
      current_ = i;
      return process_table[current_]->pid();
    }
  }

  // the next process could not be determined
  return PID_INVALID;
}

void Scheduler::run() {

  while (true) {

    pid_t proc = next();
    process_manager_->switch_process(proc);

  }
}

pid_t Scheduler::current(void) {
  return process_manager_->process_table()[current_]->pid();
}
