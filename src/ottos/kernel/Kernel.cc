/* Kernel.cpp
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

#include <ottos/const.h>

#include "sched/Scheduler.h"
#include "pm/ProcessManager.h"
#include "sys/SyscallHandler.h"

#include "Kernel.h"


Kernel::Kernel() {
}

Kernel::~Kernel() {
}

void Kernel::init()
{
  // TODO(thomas.bargetz@gmail.com) create kernel space and user space in memory

  // create the process manager
  *process_manager = ProcessManager();
  process_manager->init();

  // create the scheduler
  //*scheduler = Scheduler(process_manager);
  // scheduler->init();

  // create the system call handler
  *syscall_handler = SyscallHandler();
}

void Kernel::run()
{
  // TODO(thomas.bargetz@gmail.com) create init process
  Process* p = new Process(PID_INVALID);
  process_manager->add(p);

  // start scheduling
  // scheduler->next();
}


