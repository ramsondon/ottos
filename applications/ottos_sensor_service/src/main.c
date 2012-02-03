/* main.c
 * 
 * Copyright (c) 2011 The ottos_sensor_service project.
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
 *  Created on: 2 Feb 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <api/proc.h>
#include <api/sensor.h>
#include <api/ipc.h>

int main() {

  message_t message;
  char* namespace = "ottossensor";
  sensor_values_t oldVals;
  int i = 0;
  int sleeptime = 200;

  message.count = 1;
  message.size = sizeof(sensor_values_t);
  message.type = 1;

  oldVals = sensor_read_values();
  while (1) {
    sensor_values_t values = sensor_read_values();

//    while ((values.temp > (2 * oldVals.temp)
//        || (10 * values.solar) < oldVals.solar
//        || (10 * values.solar) > oldVals.solar
//        || values.pressure > (oldVals.pressure + 10))
//        && i < sleeptime) {
//
//      psleep(1);
//      values = sensor_read_values();
//      i++;
//    }
//    oldVals = values;

    message.content = &values;

    send(namespace, &message);

    psleep(sleeptime - i);
    i = 0;
  }

  pexit(0);
  return 0;
}
