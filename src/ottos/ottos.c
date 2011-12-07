/* ottos.cc
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
 *  Created on: Oct 19, 2011
 *      Author: Matthias Schmid <m.schmid@students.fhv.at>
 */

#include <ottos/system.h>
#include <ottos/kernel.h>

#include "../../bin/led_test.h"
#include "../../bin/serial_test.h"
#include "../../bin/console.h"

#include "kernel/intc/irq.h"
#include "kernel/pm/process.h"
#include "kernel/timer/timer.h"
#include "dev/devices.h"
#include "../hal/uart.h"

//#include "kernel/mmu/mmu.h"



int main () {
//  MMU_init();
//  initMemoryForTask(0);
//  initMemoryForTask(1);
//  *(address)0x1004 = 5;

  for(;;);

  return 0;
}
