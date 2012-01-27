/* main.c
 * 
 * Copyright (c) 2011 The ottos_ps project.
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
 *  Created on: Jan 27, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <api/proc.h>
#include <api/io.h>

int main(int argc, char **argv) {

  int cpinfo = 0;
  pinfo_t* pinfo = NULL;

  // the memory must be freed by this process after calling proc_infos()
  int count = proc_infos(pinfo);

  print("PID \t TTY \t STAT \t TIME \t MEM \t CMD \n\r");

  for (cpinfo = 0; cpinfo < count; cpinfo++) {

    char buffer[300] = { 0 };
    // print current pinfo_t
    sprintf(buffer, "%d \t %d \t %d \t %d \t %d \t %s\n\r",
            pinfo[cpinfo].pid,
            pinfo[cpinfo].tty,
            pinfo[cpinfo].stat,
            pinfo[cpinfo].time,
            pinfo[cpinfo].mem,
            pinfo[cpinfo].command
    );
    print(buffer);
    print("\n\r");
  }

  free(pinfo);
  return 0;
}
