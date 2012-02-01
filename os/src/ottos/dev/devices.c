/* devices.c
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

#include <stdlib.h>
#include <ottos/limits.h>
#include <ottos/dev/device.h>
#include <ottos/memory.h>
#include <ottos/types.h>

#include "devices.h"


#include "../../drivers/led/led.h"
#include "../../drivers/serial/serial.h"
#include "../../drivers/video/video.h"
#include "../../drivers/sensors/avr.h"
#include "../../drivers/rtc/rtc.h"

/*
 * instantiated devices; managed by this module
 */
static device_map_entry_t* device_container[DEVICE_MAX_COUNT];

/*internal init functions */
static int devices_create(device_t dev, driver_t driver);

/*
 * Initializes all device_t
 */
void devices_init() {

  // initialize the device map with zero values
  ARRAY_INIT(device_container, DEVICE_MAX_COUNT, NULL);

  /* initialize all devices */
  // TODO(ramsondon@gmail.com) insert init functions for devices (!!REFACTOR!!)
  devices_create(LED_0, omap_led_driver);
  devices_create(LED_1, omap_led_driver);
  devices_create(SERIAL_0, omap_serial_driver);
  devices_create(VIDEO_0, omap_video_driver);

  // sensor drivers
  devices_create(TEMP_0, avr_temp_driver);
  devices_create(PRESSURE_0, avr_pressure_driver);
  devices_create(SOLAR_0, avr_solar_driver);

  devices_create(RTC_0, rtc_driver);
}

driver_t* devices_driver(device_t dev) {
  return &(device_container[dev]->driver);
}

/*
 * Creates a new device_map_entry_t
 */
static int devices_create(device_t dev, driver_t driver) {

  device_map_entry_t* entry = malloc(sizeof(device_map_entry_t));
  entry->dev = dev;
  entry->driver = driver;

  if (device_container[dev] != NULL) {
    return FALSE;
  }
  device_container[dev] = entry;
  return TRUE;
}
