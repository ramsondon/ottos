/* swi_device_handlers.c
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
 *  Created on: 13 Jan 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <ottos/drivers/driver.h>
#include <ottos/dev/device.h>
#include "swi_device_handlers.h"

BOOLEAN swi_handle_sys_open(int* fd, int device_id, int flags) {
  device_t device = (device_t)device_id;
  driver_get(device).open(device);
  fd = device;
}

BOOLEAN swi_handle_sys_close(int* return_value, int fd);
BOOLEAN swi_hande_sys_read(size_t* read_bytes, int fd, char* buffer, int length);

BOOLEAN swi_handle_sys_write(size_t* written_bytes, int fd, char* buffer, size_t nbytes) {

}
