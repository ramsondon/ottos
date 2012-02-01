/* rtc.h
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

#ifndef RTC_H_
#define RTC_H_

#include <ottos/drivers/driver.h>

int rtc_open_(device_t dev);
int rtc_close_(device_t dev);
int rtc_read_(device_t dev, int count, char* buffer);
int rtc_write_(device_t dev, int count, char* buffer);
int rtc_ioctl_(device_t dev, ioctl_t msg);
int rtc_create_(device_t dev);

static driver_t rtc_driver = {
  rtc_open_,
  rtc_close_,
  rtc_read_,
  rtc_write_,
  rtc_ioctl_,
  rtc_create_
};

#endif /* RTC_H_ */
