/* main.c
 * 
 * Copyright (c) 2011 The ottos_cat project.
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
#include <api/proc.h>
#include <api/io.h>
#include <api/system.h>

void cat(const char* filename) {
  int fd = sys_open(filename, SYSTEM_FLAG_READ);
  if(fd != SYSTEM_FD_INVALID) {
    int read = 0;

    do {
      char buffer[512 + 1] = {0};
      read = sys_read(fd, buffer, 512);
      buffer[read] = 0;
      print(buffer);

    } while(read >= 512);

    sys_close(fd);
  } else {
    print("file not found\r\n");
  }
}

int main() {

  int argc = 0;
  char** argv = sys_read_arguments(&argc);

  if(argc < 2) {
    char buffer[100] = {0};
    char* proc_name = "ottos_cat";
    if(argc == 1) {
      proc_name = argv[0];
    }
    sprintf(buffer, "usage: %s [filename]\r\n", proc_name);
    print(buffer);
    pexit(-1);
  }

  cat(argv[1]);

  print("\r\n");

  pexit(0);
  return 0;
}
