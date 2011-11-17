/* mmu.c
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
 *  Created on: 11.11.2011
 *      Author: Thomas Wiedemann <thomas.wiedemann@students.fhv.at>
 *
*/
#include "mmu.h"
#include <ottos/types.h>

void mmu_init(){
  unsigned int i;
     address tableAddress = (address)0x40200000;
     // Set Domain Access control register to 0101 0101 0101 0101 0101 0101 0101 0101
     asm("\t MOV r1, #0x5557\n");
     asm("\t MOVT r1, #0x5555\n");
     asm("\t MCR p15, #0, r1, c3, c0, #0\n");

     // Set the Master Table Pointer to the internal ram
     asm("\t MOV r1, #0x0000\n");
     asm("\t MOVT r1, #0x4020\n");
     asm("\t MCR p15, #0, r1, c2, c0, #0\n");

     // Initialize Master Table
     for (i = 0x00000000; i < 0xFFF00000; i += 0x00100000) {
         *tableAddress = i | 0xC12;
         tableAddress++;
     }
     *tableAddress = 0xFFF00C12;

     // Enable MMU
     asm("\t MRC p15, #0, r1, c1, c0, #0\n");
     asm("\t ORR r1, r1, #0x1\n");
     asm("\t MCR p15, #0, r1, c1, c0, #0\n");

}


