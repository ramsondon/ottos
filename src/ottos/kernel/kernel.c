/* kernel.c
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
 *  Created on: 11.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include <ottos/kernel.h>
#include <ottos/io.h>
#include <ottos/drivers/driver.h>
#include <ottos/dev/device.h>

void kernel_panic(const char* str) {
  kernel_print("**** KERNEL PANIC ****\n\r");
  kernel_print(str);
  kernel_print("**** KERNEL PANIC ****\n\r");

  kernel_halt();
}

void kernel_print(const char* str) {
//  // TODO(fdomig@gmail.com) Has to be refactored to use an own printf()
  driver_t serial_driver = driver_get(SERIAL_0);
  const char * p = str;
  int len = 0;
  serial_driver.create(SERIAL_0);

  // TODO(fdomig@gmail.com): replace through strlen implementation
  while(*p) p++;
  len = p - str;
  serial_driver.write(SERIAL_0, len, (char*)str);
}

void kernel_halt() {
  for(;;);
}
