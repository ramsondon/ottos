/* syscalls.cc
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

#include <ottos/system.h>

#pragma INTERRUPT["SWI"]
interrupt void swi_handler(void);

int sched_yield(){
  // generate a systemcall for the scheduler
  // write the SCHEDULER_YIELD systemcall in the register


  //_enable_interrupts();
  //_call_swi(??);

  // switch to kernel mode to handle the systemcall

  // TODO read return register
  return 0;
}

#pragma INTERRUPT["SWI"]
interrupt void swi_handler(void) {
  volatile int i = 0;
  if(i++ > 1) {
    i--;
  }
}
