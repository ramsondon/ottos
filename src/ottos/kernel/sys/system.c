/* system.c
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
 *  Created on: 27 Oct 2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <ottos/system.h>
#include <ottos/syscalls.h>


asm("\t .bss _reg_val, 4");
asm("\t .global _reg_val");
asm("reg_val_a .field _reg_val, 32");

extern int reg_val;

#define PUT_TO_REGISTER(reg, val) \
  reg_val = (int) val; \
  asm("\t LDR "reg", reg_val_a");

#define READ_FROM_REGISTER(reg, var) \
  PUT_TO_REGISTER(reg, var); \
  asm("\t STR "reg", ["reg",#0]"); \
  var = (int) reg_val;

#pragma SWI_ALIAS(swi, 1)
EXTERN void swi(unsigned syscall_nr, unsigned param1, unsigned param2, unsigned param3);

#define REG_0 "r0"
#define REG_1 "r1"
#define REG_2 "r2"

EXTERN

void sys_yield() {
  swi(SYS_YIELD, 0, 0, 0);
}

void sys_exit() {
  swi(SYS_EXIT, 0, 0, 0);
}

void sys_create_process(int priority, int initial_address, int wait_for_exit) {
  swi(SYS_CREATE_PROCESS, priority, initial_address, wait_for_exit);
}

address_t sys_open(char* filename, int flags) {

  address_t  address = (address_t) (void*)0;

  // store registers temporary
  int t[2] = {0, 0};
  //READ_FROM_REGISTER(REG_1, &t[0]);
  //READ_FROM_REGISTER(REG_2, &t[1]);

  // put system call parameters
  PUT_TO_REGISTER(REG_1, (int) filename);
  PUT_TO_REGISTER(REG_2, flags);

  // set system call
  swi(SYS_OPEN, 0, 0, 0);

  // read return

  READ_FROM_REGISTER(REG_0, address);

  // reset registers to temporary state
  PUT_TO_REGISTER(REG_1, t[0]);
  PUT_TO_REGISTER(REG_2, t[1]);

  return address;
}


