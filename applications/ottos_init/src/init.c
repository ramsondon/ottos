/* init.c
 * 
 * Copyright (c) 2011 The ottos_init project.
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
 *  Created on: 02.02.2012
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include <ottos/types.h>

#include <api/proc.h>
#include <api/system.h>
#include <api/io.h>

#define APP_WEATHER_SERVICE "/bin/weather_service"
#define APP_WEATHER_LOGGER  "/bin/weather_logger"
#define APP_WEATHER_UI      "/bin/weather_ui"
#define APP_TTY             "/bin/ottos_tty"

pid_t start_app(const char* app) {
  print(app);
  print(" ................................. done\r\n");
  return (pid_t) sys_execute(1, FALSE, app, 0, NULL);
}

void run_forever() {
  pid_t tty_pid = start_app(APP_TTY);

  do {
    pinfo_t tty_info;

    // is the tty process running?
    if (0 == pinfo_for(tty_pid, &tty_info)) {
      print("tty is not running, restarting ...\n\r");
      tty_pid = start_app(APP_TTY);
    }

    // hand over to another process
    sys_yield();

  } while (1);
}


int main(int argc, char** argv) {

  // basic processes
  // start_app(APP_WEATHER_SERVICE);
  // start_app(APP_WEATHER_LOGGER);
  // start_app(APP_WEATHER_UI);

  // start terminal and check in an endless loop if tty is running
  print("Booting OttOS ...\n\r");
  run_forever();

  pexit(0);
  return 0;
}
