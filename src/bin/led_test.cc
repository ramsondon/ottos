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

#include "led_test.h"

int toggle_led1() {
  volatile int i;

  *(volatile unsigned long *)GPIO5_OE |= SET_BIT(LED_DEVICE_USR0);

  for (i = 0;; i++) {
    if (i > 100000) {
      i = 0;

      // switch led 1
      *(volatile unsigned long *) GPIO5_DATAOUT ^= SET_BIT(LED_DEVICE_USR0);

      sched_yield();
    }
  }

  return 0;
}

int toggle_led2() {
  volatile int i;

  *(volatile unsigned long *)GPIO5_OE |= SET_BIT(LED_DEVICE_USR1);

  for (i = 0;; i++) {
    if (i > 100000) {
      i = 0;

      // switch led 2
      *(volatile unsigned long *) GPIO5_DATAOUT ^= SET_BIT(LED_DEVICE_USR1);

      sched_yield();
    }
  }

  return 0;
}
