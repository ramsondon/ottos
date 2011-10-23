/* RoundRobin.cc
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
 *  Created on: Oct 23, 2011
 *      Author: Matthias Schmid <m.schmid@students.fhv.at>
 */

#include <ottos/const.h>

#include "RoundRobin.h"
#include "../pm/Process.h"


RoundRobin::RoundRobin() {
}

RoundRobin::~RoundRobin() {
}

int RoundRobin::next(Process** procs, int cur_proc, int nr_of_procs) {

  // schedule all procs as a circular array
  for (int i=(cur_proc+1)%nr_of_procs; i < nr_of_procs; i=(i+1)%nr_of_procs) {

    // if next process is ready
    if (procs[i] != 0 && procs[i]->state == READY) {
      return i;
    }
  }
  return kInvalidProcess;
}
