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
#include <ottos/limits.h>

enum ProcessState {
  READY, BLOCKED, RUNNING
};

#include <stdio.h>
class Process {
  public:
    Process();
    virtual ~Process();

    inline pid_t pid() { return pid_; }
    inline void set_pid(pid_t pid) { pid_ = pid; }

    inline int priority() { return priority_; }
    inline void set_priority(int priority) { priority_ = priority; }

    inline ProcessState state() { return state_; }
    inline void set_state(ProcessState state) { state_ = state; }

    inline function_t func() { return func_; }
    inline void set_func(function_t function) { func_ = function; }

    inline int* registers() { return registers_; }
    inline void set_registers(int* r) {
      for(int i = 0; i<REGISTER_SIZE; i++) registers_[i] = r[i];
    }

    inline int executed() { return executed_; }
    inline void mark_as_executed() { executed_ = 1; }

    inline int stack_pointer() { return stack_pointer_; }
    inline void set_stack_pointer(int sp) { stack_pointer_ = sp; }

    inline int entry() { return entry_;}
    inline void set_entry(int entry) { entry_ = entry;}

  private:
    pid_t pid_;
    int priority_;
    ProcessState state_;

    int registers_[REGISTER_SIZE];

    function_t func_;

    int executed_;

    int stack_pointer_;

    int entry_;
};

#endif /* PROCESS_H_ */
