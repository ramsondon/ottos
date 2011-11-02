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
#include "pm/Process.h"
#include "sys/SyscallHandler.h"

#include "../../bin/led_test.h"

#include "Kernel.h"


Kernel::Kernel() {
}

Kernel::~Kernel() {
}

void Kernel::init()
{
  // TODO(thomas.bargetz@gmail.com) create kernel space and user space in memory

  // create the process manager
  process_manager_ = new ProcessManager();
  process_manager_->init();

  // create the scheduler
  scheduler_ = new Scheduler(process_manager_);
  scheduler_->init();

  // create the system call handler
  syscall_handler_ = new SyscallHandler();
}

void Kernel::run()
{
  Process* led1 = new Process();
  led1->set_func(toggle_led1);

  Process* led2 = new Process();
  led2->set_func(toggle_led2);

  process_manager_->add(led1);
  process_manager_->add(led2);

  // TODO(thomas.bargetz@gmail.com) create init process

  // Process* p = new Process();
  // process_manager->add(p);

  // pseudo code for creation of the first process
  //
  // if (fork() == 0) {
  //     switch_to_user_mode();
  //     exit(exec(p->pid));
  // } else {
  //     waitpid(p->pid);
  // }

  // start scheduling
  scheduler_->run();
}

Scheduler *Kernel::scheduler()
{
  return scheduler_;
}



ProcessManager *Kernel::process_manager()
{
  return process_manager_;
}



SyscallHandler *Kernel::syscall_handler()
{
  return syscall_handler_;
}




