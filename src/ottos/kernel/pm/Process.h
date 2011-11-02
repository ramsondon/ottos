/* Process.h
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
 *  Created on: 21.10.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <ottos/types.h>

enum ProcessState {
  READY, BLOCKED, RUNNING
};

class Process {
  public:
    Process();
    virtual ~Process();

    pid_t pid();
    void set_pid(pid_t pid);
    int priority();
    void set_priority(int priority);
    ProcessState state();
    void set_state(ProcessState state);
    function_t func();
    void set_func(function_t function);

    int* registers();
    void set_registers(int* registers);

    int executed();
    void mark_as_executed();

    int stack_pointer();
    void set_stack_pointer(int sp);

  private:
    pid_t pid_;
    int priority_;
    ProcessState state_;

    int registers_[16];

    function_t func_;

    int executed_;

    int stack_pointer_;
};

#endif /* PROCESS_H_ */
