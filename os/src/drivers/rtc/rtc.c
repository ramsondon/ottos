/* rtc.c
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
 *  Created on: 1 Feb 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <ottos/memory.h>
#include <ottos/types.h>
#include <ottos/error.h>
#include <ottos/const.h>
#include <ottos/kernel.h>

#include "../hal/omap353x/rtc.h"
#include "../hal/omap353x/i2c.h"
#include "../drivers/i2c/i2c.h"

#include "rtc.h"

#define MUX_GPIO1    0x480025EC

static BOOLEAN initialized = FALSE;

int rtc_open_(device_t dev) {
  if (initialized == FALSE) {
    i2c_write8(I2C1_BASE, 0x4b, RTC_CTRL_REG, 0x41);
    initialized = TRUE;
  }
  return DRIVER_NO_ERROR;
}

int rtc_close_(device_t dev) {
  return DRIVER_NO_ERROR;
}

int rtc_read_(device_t dev, int count, char* buffer) {
  char response[6];
  time_t* t = (time_t*) buffer;

  i2c_write8(I2C1_BASE, 0x4b, RTC_CTRL_REG, 0x41);
  i2c_read(I2C1_BASE, 0x4b, SECONDS_REG, (uint8_t*)response, 6);

  kernel_sleep(1);

  t->seconds = (response[0] >> 4) * 10 + (response[0] & 0x0F);
  t->minutes = (response[1] >> 4) * 10 + (response[1] & 0x0F);
  t->hours = (response[2] >> 4) * 10 + (response[2] & 0x0F);
  t->days = (response[3] >> 4) * 10 + (response[3] & 0x0F);
  t->month = (response[4] >> 4) * 10 + (response[4] & 0x0F);
  t->year = (response[5] >> 4) * 10 + (response[5] & 0x0F);

  return sizeof(time_t);
}

int rtc_write_(device_t dev, int count, char* buffer) {
  time_t* t = (time_t*) buffer;

  i2c_write8(I2C1_BASE, 0x4b, SECONDS_REG, t->seconds);
  i2c_write8(I2C1_BASE, 0x4b, MINUTES_REG, t->minutes);
  i2c_write8(I2C1_BASE, 0x4b, HOURS_REG, t->hours);
  i2c_write8(I2C1_BASE, 0x4b, DAYS_REG, t->days);
  i2c_write8(I2C1_BASE, 0x4b, MONTHS_REG, t->month);
  i2c_write8(I2C1_BASE, 0x4b, YEARS_REG, t->year);

  return sizeof(time_t);
}

int rtc_ioctl_(device_t dev, ioctl_t msg) {
  return DRIVER_ERROR_NOT_SUPPORTED;
}

int rtc_create_(device_t dev) {
  return DRIVER_ERROR_NOT_SUPPORTED;
}

