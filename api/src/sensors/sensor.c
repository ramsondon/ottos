/* sensor.c
 * 
 * Copyright (c) 2011 The ottos_api project.
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
#include <stdlib.h>
#include <api/system.h>
#include <api/sensor.h>

static double read_value(int fd) {
  double value = 0.0;

  if (fd != SYSTEM_FD_INVALID) {
    sys_read(fd, (char*)&value, sizeof(double));
  }

  return value;
}

double sensor_read_temp() {

  int fd = sys_open(SYSTEM_TEMP_0_PATH, SYSTEM_FLAG_READ);

  // no need to close sensor
  return read_value(fd);
}

double sensor_read_pressure() {

  int fd = sys_open(SYSTEM_PRESSURE_0_PATH, SYSTEM_FLAG_READ);

  // no need to close sensor
  return read_value(fd);
}

double sensor_read_solar() {

  int fd = sys_open(SYSTEM_SOLAR_0_PATH, SYSTEM_FLAG_READ);

  // no need to close sensor
  return read_value(fd);
}
