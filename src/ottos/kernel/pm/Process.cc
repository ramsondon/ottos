/* Process.cpp
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

#include <ottos/const.h>

#include "Process.h"

Process::Process() {
  executed_ = 0;
}

Process::~Process() {

}

pid_t Process::pid()
{
  return pid_;
}



function_t Process::func()
{
  return func_;
}



int Process::priority()
{
  return priority_;
}



void Process::set_func(function_t function)
{
  func_ = function;
}



void Process::set_state(ProcessState state)
{
  state_ = state;
}



void Process::set_priority(int priority)
{
  priority_ = priority;
}



ProcessState Process::state()
{
  return state_;
}



void Process::set_pid(pid_t pid)
{
  pid_ = pid;
}

int *Process::registers()
{
  return registers_;
}



void Process::set_registers(int *registers)
{
  // copy the array
  for(int i = 0; i < 16; i++) {
    registers_[i] = registers[i];
  }
}

int Process::executed()
{
  return executed_;
}



void Process::mark_as_executed()
{
  executed_ = 1;
}

int Process::stack_pointer()
{
  return stack_pointer_;
}



void Process::set_stack_pointer(int sp)
{
  stack_pointer_ = sp;
}






