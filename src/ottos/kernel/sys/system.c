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

#pragma SWI_ALIAS(swi, 1)
EXTERN void swi(unsigned syscall_nr, unsigned param1, unsigned param2, unsigned param3);

void sys_yield() {
	swi(SYS_YIELD, 0, 0, 0);
}

void sys_exit() {
	swi(SYS_EXIT, 0, 0, 0);
}

void sys_create_process(int priority, int initial_address, int wait_for_exit) {
	swi(SYS_CREATE_PROCESS, priority, initial_address, wait_for_exit);
}

void sys_mmu_test() {
	swi(SYS_MMU_TEST, 0, 0, 0);
}

void sys_print(int length, char* output) {
  swi(SYS_PRINT, length, (unsigned int)output, 0);
}

void sys_bind(const char* ns) {
  swi(SYS_BIND_NAMESPACE, (unsigned)ns, 0, 0);
}

void sys_send(const char* ns, message_t* msg) {
  swi(SYS_SEND, (unsigned)ns, (unsigned)msg, 0);
}

void sys_receive(const char* ns, message_t *msg) {
  swi(SYS_RECEIVE, (unsigned)ns, (unsigned)msg, 0);
}

