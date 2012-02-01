/* device.h
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

#ifndef DEVICE_H_
#define DEVICE_H_

#include <ottos/types.h>

#define DEVICE_DRIVER_REGISTER(device_t, driver_t) (devices_create(device_t, driver_t))


typedef int device_t;

/*
 * Device identifiers
 *
 * WARNING: always check the <ottos/limits.h> for DEVICE_MAX_COUNT when adding
 *          new devices.
 */

#define DEVICE_MAX_COUNT 30

#define DEVICE_INVALID  -1

#define GPTIMER_1       0
#define GPTIMER_2       1
#define GPTIMER_3       2
#define GPTIMER_4       3
#define GPTIMER_5       4
#define GPTIMER_6       5
#define GPTIMER_7       6
#define GPTIMER_8       7
#define GPTIMER_9       8
#define GPTIMER_10      9
#define GPTIMER_11      10
#define LED_0           11
#define LED_1           12
#define SERIAL_0        13
#define VIDEO_0         14
#define TEMP_0          15
#define PRESSURE_0      16
#define SOLAR_0         17
#define RTC_0           18

/**
 * External devices
 */
#define DEV_STATUS uint32_t

typedef struct _EXTERNAL_DEVICE EXTERNAL_DEVICE;

typedef DEV_STATUS(*EXTERNAL_DEVICE_READ)(
    EXTERNAL_DEVICE *this,
    uint32_t reg,
    uint32_t length,
    void* buffer);

typedef DEV_STATUS(*EXTERNAL_DEVICE_WRITE)(
    EXTERNAL_DEVICE *this,
    uint32_t reg,
    uint32_t length,
    void* buffer);

struct _EXTERNAL_DEVICE {
    EXTERNAL_DEVICE_READ read;
    EXTERNAL_DEVICE_WRITE write;
};

#endif /* DEVICE_H_ */
