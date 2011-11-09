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

#include "../drivers/leds/LedDriver.h"

#include "led_test.h"

int toggle_led1() {

	*(volatile unsigned long *)GPIO5_OE |= SET_BIT(LED_DEVICE_USR0);
	for(int i = 0;; i++) {
		if(i > 100000) {
			i = 0;

			*(volatile unsigned long *) GPIO5_DATAOUT ^= SET_BIT(LED_DEVICE_USR0);
			sys_yield();
		}
	}

//  LedDriver d = LedDriver(LED_DEVICE_USR0);
//  d.open();
//  char buffer = (char) 1;
//
//  while (1 == 1) {
//    d.write(1, &buffer);
//    for (volatile int i = 0; i < 100000; i++) {
//    }
//    buffer = (buffer == 1) ? 0 : 1;
//    sys_yield();
//
//  }
//  d.close();

//	int i = 5;
//	while(i > 0) {
//		printf("led1: before yield: i=%d\n", i);
//		i--;
//		sys_yield();
//		printf("led1: after yield: i=%d\n", i);
//	}

  return 0;
}

int toggle_led2() {

	*(volatile unsigned long *)GPIO5_OE |= SET_BIT(LED_DEVICE_USR1);
	for(int i = 0;; i++) {
		if(i > 100000) {
			i = 0;

			*(volatile unsigned long *) GPIO5_DATAOUT ^= SET_BIT(LED_DEVICE_USR1);
			sys_yield();
		}
	}
//
//  LedDriver d = LedDriver(LED_DEVICE_USR1);
//  char buffer = (char) 1;
//  d.open();
//
//  while (1 == 1) {
//    d.write(1, &buffer);
//    for (volatile int i = 0; i < 100000; i++) {
//    }
//    buffer = (buffer == 1) ? 0 : 1;
//    sys_yield();
//  }
//  d.close();

//	int i = 10;
//	while(i > 0) {
//		printf("led2: before yield: i=%d\n", i);
//		i--;
//		sys_yield();
//		printf("led2: after yield: i=%d\n", i);
//	}

  return 0;
}

int test_file_operations() {
  file_t* fp = fopen("/dev/led01", "w");
  char buffer = 1;
  fwrite(fp, sizeof(char), 1, &buffer);
  fclose(fp);

  return 0;
}
