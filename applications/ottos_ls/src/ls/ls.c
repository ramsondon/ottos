/* ls.c
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

#include "ls.h"
#include <api/system.h>
#include <api/io.h>

void ls(const char* path) {
  int fd = sys_open(path, 0);
  print("ls ...\n");
  sys_close(fd);
}

void ls2(const char* path) {
  dir_t dirstat;

  if (sys_opendir(path, &dirstat)) {
    dir_entry_t dirent;

    while (sys_readdir(&dirstat, &dirent) == 0) {
      char buffer[512];
      sprintf(buffer, "%crwx------ root wheel %5d %s\r\n", (dirent.is_dir ? 'd'
          : '-'), dirent.size, dirent.filename);
      print(buffer);
    }

    sys_closedir(&dirstat);
  }
}
