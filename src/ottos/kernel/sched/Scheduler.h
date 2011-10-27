/* Scheduler.h
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
 *  Created on: Oct 21, 2011
 *      Author: Matthias Schmid <m.schmid@students.fhv.at>
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "../pm/Process.h"
#include "RoundRobin.h"


/* class Scheduler
 *
 * schedules the current processes.
 *
 */
class Scheduler {
  public:
    /* constructor */
    Scheduler();
    /* destructor */
    ~Scheduler();
    /* gets the next proc to be executed. does not change any states in procs.*/
    int next(Process** procs, int size);
    /* returns the index of the current proc */
    int current(void);

  private:
    /* the current process index */
    int current_;
    /* internal scheduling algorithm */
    RoundRobin algorithm_;
};

#endif /* SCHEDULER_H_ */
