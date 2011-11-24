/* memory.h
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
 *  Created on: 18.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <ottos/types.h>
#include <bits.h>

#define ARRAY_INIT(array, size, value) \
  { int i=0; \
  for(;i<size;i++) { \
    array[i]=value; } \
  }

/*
#define MMIO_WRITE32(addr, data) \
    asm("\t ldr r1, ($addr) ;\n" \
    "\t ldr r0, ($data) ;\n" \
    "\t str r0, [r1]") */
#define MMIO_WRITE32(addr, data) \
  (*((mem_address_t *) addr) = data)

/*
#define MMIO_OR32(addr, or_data) \
  asm("\t ldr r1, ($addr) ;\n" \
  "\t ldr r2, ($or_data) ;\n" \
  "\t ldr r0, [r1] ;\n" \
  "\t orr r0, r0, r2 ;\n" \
  "\t str r0, [r1]");*/

#define MMIO_OR32(addr, or_data) \
  (*((mem_address_t *) addr) |= or_data)

/*
#define MMIO_AND32(addr, and_data) \
  asm("\t ldr r1, ($addr) ;\n" \
  "\t ldr r2, ($and_data) ;\n" \
  "\t ldr r0, [r1] ;\n" \
  "\t and r0, r0, r2 ;\n" \
  "\t str r0, [r1]"); */

#define MMIO_AND32(addr, and_data) \
  (*((mem_address_t *) addr) &= and_data)

/*
#define MMIO_AND_THEN_OR32(addr, and_data, or_data) \
  asm("\t ldr r1, ($addr) ;\n" \
  "\t ldr r0, r1 ;\n" \
  "\t ldr r2, ($and_data) ;\n" \
  "\t and r0, r0, r2 ;\n" \
  "\t ldr r2, ($or_data) ;\n" \
  "\t orr r0, r0, r2 ;\n" \
  "\t str r0, [r1]"); */

#define MMIO_AND_THEN_OR32(addr, and_data, or_data) \
  MMIO_AND32(addr, and_data); \
  MMIO_OR32(addr, or_data)

#define MMIO_READ32(addr) *((mem_address_t *) addr)


#endif /* MEMORY_H_ */
