/* main.c
 * 
 * Copyright (c) 2011 The ottos_sensor_console_logger project.
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

#include <stdio.h>

#include <api/sensor.h>
#include <api/ipc.h>
#include <api/proc.h>
#include <api/io.h>
#include <ottos/types.h>

int main() {


  message_t message;
  sensor_values_t sval;
  char* namespace = "ottossensor";

  message.count = 1;
  message.size = sizeof(sensor_values_t);
  message.type = 1;
  message.content = &sval;

  if (bind(namespace) != IPC_SUCCESS) {
      print("namespace binding error\n\r");
      pexit(-1);
  }

  while(1) {

    if (receive(namespace, &message) == IPC_SUCCESS) {
      char buffer[100] = {0};
      sprintf(buffer, "TEMP: %.02f\tSOLAR: %.02f\tPRESSURE: %.02f\r\n", sval.temp, sval.solar, sval.pressure);
      print(buffer);
    }
  }

  pexit(0);
  return 0;
}

