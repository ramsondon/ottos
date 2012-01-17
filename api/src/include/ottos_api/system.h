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

#include <ottos/types.h>
#include <ottos/const.h>

#define SYSTEM_DEV_PATH  "/dev"
#define SYSTEM_DEV_ID_INVALID -1

#define SYSTEM_SERIAL_0_PATH SYSTEM_DEV_PATH "/serial0"

#define SYSTEM_LED_0_PATH SYSTEM_DEV_PATH "/led0"
#define SYSTEM_LED_1_PATH SYSTEM_DEV_PATH "/led1"

// binary flags. increase new flag number in binary order (e.g.: 1, 2, 4, 8, ...)
#define SYSTEM_FLAG_READ 1
#define SYSTEM_FLAG_WRITE 2

EXTERN int sys_open(const char* path, int flags);
EXTERN size_t sys_read(int fd, const char* buffer, size_t count);
EXTERN size_t sys_write(int fd, const char* buffer, size_t nbytes);
EXTERN int sys_close(int fd);

EXTERN int sys_execute(int priority, BOOLEAN block_current, const char* path);

#endif /* SYSTEM_H_ */
