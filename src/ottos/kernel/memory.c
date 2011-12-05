/* memory.c
 * 
 * Copyright (c) 2011 The ottos project.
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
 *  Created on: 25.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include <ottos/memory.h>
#include <ottos/types.h>

void* memory_init_32(void* buffer, size_t length, uint32_t value) {
  do {
    ((uint32_t*) buffer)[--length] = value;
  } while (length != 0);
  return buffer;
}

void* memory_init_zero(void* buffer, size_t length) {
  return memory_init_32(buffer, length, 0);
}
