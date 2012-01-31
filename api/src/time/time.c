/* time.c
 * 
 * Copyright (c) 2011 The ottos_api project.
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
 *  Created on: Jan 29, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <stdio.h>
#include <ottos/types.h>
#include <api/time.h>

static char* pad2_str(uint32_t val, char* buffer) {
  sprintf(buffer, ((val < 10) ? "0%d" : "%2d"), val);
  return buffer;
}

char* timetostr(uint64_t ms, char* buffer) {

  uint32_t days = (uint32_t) (ms / ONE_DAY);
  uint32_t hours = (uint32_t) (ms / ONE_HOUR - days * 24);
  uint32_t minutes = (uint32_t) (ms / ONE_MINUTE - (hours + days * 24) * 60);
  uint32_t seconds = (uint32_t) (ms / ONE_SEC - (minutes + (hours + days * 24) * 60) * 60);
  //uint32_t milliseconds = (uint32_t) (ms % 1000);
  char h[5] = {0};
  char m[5] = {0};
  char s[5] = {0};

  sprintf(buffer, "%s:%s:%s", pad2_str(hours, h), pad2_str(minutes, m), pad2_str(seconds, s));
  return buffer;
}
