/* proc.c
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
 *  Created on: Jan 27, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <stdlib.h>

#include <ottos/const.h>
#include <ottos/types.h>

#include <api/system.h>
#include <api/proc.h>

#define PSTATE_MAP_COUNT 4

static const char* pstate_map[] = { "ready", "wait", "exec", "?"};

uint32_t pcount() {
  return sys_pcount();
}

uint32_t pinfo(pinfo_t* pinfo, uint32_t count) {
  return sys_pinfo(pinfo, count);
}

void pexit(int state) {
  sys_exit(state);
}

const char* pstate_readable(int stat) {

  if (stat < PSTATE_MAP_COUNT) {
    return pstate_map[stat];
  }
  return pstate_map[PSTATE_MAP_COUNT-1];
}

