/* console.c
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
 *  Created on: 29 Nov 2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

//#include <stdio.h>
//#include <stdlib.h>
#include <cstring>

//#include <ottos/drivers/driver.h>
//#include <ottos/memory.h>

#include "../drivers/serial/serial.h"

//#include <ottos/system.h>



int console_start() {

  driver_t drv = driver_get(SERIAL_0);
  char* display_text;
  char buffer[255];
  int buffer_length;

  drv.create(SERIAL_0);

  display_text = "\r\nOttos Console\r\n";
  drv.write(SERIAL_0, strlen(display_text), display_text);

  while (1) {

    display_text = "\r\n$: ";
    drv.write(SERIAL_0, strlen(display_text), display_text);

    // get command
    buffer_length = drv.read(SERIAL_0, 255, buffer);
    buffer[buffer_length] = '\0';

    if(strcmp("exit\r", buffer) == 0) {
      display_text = "\r\nBye Bye (wave)\r\n";
      drv.write(SERIAL_0, strlen(display_text), display_text);
      break;
    }
  }
  return 0;
}
