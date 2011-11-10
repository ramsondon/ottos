/* devmanager.c
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

#include <ottos/limits.h>
#include <ottos/dev/device.h>

#include "devices.h"

#include "../../drivers/led/led.h"

/*
 * instantiated devices; managed by this module
 */
static device_map_entry_t device_container[DEVICE_MAX_COUNT];

/*internal init functions */
static device_map_entry_t devices_create(device_t device, driver_t driver);
static void device_led_init();

/*
 * Initializes all device_t
 */
void devices_init() {

  // initialize the device map with zero values
  int i;
  for (i = 0; i < DEVICE_MAX_COUNT; i++) {
    device_container[i].dev = DEVICE_INVALID;
    device_container[i].driver = driver_null();
  }

  /* initialize all devices */
  // TODO: insert init functions for devices
  device_led_init();

}

driver_t devices_driver(device_t dev) {
  return device_container[dev].driver;
}

/*
 * creates the led driver and initializes the device map entries
 */
void device_led_init() {

  // register led driver
  driver_t led_driver;
  led_driver.open = led_open;
  led_driver.close = led_close;
  led_driver.create = led_create;
  led_driver.ioctl = led_ioctl;
  led_driver.read = led_read;
  led_driver.write = led_write;

  device_container[LED_0] = devices_create(LED_0, led_driver);
  device_container[LED_1] = devices_create(LED_1, led_driver);
}

/*
 * Creates a new device_map_entry_t
 */
device_map_entry_t devices_create(device_t device, driver_t driver) {

  device_map_entry_t entry;
  entry.dev = device;
  entry.driver = driver;
  return entry;
}


