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
#include <ottos/io.h>

// TODO(ramsondon@gmail.com) remove when swi handles SYS_OPEN
#include <ottos/dev/device.h>
// TODO(ramsondon@gmail.com) remove when swi handles SYS_OPEN
#include <ottos/drivers/driver.h>

#include "led_test.h"

int toggle_led1() {

  int i;
  char buffer;
  char state;
  driver_t drv = driver_get(LED_0);

  drv.open(LED_0);
	for(i = 0;; i++) {
		if(i > 1000000) {

		  drv.read(LED_0, 1, &state);
			buffer = ((int)state == 0)? 1 : 0;
			drv.write(LED_0, 1, &buffer);
		  i = 0;
			sys_yield();
		}
	}
	drv.close(LED_0);
  return 0;
}

int toggle_led2() {

  int i;
  char buffer;
  char state;
  driver_t drv = driver_get(LED_1);

  drv.open(LED_1);
	for(i = 0;; i++) {
		if(i > 100000) {
		  drv.read(LED_1, 1, &state);
      buffer = ((int)state == 0)? 1 : 0;
      drv.write(LED_1, 1, &buffer);
      i = 0;
			sys_yield();
		}
	}
	drv.close(LED_1);
  return 0;
}

