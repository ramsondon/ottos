/* main.c
 * 
 * Copyright (c) 2011 The ottos_rtc_test project.
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
#include <ottos/types.h>
#include <api/io.h>
#include <api/system.h>

int main() {

  char buffer[100] = {0};
  time_t time = sys_get_time();

  sprintf(buffer,
          "current time: %d.%d.%d %d:%d:%d\r\n",
          time.days, time.month, time.year,
          time.hours, time.minutes, time.seconds);

  print(buffer);

  sys_exit();

  return 0;
}
