/* Kernel.h
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

#ifndef KERNEL_H_
#define KERNEL_H_

// forward declarations
class Scheduler;
class ProcessManager;
class SyscallHandler;

extern Scheduler* sched;

class Kernel {
  public:
    Kernel();
    virtual ~Kernel();

    void init();
    void run();

    Scheduler* scheduler();
    ProcessManager* process_manager();
    SyscallHandler* syscall_handler();

  private:
    Scheduler* scheduler_;
    ProcessManager* process_manager_;
    SyscallHandler* syscall_handler_;
};

#endif /* KERNEL_H_ */
