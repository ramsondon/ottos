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

#include "../drivers/serial/serial.h"



int serial_test_create(void) {
  char read[1];
  driver_t sd = driver_get(SERIAL_0);
  sd.create(SERIAL_0);
  sd.write(SERIAL_0, 1, "R");

  read[0] = NULL;
  sd.read(SERIAL_0,1, read);
  printf("%c\n", read[0]);

  return 1;
}
