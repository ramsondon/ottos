/* device_manager.h
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

#ifndef DEVICES_H_
#define DEVICES_H_

#include <ottos/const.h>
#include <ottos/dev/device.h>
#include <ottos/drivers/driver.h>

typedef struct device_map_entry_t {
    device_t dev;
    driver_t driver;
} device_map_entry_t;


/* initialize all available devices */
EXTERN void devices_init();

/* returns the driver_t for device_t */
EXTERN driver_t devices_driver(device_t dev);

#endif /* DEVICES_H_ */
