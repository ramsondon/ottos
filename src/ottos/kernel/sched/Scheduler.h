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
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <ottos/types.h>

class Process;
class ProcessManager;

/* class Scheduler
 *
 * schedules the current processes.
 *
 */
class Scheduler {
  public:
    /* constructor */
    Scheduler(ProcessManager* process_manager);
    /* destructor */
    ~Scheduler();
    // initializes the Scheduler
    void init(void);
    /* runs the scheduler */
    void run(void);

  private:
    /* the current process index */
    int current_;
    /* internal scheduling algorithm */
    ProcessManager* process_manager_;

    /* gets the next proc to be executed. does not change any states in procs.*/
    pid_t next();
    /* returns the index of the current proc */
    pid_t current(void);
};

#endif /* SCHEDULER_H_ */
