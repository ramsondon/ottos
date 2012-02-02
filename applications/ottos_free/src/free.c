/* free.c
 * 
 * Copyright (c) 2011 The ottos_free project.
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
 *  Created on: Feb 1, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <ottos/types.h>
#include <api/io.h>
#include <api/memory.h>
#include <api/proc.h>

int main(int argc, char **argv) {

  meminfo_t* info = malloc(sizeof(meminfo_t));
  char title[100] = { 0 };
  char buffer[100] = { 0 };
  char memtotal[10] = { 0 };
  char memused[10] = { 0 };
  char memfree[10] = { 0 };
  double total = 0;
  double freemem = 0;
  double used = 0;

  // write memory info to meminfo_t*
  meminfo(info);

  total = info->total_extddr + info->total_intram;
  used = info->used_extddr + info->used_intram;
  freemem = total - used;

  sprintf(title, "%10s\t%10s\t%10s\n\r", "total", "used", "free");
  sprintf(buffer, "%10s\t%10s\t%10s\n\r", memstr(total, memtotal), memstr(used, memused), memstr(freemem, memfree));

  // print buffer to STDOUT
  print(title);
  print(buffer);
  free(info);

  pexit(0);
  return 0;
}
