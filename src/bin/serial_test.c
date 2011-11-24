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

#include <ottos/drivers/driver.h>
#include <ottos/memory.h>

#include "../drivers/serial/serial.h"

static driver_t serial_test_serial_test_driver;

int serial_test_create(void) {
  serial_test_serial_test_driver = driver_get(SERIAL_0);
  serial_test_serial_test_driver.create(SERIAL_0);
  return 1;
}

void serial_test_start_msg() {
  char* buffer = "Welcome\n";
  serial_test_serial_test_driver.write(SERIAL_0,8, buffer);
}


int serial_test_communicate(void) {

  char buffer[1];
  ARRAY_INIT(buffer, 1, NULL);

  serial_test_serial_test_driver.read(SERIAL_0,1, buffer);
  switch(buffer[0]) {
    // The Enter Button will send a \r carriage return, but we want a newline
    case '\r':
      serial_test_serial_test_driver.write(SERIAL_0,1,"\n");
    default:
      serial_test_serial_test_driver.write(SERIAL_0,1,buffer);
  }

  return 1;
}
