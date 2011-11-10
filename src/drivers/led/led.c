/* led.c
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

#include <ottos/dev/device.h>
#include <ottos/types.h>

#include "led.h"
#include "../../hal/leds.h"


static int led_device_bit(device_t* dev);


/*
 * Enable data output for led device_t
 */
int led_open(device_t* dev) {

  // determine the led device bit for device_t dev
  int bit = led_device_bit(dev);
  if (bit == DEVICE_INVALID) {
    return FALSE;
  }
  // enable output at gpio5
  *(volatile unsigned long *) GPIO5_OE |= (1 << bit);
  return TRUE;
}

/*
 * Disable data output for led device_t
 */
int led_close(device_t* dev) {

  // determine the led device bit for device_t dev
  int bit = led_device_bit(dev);
  if (bit != DEVICE_INVALID) {
    // disable output at gpio5
    *(volatile unsigned long *) GPIO5_OE &= ~(1 << bit);
    return TRUE;
  }
  return FALSE;
}

int led_read(device_t* dev, int count, char* buffer) {

  int bit = led_device_bit(dev);
  if (bit != DEVICE_INVALID) {
    *buffer = (char) (*(volatile unsigned long *) GPIO5_DATAOUT & (1 << bit)
        ? LED_ON : LED_OFF);
    return TRUE;
  }
  return FALSE;
}

int led_write(device_t* dev, int count, char* buffer) {

  int task = (int) *buffer;
  int bit = led_device_bit(dev);
  if (bit == DEVICE_INVALID) {
    return FALSE;
  }

  switch (task) {
    case LED_ON:
      *(volatile unsigned long *) GPIO5_DATAOUT |= (1 << bit);
      break;
    case LED_OFF:
      *(volatile unsigned long *) GPIO5_DATAOUT &= ~(1 << bit);
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

int led_ioctl(device_t* dev, ioctl_t msg) {
  return FALSE;
}

int led_create(device_t* dev) {
  return FALSE;
}

/*
 * Returns the bit to set for a specific led device or -1 if led does not
 * exist.
 */
int led_device_bit(device_t* dev) {

  if (dev->dev == LED_0) {
    return LED_DEVICE_USR0;
  } else if (dev->dev == LED_1) {
    return LED_DEVICE_USR1;
  }
  return DEVICE_INVALID;
}
