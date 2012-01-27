/* system.c
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
 *  Created on: 13 Jan 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <string.h>
#include <ottos/syscalls.h>
#include <ottos/types.h>
#include <stdio.h>
#include <ottos/dev/device.h>
#include <api/system.h>

#pragma SWI_ALIAS(swi, 1)
EXTERN void swi(unsigned int syscall_nr, unsigned int param1, unsigned int param2, unsigned int param3);

static system_file_type_t system_get_file_type(const char* path) {
  if (strncmp(SYSTEM_DEV_PATH, path, strlen(SYSTEM_DEV_PATH)) == 0) {
    return DEVICE_FILE;
  }
  return NON_DEVICE_FILE;
}

static int system_get_device_id(const char* path) {
	if (strcmp(SYSTEM_SERIAL_0_PATH, path) == 0) {
		return SERIAL_0;
	} else if (strcmp(SYSTEM_LED_0_PATH, path) == 0) {
		return LED_0;
	} else if (strcmp(SYSTEM_LED_1_PATH, path) == 0) {
		return LED_1;
	} else if (strcmp(SYSTEM_VIDEO_0_PATH, path) == 0) {
    return VIDEO_0;
  }
	return SYSTEM_DEV_ID_INVALID;
}

int sys_open(const char* path, int flags) {
  int return_value = SYSTEM_FD_INVALID;
  int device_id = SYSTEM_DEV_ID_INVALID;

  switch (system_get_file_type(path)) {
    case DEVICE_FILE:
      device_id = system_get_device_id(path);
      if (device_id != SYSTEM_DEV_ID_INVALID) {
        swi(SYS_OPEN, (unsigned int) &return_value, device_id, flags);
      }
      break;
    case NON_DEVICE_FILE:
      swi(SYS_FOPEN, (unsigned int) &return_value, (unsigned int) path, flags);
      break;
  }
  return return_value;
}

size_t sys_write(int fd, const char* buffer, size_t nbytes) {
  size_t return_value = nbytes;

  swi(SYS_WRITE, fd, (unsigned int) buffer, (unsigned int) &return_value);

  return return_value;
}

size_t sys_read(int fd, const char* buffer, size_t count) {
  size_t return_value = count;

  swi(SYS_READ, fd, (unsigned int) buffer, (unsigned int) &return_value);

  return return_value;
}

int sys_close(int fd) {
  int return_value = 0;

  swi(SYS_CLOSE, (unsigned int) &return_value, fd, 0);

  return return_value;
}

unsigned int sys_physical_address_of(const void* address) {

  unsigned int physical_address = 0;
  swi(SYS_PHYSICAL_ADDRESS, (unsigned int) address, (unsigned int) &physical_address, 0);
  return physical_address;
}

int sys_execute(int priority, BOOLEAN block_current, const char* path) {
  int return_value = PID_INVALID;

  swi(SYS_EXEC, priority, (unsigned int) block_current, (unsigned int) path);

  return return_value;
}

void sys_bind(const char* ns, int* success) {
  swi(SYS_BIND_NAMESPACE, (unsigned int) ns, (unsigned int) success, 0);
}

void sys_send(const char* ns, message_t* msg, int* success) {
  swi(SYS_SEND, (unsigned int) ns, (unsigned int) msg, (unsigned int) success);
}

void sys_wait_msg(const char* ns) {
  swi(SYS_WAIT_MSG, (unsigned int) ns, 0, 0);
}

void sys_receive(const char* ns, message_t *msg, int* success) {
  swi(SYS_RECEIVE, (unsigned int) ns, (unsigned int) msg, (unsigned int) success);
}
