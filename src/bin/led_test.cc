/* led_1_test.c
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
#include <ottos/syscalls.h>
#include "../ottos/kernel/sys/SyscallHandler.h"

#include "../drivers/leds/LedDriver.h"

#include "led_test.h"

int toggle_led1() {

  LedDriver d = LedDriver(LED_DEVICE_USR0);
  d.open();
  char buffer = (char) 1;

  while (1 == 1) {
    d.write(1, &buffer);
    for (volatile int i = 0; i < 100000; i++) {
    }
    buffer = (buffer == 1) ? 0 : 1;
    swi(SCHEDULER_YIELD);
  }
  d.close();

  return 0;
}

int toggle_led2() {

  LedDriver d = LedDriver(LED_DEVICE_USR1);
  char buffer = (char) 1;
  d.open();

  while (1 == 1) {
    d.write(1, &buffer);
    for (volatile int i = 0; i < 100000; i++) {
    }
    buffer = (buffer == 1) ? 0 : 1;
    swi(SCHEDULER_YIELD);
  }
  d.close();

  return 0;
}
