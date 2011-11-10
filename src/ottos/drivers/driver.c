/* driver.c
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
 *  Created on: Nov 10, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <ottos/drivers/driver.h>
#include "../dev/devices.h"

/*
 * NULL driver_t stub functions
 */
static int driver_null_open(device_t dev) {
  return FALSE;
}
static int driver_null_close(device_t dev) {
  return FALSE;
}
static int driver_null_read(device_t dev, int count, char* buffer) {
  return FALSE;
}
static int driver_null_write(device_t dev, int count, char* buffer) {
  return FALSE;
}
static int driver_null_ioctl(device_t dev, ioctl_t msg) {
  return FALSE;
}
static int driver_null_create(device_t dev) {
  return FALSE;
}

/*
 * A null driver stub which functions always return FALSE and do nothing.
 * This function is used for device driver requests which device is not defined.
 */
driver_t driver_null() {
  driver_t drv;
  drv.close = driver_null_close;
  drv.create = driver_null_create;
  drv.ioctl = driver_null_ioctl;
  drv.open = driver_null_open;
  drv.read = driver_null_read;
  drv.write = driver_null_write;
  return drv;
}

driver_t driver_driver(device_t dev) {
  return devices_driver(dev);
}
