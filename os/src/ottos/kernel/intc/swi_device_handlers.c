/* swi_device_handlers.c
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
 *  Created on: 13 Jan 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <stdlib.h>
// TODO move fs to a different folder?
#include <vfat/fat_filelib.h>
#include <ottos/code.h>
#include <ottos/error.h>
#include <ottos/const.h>
#include <ottos/types.h>
#include <ottos/syscalls.h>
#include <ottos/kernel.h>
#include <ottos/io.h>
#include <ottos/dev/device.h>
#include <ottos/drivers/driver.h>
#include "../ipc/ipc.h"
#include "../pm/process.h"
#include "../mmu/mmu.h"
#include "swi_device_handlers.h"

// forward declarations
BOOLEAN swi_handle_sys_fclose(int* error_code, process_file_descriptor_t* fd);
BOOLEAN swi_handle_sys_fwrite(process_file_descriptor_t* fd_process, const char* buffer, int buffer_size, int* written_bytes);
BOOLEAN swi_handle_sys_fread(process_file_descriptor_t* fd_process, char* buffer, int count, int* read_bytes);

BOOLEAN swi_handle_sys_yield() {
	return TRUE;
}

BOOLEAN swi_handle_sys_exit() {
	// delete the active process
	process_delete();
	return TRUE;
}

BOOLEAN swi_handle_sys_create(int priority, int block_current, int executable_file_address) {
	// get the real address of the executable_file_address
	const char* executable_file = (const char*) mmu_get_physical_address(process_table[process_active], executable_file_address);
	//int* pid = (int*)mmu_get_physical_address(process_table[process_active], pid_address);
	code_bytes_t* code = NULL;

	kernel_debug(1, "Loading hex file");
	code = code_get_single_file(executable_file);
	if(code == NULL) {
		return FALSE;
	}

	kernel_debug(1, "Creating a new process");
	/**pid = */process_create(priority, code);

	kernel_debug(1, "Created process");

	free(code->byte_0);
	free(code->byte_1);
	free(code->byte_2);
	free(code->byte_3);
	free(code);

	if (block_current == TRUE) {
		// block the active process
		process_table[process_active]->state = BLOCKED;
		return TRUE;
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_open(int fd_address, int device_id, int flags) {
	// get the real address of the file descriptor
	int* fd = (int*) mmu_get_physical_address(process_table[process_active], fd_address);
	int device_error_code;

	device_t device = (device_t) device_id;
	driver_t* driver = driver_get(device);
	if (driver == NULL) {
		kernel_error(DEVICE_UNKNOWN_ID, "Cannot open device");
	} else {
		device_error_code = driver->open(device);
		if (device_error_code != 0) {
      // could not open the device
		  kernel_error(DRIVER_ERROR_CANNOT_OPEN, "An error occurred while opening a device");
			*fd = SYSTEM_FD_INVALID;
		} else {
			// the file descriptor is the device
			*fd = device;

			// add a new process file descriptor
			process_add_device_descriptor(*fd);
		}
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_physical_address(unsigned int vaddr, unsigned int physaddr) {
  int* virtual = (int*) mmu_get_physical_address(process_table[process_active], vaddr);
  int* physical = (int*)mmu_get_physical_address(process_table[process_active], physaddr);
  *physical = (unsigned int)virtual;
  return FALSE;
}

BOOLEAN swi_handle_sys_close(int error_code_address, int fd) {
	// get the real address of the error code
	int* error_code = (int*) mmu_get_physical_address(process_table[process_active], error_code_address);

	// check if the file descriptor is a device or a file
	process_file_descriptor_t* fd_process = process_get_file_descriptor(fd);
	if (fd_process == NULL) {
	  char message[256];
	  sprintf(message, "Cannot close device or file %d (fd)", fd);
		kernel_error(PROCESS_UNKNOWN_FILE_DESCRIPTOR, message);
	} else {
		if (fd_process->type == DEVICE_FILE) {
			device_t device = (device_t) fd;
			*error_code = driver_get(device)->close(device);
		} else {
			swi_handle_sys_fclose(error_code, fd_process);
		}

		process_remove_file_descriptor(fd);
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_write(int fd, int buffer_address, int buffer_size_address) {
	// get the real address of the buffer_address
	const char* buffer = (const char*) mmu_get_physical_address(process_table[process_active], buffer_address);
	// use the buffer size address to get the length of the buffer and to return the written bytes
  int* written_bytes = (int*) mmu_get_physical_address(process_table[process_active], buffer_size_address);
	int buffer_size = *written_bytes;

	// check if the file descriptor is a device or a file
	process_file_descriptor_t* fd_process = process_get_file_descriptor(fd);
	if (fd_process == NULL) {
		kernel_error(PROCESS_UNKNOWN_FILE_DESCRIPTOR, "Cannot write into device or file");
	} else {
		if (fd_process->type == DEVICE_FILE) {
			device_t device = (device_t) fd;
			*written_bytes = driver_get(device)->write(device, buffer_size, (char*) buffer);
		} else {
			swi_handle_sys_fwrite(fd_process, buffer, buffer_size, written_bytes);
		}
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_read(int fd, int buffer_address, int buffer_size_address) {
	// get the real address of the buffer_address
	char* buffer = (char*) mmu_get_physical_address(process_table[process_active], buffer_address);
  // use the buffer size address to get the length of the buffer and to return the read bytes
  int* read_bytes = (int*) mmu_get_physical_address(process_table[process_active], buffer_size_address);
  int buffer_size = *read_bytes;

	// check if the file descriptor is a device or a file
	process_file_descriptor_t* fd_process = process_get_file_descriptor(fd);
	if (fd_process == NULL) {
		kernel_error(PROCESS_UNKNOWN_FILE_DESCRIPTOR, "Cannot read from device or file");
	} else {
		if (fd_process->type == DEVICE_FILE) {
			device_t device = (device_t) fd;
			*read_bytes = driver_get(device)->read(device, buffer_size, buffer);
		} else {
			swi_handle_sys_fread(fd_process, buffer, buffer_size, read_bytes);
		}
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_fopen(int fd_address, int path_address, int flags) {
	// get the real address of the file descriptor
	int* fd = (int*) mmu_get_physical_address(process_table[process_active], fd_address);
	// get the real address of the path address
	const char* path = (const char*) mmu_get_physical_address(process_table[process_active], path_address);

	// TODO parse flags to get the mode
	const char mode[] = { 'r' };
	FL_FILE* file = fl_fopen(path, mode);

	if (file == NULL) {
		kernel_error(FILE_UNKNOWN, "Cannot open file");
	} else {
		process_file_descriptor_t* fd_process = process_add_file_descriptor(file);
		*fd = fd_process->fd;
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_fclose(int* error_code, process_file_descriptor_t* fd_process) {

	fl_fclose(fd_process->file);

	return FALSE;
}

BOOLEAN swi_handle_sys_fwrite(process_file_descriptor_t* fd_process, const char* buffer, int buffer_size, int* written_bytes) {

	*written_bytes = fl_fwrite(buffer, buffer_size, buffer_size, fd_process->file);

	return FALSE;
}

BOOLEAN swi_handle_sys_fread(process_file_descriptor_t* fd_process, char* buffer, int count, int* read_bytes) {

	*read_bytes = fl_fread(buffer, count, count, fd_process->file);

	return FALSE;
}

BOOLEAN swi_handle_sys_bind(int ns_address, int result_address) {
  const char* namespace =
      (const char*) mmu_get_physical_address(process_table[process_pid()], ns_address);

  int* result = (int*)mmu_get_physical_address(process_table[process_pid()], result_address);

  *result = ipc_bind(namespace, process_pid());

  return FALSE;
}

BOOLEAN swi_handle_sys_send(int ns_address, int msg_address, int result_address) {
  const char* namespace =
      (const char*) mmu_get_physical_address(process_table[process_pid()], ns_address);
  message_t* message =
      (message_t*) mmu_get_physical_address(process_table[process_pid()], msg_address);
  int* result = (int*)mmu_get_physical_address(process_table[process_pid()], result_address);
  void* content = (void*)mmu_get_physical_address(process_table[process_pid()], (unsigned int)message->content);

  *result = ipc_send_msg(namespace, *message, content, process_pid());

  return FALSE;
}

BOOLEAN sys_handle_sys_wait(int ns_address) {
  const char* namespace =
        (const char*) mmu_get_physical_address(process_table[process_pid()], ns_address);

  if (ipc_lookup_msg_concrete(namespace, process_pid()) == WAITING) {
    process_table[process_pid()]->state = BLOCKED;
    process_table[process_pid()]->blockstate = IPC_WAIT;

    return TRUE;
  }

  return FALSE;
}

BOOLEAN swi_handle_sys_receive(int ns_address, int msg_address, int result_address) {
  const char* namespace =
      (const char*) mmu_get_physical_address(process_table[process_pid()], ns_address);
  message_t* message =
      (message_t*) mmu_get_physical_address(process_table[process_pid()], msg_address);
  int* result = (int*)mmu_get_physical_address(process_table[process_pid()], result_address);
  void* content = (void*)mmu_get_physical_address(process_table[process_pid()], (unsigned int)message->content);

  *result = ipc_receive_msg(namespace, message, content, process_pid());

  return FALSE;
}

BOOLEAN swi_handle(unsigned int syscall_nr, unsigned int param1, unsigned int param2, unsigned int param3) {

	// handle interrupts
	switch (syscall_nr) {
	case SYS_YIELD:
		return swi_handle_sys_yield();
	case SYS_EXIT:
		return swi_handle_sys_exit();
	case SYS_EXEC:
		// param1 = priority
		// param2 = block_current
		// param3 = path
		return swi_handle_sys_create(param1, param2, param3);
	case SYS_OPEN:
		// param1 = return value (fd)
		// param2 = device id
		// param3 = flags
		return swi_handle_sys_open(param1, param2, param3);
	case SYS_FOPEN:
		// param1 = return value (fd)
		// param2 = path
		// param3 = falgs
		return swi_handle_sys_fopen(param1, param2, param3);
	case SYS_CLOSE:
		// param1 = return value (error code)
		// param2 = fd
		return swi_handle_sys_close(param1, param2);
	case SYS_WRITE:
		// param1 = fd
		// param2 = buffer
		// param3 = buffer size
		return swi_handle_sys_write(param1, param2, param3);
	case SYS_READ:
		// param1 = fd
		// param2 = buffer
		// param3 = count
		return swi_handle_sys_read(param1, param2, param3);
  case SYS_BIND_NAMESPACE:
    // param1 = namespace
    // param2 = result
    return swi_handle_sys_bind(param1, param2);
  case SYS_SEND:
    // param1 = namespace
    // param2 = message
    // param3 = result
    return swi_handle_sys_send(param1, param2, param3);
  case SYS_WAIT_MSG:
    // param1 = namespace
    return sys_handle_sys_wait(param1);
  case SYS_RECEIVE:
    // param1 = namespace
    // param2 = message
    // param3 = result
    return swi_handle_sys_receive(param1, param2, param3);
  case SYS_PHYSICAL_ADDRESS:
    // param1 = virtual address
    // param2 = physical address return value
    return swi_handle_sys_physical_address(param1, param2);
	default:
		// unknown syscall number
		kernel_error(SWI_UNKNOWN_SYSCALL_NR, "Unknown syscall-number. Ignoring.");
		break;
	}

	return FALSE;
}
