/* ProcessManager.cpp
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
 *  Created on: 27 Oct 2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <vector>

#include <ottos/const.h>

#include "Process.h"
#include "ProcessManager.h"

ProcessManager::ProcessManager() {
}

ProcessManager::~ProcessManager() {
}

void ProcessManager::init() {
  process_table_ = std::vector<Process *>(PROCESS_MAX_COUNT);
}

int ProcessManager::run(function_t function)
{
  return -1;
}



int ProcessManager::switch_process(pid_t to)
{
  // TODO(fdomig@gmail.com) must use ATOMIC_START
  // TODO save process relevant data (registers, etc)
  process_table_[current_]->set_state(READY);

  current_ = to;
  process_table_[current_]->set_state(RUNNING);

  process_table_[to]->func()();

  // TODO(fdomig@gmail.com) must use ATOMIC_END
  return 0;
}

pid_t ProcessManager::current_process() {
  return current_;
}

pid_t ProcessManager::add(Process *proc)
{
  // TODO(fdomig@gmail.com) must use ATOMIC_START
  for(int i = 0;i<PROCESS_MAX_COUNT; i++) {
    if (process_table_[i] == NULL) {
      process_table_[i] = proc;
      proc->set_pid(static_cast<pid_t>(i));
      proc->set_state(READY);
      return proc->pid();
    }
  }
  return PID_INVALID;
  // TODO(fdomig@gmail.com) must use ATOMIC_END
}

std::vector<Process *>* ProcessManager::process_table() {
  return &process_table_;
}


