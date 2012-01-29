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

static uint32_t process_count = 0;

pinfo_t* get_pinfos() {

  pinfo_t* pinfos = NULL;
  process_count = pcount();

  if (process_count <= 0) {
    return NULL;
  }
  pinfos = malloc(sizeof(pinfo_t) * process_count);
  process_count = pinfo(pinfos, process_count);
  return pinfos;
}

void free_pinfos(pinfo_t* pinfos) {
  int i = 0;
  for (i = 0; i < process_count; i++) {
    free(&pinfos[i]);
  }
}

void print_pinfo(pinfo_t* pinfo) {

  char buffer[500] = { 0 };
  // print current pinfo_t
  sprintf(buffer, "%d \t %d \t %s \t %d \t %d \t %/path/cmd\n\r",
           pinfo->pid,
           pinfo->tty,
           pstate_readable(pinfo->stat),
           pinfo->time,
           pinfo->mem
   //      pinfo->command
  );
  print(buffer);
}

int main(int argc, char **argv) {

  // allocate resources and get process infos
  pinfo_t* pinfo = get_pinfos();

  // print header of ps
  print("PID  \t TTY  \t STAT    \t TIME  \t MEM  \t CMD \n\r");

  if (pinfo != NULL) {

    int i = 0;
    // print all process info blocks
    for (i = 0; i < process_count; i++) {
      print_pinfo(&pinfo[i]);
    }
    // free allocated resources
    free_pinfos(pinfo);
  }
  return 0;
}
