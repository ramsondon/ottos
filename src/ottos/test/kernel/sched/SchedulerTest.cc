/* SchedulerTest.cc
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
 *  Created on: Oct 27, 2011
 *      Author: Matthias Schmid <m.schmid@students.fhv.at>
 */

#include "SchedulerTest.h"

#include "../../../../ottos/kernel/sched/Scheduler.h"
#include "../../../../ottos/kernel/sched/RoundRobin.h"

SchedulerTest::SchedulerTest() {

}

SchedulerTest::~SchedulerTest() {
}

void SchedulerTest::run() {

  // FIXME: make array instead of malloc and then uncomment
  // make private variables for that init stuff

//  int MAX_PROC = 5;
//   Process* procs = (Process*)malloc(sizeof(Process)*MAX_PROC);
//   procs[0] = Process(0);
//   procs[0].state = READY;
//   procs[1] = Process(1);
//   procs[1].state = READY;
//   procs[2] = Process(2);
//   procs[2].state = READY;
//   procs[3] = 0;
//   procs[4] = 0;
//   RoundRobin rr;
//   Scheduler sched(rr);
//
//
//   // run first process
//   procs[sched.next(&procs, MAX_PROC)].state = RUNNING;
//
//   while (true) {
//     int running = sched.current();
//     procs[sched.next(&procs, MAX_PROC)].state = RUNNING;
//     procs[running].state = READY;
//     printf("Running %d\n", procs[sched.current()].pid);
//   }
}
