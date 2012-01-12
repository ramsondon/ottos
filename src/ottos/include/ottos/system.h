/* system.h
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

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <ottos/const.h>
#include <ottos/types.h>

/**
 * Scheduler system calls
 */
EXTERN void sys_yield();
EXTERN void sys_exit();
EXTERN void sys_create_process(int priority, int initial_address, int wait_for_exit);

/**
 * I/O system calls
 */
EXTERN address_t sys_open(char* filename, int flags);
EXTERN size_t sys_read();
EXTERN size_t sys_write();
EXTERN int sys_close();

/*
 * IPC system calls
 */
EXTERN void sys_bind(const char* ns);
EXTERN void sys_send(const char* ns, message_t* msg);
EXTERN void sys_receive(const char* ns, message_t* msg);

/*
 * MMU system calls
 */
EXTERN void sys_mmu_test();

/*
 * Syscall tests
 */
EXTERN void sys_print(int length, char* output);

#endif /* SYSTEM_H_ */
