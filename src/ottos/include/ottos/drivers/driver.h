/* driver.h
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

#ifndef DRIVER_H_
#define DRIVER_H_

#include <ottos/const.h>
#include <ottos/dev/device.h>

/*
 * The driver specific I/O control execution flag
 */
typedef int ioctl_t;

/*
 * struct driver_t
 * The device driver interface of the ottOS
 */
typedef struct driver_t {
    int (*open)(device_t dev);
    int (*close)(device_t dev);
    int (*read)(device_t dev, int count, char* buffer);
    int (*write)(device_t dev, int count, char* buffer);
    int (*ioctl)(device_t dev, ioctl_t msg);
    int (*create)(device_t dev);
} driver_t;

/*
 * Returns the driver for device dev
 */
EXTERN driver_t driver_get(device_t dev);


#endif /* DRIVER_H_ */
