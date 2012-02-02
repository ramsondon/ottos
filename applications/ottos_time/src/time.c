/* time.c
 * 
 * Copyright (c) 2011 The ottos_time project.
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
 *  Created on: Feb 2, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <stdio.h>

#include <ottos/const.h>
#include <ottos/types.h>

#include <api/proc.h>
#include <api/time.h>
#include <api/io.h>
#include <api/system.h>

int main() {

  uint64_t stime = 0, etime = 0;
  char buffer[100] = { 0 };
  char timebuf[10] = { 0 };

  // get argument values (process name)
  int argc = 0;
  char **argv = sys_read_arguments(&argc);
  pid_t child = PID_INVALID;

  // check arguments
  if (argc < 2) {
    print("Invalid usage: time <process name> <args>\r\n");
    pexit(-1);
  }

  // goto next argument for starting child process
  argv++;
  // get current timestamp
  stime = uptime();

  // execute child process and stay blocked until process finished
  child = pexec(1, TRUE, (const char*)argv[0], argc - 1, argv);

  // get end of execution time
  etime = uptime();

  if (child == PID_INVALID) {
    pexit(-1);
  }

  // print time delta
  sprintf(buffer, "\r\nreal %s\r\n", timetostr_m_s_ms(etime - stime, timebuf));
  print(buffer);

  // safley exit process
  pexit(0);
  return 0;
}
