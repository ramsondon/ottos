/* avr.h
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

#ifndef AVR_H_
#define AVR_H_

#include <ottos/drivers/driver.h>

#define MUX_GPIO139   0x4800216A
#define MUX_GPIO138   0x48002168
#define MUX_GPIO137   0x48002166
#define MUX_GPIO136   0x48002164
#define MUX_GPIO135   0x48002162
#define MUX_GPIO134   0x48002160
#define MUX_GPIO133   0x4800215E
#define MUX_GPIO132   0x4800215C
#define MUX_GPIO131   0x4800215A
#define MUX_GPIO130   0x48002158

#define GPIO_139    BIT11
#define GPIO_138    BIT10
#define GPIO_137    BIT9
#define GPIO_136    BIT8
#define GPIO_135    BIT7
#define GPIO_134    BIT6
#define GPIO_133    BIT5
#define GPIO_132    BIT4
#define GPIO_131    BIT3
#define GPIO_130    BIT2

#define GPIO_MUX_SETTINGS 0x11C   //pullup enabled, Muxmode4, input enabled

#define GPIO5_DATAIN  0x49056038
#define GPIO4_DATAIN  0x49054038
#define GPIO5_DATAOUT 0x4905603C


#define GPIO5_SETDATA 0x49056094
#define GPIO5_CLEANDATA 0x49056090
#define GPIO5_OE    0x49056034

// PRCM
#define CM_FCLKEN_PER      0x48005000   // Controls the modules functional clock activity. (Table 4-237)
#define CM_ICLKEN_PER      0x48005010

int avr_open_(device_t dev);
int avr_close_(device_t dev);
int avr_read_(device_t dev, int count, char* buffer);
int avr_write_(device_t dev, int count, char* buffer);
int avr_ioctl_(device_t dev, ioctl_t msg);
int avr_create_(device_t dev);

static driver_t avr_temp_driver = {
  avr_open_,
  avr_close_,
  avr_read_,
  avr_write_,
  avr_ioctl_,
  avr_create_
};

static driver_t avr_pressure_driver = {
  avr_open_,
  avr_close_,
  avr_read_,
  avr_write_,
  avr_ioctl_,
  avr_create_
};

static driver_t avr_solar_driver = {
  avr_open_,
  avr_close_,
  avr_read_,
  avr_write_,
  avr_ioctl_,
  avr_create_
};


#endif /* AVR_H_ */
