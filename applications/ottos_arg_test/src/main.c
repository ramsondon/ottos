/* main.c
 * 
 * Copyright (c) 2011 The ottos_arg_test project.
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
 *  Created on: 27 Jan 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <stdio.h>
#include <api/system.h>
#include <api/io.h>

int main() {

  int argc = 0;
  char** argv = NULL;
  char buffer[100] = {0};
  int i = 0;

  argv = sys_read_arguments(&argc);

  sprintf(buffer, "argc: %d\r\n", argc);
  print(buffer);

  print("arguments:\r\n");

  for(i = 0; i < argc; i++) {
    sprintf(buffer, "argv[%d]: %s\r\n", i, argv[i]);
    print(buffer);
  }

  print("bye bye\r\n");

  return 0;
}
