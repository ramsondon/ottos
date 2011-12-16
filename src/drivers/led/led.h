/* led.h
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

#ifndef DRIVERS_LED_LED_H_
#define DRIVERS_LED_LED_H_

#include <ottos/drivers/driver.h>
#include <ottos/dev/device.h>

int led_open(device_t dev);
int led_close(device_t dev);
int led_read(device_t dev, int count, char* buffer);
int led_write(device_t dev, int count, char* buffer);
int led_ioctl(device_t dev, ioctl_t msg);
int led_create(device_t dev);

static driver_t omap_led_driver = {
  led_open,
  led_close,
  led_read,
  led_write,
  led_ioctl,
  led_create
};

#endif /* LED_H_ */
