/* uptime_test.c
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
 *  Created on: 16.12.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include "uptime_test.h"

#include <ottos/timer.h>
#include <ottos/kernel.h>
#include <ottos/io.h>

void uptime_test() {
  while (1) {
    char timer[64];
    time_t uptime = { 0, 0, 0, 0, 0 };

    timer_parse_time(&uptime, timer_system_uptime());
    timer_sprint_time(uptime, timer);
    kernel_print("Current system uptime: ");
    kernel_print(timer);
    kernel_print("\n\r");
  }
}
