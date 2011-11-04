/* LedDriver.cc
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
 *  Created on: 03.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include "LedDriver.h"

#include "../../hal/leds.h"

LedDriver::LedDriver(int dev) :
  Driver(dev) {
}

LedDriver::~LedDriver() {
}

int LedDriver::open() {
  *(volatile unsigned long *) GPIO5_OE |= (1 << dev_);
  return 1;
}

int LedDriver::close() {
  *(volatile unsigned long *) GPIO5_OE &= ~(1 << dev_);
  return 1;
}

int LedDriver::write(int count, char* buffer) {
  int what = (int) *buffer;
  switch (what) {
    case LED_ON:
      *(volatile unsigned long *) GPIO5_DATAOUT |= (1 << dev_);
      break;
    case LED_OFF:
      *(volatile unsigned long *) GPIO5_DATAOUT &= ~(1 << dev_);
      break;
    default:
      return -1;
  }
  return 0;
}

int LedDriver::read(int count, char* buffer) {
  *buffer = (char) (*(volatile unsigned long *) GPIO5_DATAOUT & (1<<dev_) ? LED_ON : LED_OFF);
}
