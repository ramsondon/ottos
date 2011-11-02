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

#include <vector>

#include <ottos/types.h>
#include <ottos/limits.h>

#define SAVE_REGISTERS asm("\tPUSH {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r14}")
#define LOAD_REGISTERS asm("\tPOP {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r14}")

extern int stack_pointer;
extern int old_stack_pointer;
extern int func_pointer;

class Process;

class ProcessManager {
  public:
    ProcessManager();
    virtual ~ProcessManager();

    int run(function_t function);
    int switch_process(pid_t to);
    pid_t current_process(void);
    pid_t add(Process* proc);
    void init();

    std::vector<Process *>* process_table();

  private:
    pid_t current_;
    std::vector<Process *> process_table_;
};

#endif /* PROCESSMANAGER_H_ */
