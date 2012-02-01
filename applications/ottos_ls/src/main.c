/* main.c
 * 
 * Copyright (c) 2011 The ottos_ls project.
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
 *  Created on: 25.01.2012
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include "ls/ls.h"
#include <api/system.h>
#include <api/io.h>
#include <api/proc.h>

int main() {

  int argc = 0;
  char** argv = sys_read_arguments(&argc);

  if(argc != 2) {
	  print("usage: ls [path] (path isn't optional)\r\n");
	  sys_exit(1);
  }

  ls(argv[1]);
  pexit(0);
  return 0;
}
