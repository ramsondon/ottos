/* main.c
 * 
 * Copyright (c) 2011 The ottos_sensor_test project.
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

#include <stdio.h>
#include <api/io.h>
#include <api/sensor.h>
#include <api/system.h>

int main() {

  int argc = 0;
  char** argv = sys_read_arguments(&argc);

  if(argc < 2) {
    print("usage: ");
    print(argv[0]);
    print(" [sensor_number]\r\n");
    sys_exit();
  } else {
    double value = 0.0;
    int sensor_number = argv[1][0] - '0';
    char buffer[100] = {0};
    sprintf(buffer, "sensor_number: string:%s int:%d\r\n", argv[1], sensor_number);
    print(buffer);

    if (sensor_number < 0 || sensor_number > 3) {
      print("sensor_number value must be 1, 2 or 3\r\n");
      sys_exit();
    } else {
      switch(sensor_number) {
        case 1:
          value = sensor_read_temp();
          break;
        case 2:
          value = sensor_read_pressure();
          break;
        case 3:
          value = sensor_read_solar();
          break;
        default:
          print("WTF????\r\n");
          break;
      }

      sprintf(buffer, "sensor value: %.2f\r\n", value);
      print(buffer);
    }
  }

  return 0;
}
