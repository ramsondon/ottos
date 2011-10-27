/* ProcessManager.h
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

#ifndef PROCESSMANAGER_H_
#define PROCESSMANAGER_H_

#include <ottos/types.h>
#include <ottos/limits.h>

class Process;

class ProcessManager {
  public:
    ProcessManager();
    virtual ~ProcessManager();

    int run(function_t function);
    int switchProcess(pid_t to);
    pid_t add(Process* proc);
    void init();


  private:
    pid_t current_;

    // the pid_t of a process is the index in the process table
    Process* process_table_[PROCESS_MAX_COUNT];
};

#endif /* PROCESSMANAGER_H_ */
