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
 *      Author: Matthias Schmid <m.schmid@students.fhv.at>
 */

#include "Scheduler.h"
#include "../pm/Process.h"
#include "RoundRobin.h"

Scheduler::Scheduler() {
  current_ = -1;            /* no proc has started yet; current_ is -1 */
  algorithm_ = RoundRobin();
}

Scheduler::~Scheduler() {
}

int Scheduler::next(Process** procs, int size) {
  current_ = algorithm_.next(procs, current_, size);
  return current_;
}

int Scheduler::current(void) {
  return current_;
}
