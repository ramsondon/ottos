/* serial_test.c
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
 *  Created on: Nov 19, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>

#include <ottos/drivers/driver.h>
#include <ottos/memory.h>

#include "../drivers/serial/serial.h"

#include <ottos/system.h>

static driver_t serial_test_serial_test_driver;

int serial_test_create(void) {
  serial_test_serial_test_driver = driver_get(SERIAL_0);
  serial_test_serial_test_driver.create(SERIAL_0);
  return 1;
}

void serial_test_start_msg() {
  char* buffer = "\n\rWelcome to Serial Driver Test\n\r\0\0\0\0\0\0\0\0\0";
  serial_test_serial_test_driver.write(SERIAL_0,40, buffer);
}


int serial_test_communicate(void) {

  char buffer[1];
  ARRAY_INIT(buffer, 1, NULL);

  serial_test_serial_test_driver.read(SERIAL_0,1, buffer);
  switch(buffer[0]) {
    // The Enter Button will send a \r carriage return, but we want a newline
    case '\r':
      serial_test_serial_test_driver.write(SERIAL_0,2,"\r\n");
    default:
      serial_test_serial_test_driver.write(SERIAL_0,1,buffer);
  }

  return 1;
}

int serial_test_test() {
  serial_test_create();
  serial_test_start_msg();
  while(1) {
    serial_test_communicate();
  }
  return 0;
}

int serial_test_test_yield() {
  serial_test_create();
  serial_test_start_msg();
  while(1) {
    serial_test_communicate();
    sys_yield();
  }
  return 0;
}



int serial_test_write_1() {
  int i;
  driver_t drv = driver_get(SERIAL_0);
  drv.create(SERIAL_0);
  drv.open(SERIAL_0);
  for(i = 0;; i++) {
   if(i > 300000) {
     drv.write(SERIAL_0, 8, "PID: 1\n\r");
     i = 0;
   }
  }
  drv.close(SERIAL_0);
  return 0;
}

int serial_test_write_2() {
  int i;
  driver_t drv = driver_get(SERIAL_0);
  drv.create(SERIAL_0);
  drv.open(SERIAL_0);
  for(i = 0;; i++) {
   if(i > 300000) {
     drv.write(SERIAL_0, 8, "PID: 2\n\r");
     i = 0;
   }
  }
  drv.close(SERIAL_0);
  return 0;
}

int serial_test_write_3() {
  int i;
  driver_t drv = driver_get(SERIAL_0);
  drv.create(SERIAL_0);
  drv.open(SERIAL_0);
  for(i = 0;; i++) {
   if(i > 300000) {
     drv.write(SERIAL_0, 8, "PID: 3\n\r");
     i = 0;
   }
  }
  drv.close(SERIAL_0);
  return 0;
}
int serial_test_write_4() {
  int i;
  driver_t drv = driver_get(SERIAL_0);
  drv.create(SERIAL_0);
  drv.open(SERIAL_0);
  for(i = 0;; i++) {
   if(i > 300000) {
     drv.write(SERIAL_0, 8, "PID: 4\n\r");
     i = 0;
   }
  }
  drv.close(SERIAL_0);
  return 0;
}

int serial_test_write_5() {
  int i;
  driver_t drv = driver_get(SERIAL_0);
  drv.create(SERIAL_0);
  drv.open(SERIAL_0);
  for(i = 0;; i++) {
   if(i > 300000) {
     drv.write(SERIAL_0, 8, "PID: 5\n\r");
     i = 0;
   }
  }
  drv.close(SERIAL_0);
  return 0;
}

int serial_test_calculator() {
  driver_t drv = driver_get(SERIAL_0);
  char buffer[1];
  int a = 0;
  int b = 0;
  int res = 0;
  drv.create(SERIAL_0);
  while (1) {
    drv.write(SERIAL_0, 32, "\n\rOttos Serial Test Calculator\n\r");

    drv.write(SERIAL_0, 13, "\n\rEnter Op1:");
    drv.read(SERIAL_0, 1, buffer);
    a = atoi(buffer);
    drv.write(SERIAL_0, 1, buffer);

    drv.write(SERIAL_0, 13, "\n\rEnter Op2:");
    drv.read(SERIAL_0, 1, buffer);
    b = atoi(buffer);
    drv.write(SERIAL_0, 1, buffer);
    res = a + b;
    sprintf(buffer, "%d", res);
    drv.write(SERIAL_0, 23, "\n\rResult of Op1 + Op2: ");
    drv.write(SERIAL_0, 1, buffer);
  }
  return 0;
}

int serial_test_write_exit_1() {
  int i;
  driver_t drv = driver_get(SERIAL_0);
  drv.create(SERIAL_0);
  drv.open(SERIAL_0);
//  for(i = 0; i < 1000000; i++) {
//   if(i > 300000) {
     drv.write(SERIAL_0, 8, "exit_1\n\r");
//   }
//  }
  drv.close(SERIAL_0);
  return 0;
}

int serial_test_write_exit_2() {
  int i;
  driver_t drv = driver_get(SERIAL_0);
  drv.create(SERIAL_0);
  drv.open(SERIAL_0);
//  for(i = 0; i < 310000; i++) {
//   if(i > 300000) {
     drv.write(SERIAL_0, 8, "exit_2\n\r");
//   }
//  }
  drv.close(SERIAL_0);
  return 0;
}
