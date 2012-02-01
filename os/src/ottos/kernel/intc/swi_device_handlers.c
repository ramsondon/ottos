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
#include <ottos/memory.h>
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

BOOLEAN swi_handle_sys_exit(int state) {
	// delete the active process
	process_delete();
	return TRUE;
}

BOOLEAN swi_handle_sys_create(int parameters_address) {
	code_bytes_t* code = NULL;

	// get the real address of the executable_file_address
	int* parameters = (int*) mmu_get_physical_address(process_table[process_active], parameters_address);

	// get parameter values
	int* pid = (int*) mmu_get_physical_address(process_table[process_active], parameters[0]);
	int priority = parameters[1];
	BOOLEAN block_current = (BOOLEAN) parameters[2];
	const char* executable_file = (const char*) mmu_get_physical_address(process_table[process_active], parameters[3]);
	int argc = parameters[4];
	char** argv = (char**) mmu_get_physical_address(process_table[process_active], parameters[5]);
	char** argv_copy = NULL;

	//code = code_get(executable_file);
	code = code_get_single_file(executable_file);
	if (code == NULL) {
		return FALSE;
	}

	if (argc > 0) {
		int i = 0;

		// copy argv strings
		argv_copy = malloc(sizeof(char*) * argc);

		for (i = 0; i < argc; i++) {
			char* arg = (char*) mmu_get_physical_address(process_table[process_active], (unsigned int) argv[i]);
			if (arg != NULL) {

				int length = strlen(arg) + 1;
				argv_copy[i] = (char*) malloc(sizeof(char) * length);
				strcpy(argv_copy[i], arg);
				//      argv_copy[i] = (char*) malloc(sizeof(char) * (strlen("bla") + 1));
				//      strcpy(argv_copy[i], "bla");
			}
		}
	}

	*pid = process_create(priority, code, argv_copy[0], argc, argv_copy);

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
	int* physical = (int*) mmu_get_physical_address(process_table[process_active], physaddr);
	*physical = (unsigned int) virtual;
	return FALSE;
}

BOOLEAN swi_handle_sys_nr_of_process(unsigned int count) {

  int* ret = (int*) mmu_get_physical_address(process_table[process_active], count);
  *ret = process_count();
  return FALSE;
}

BOOLEAN swi_handle_sys_process_info(unsigned int mem, unsigned int count, unsigned int act_nr_of_pinfos) {

  pinfo_t* list = (pinfo_t*) mmu_get_physical_address(process_table[process_active], mem);
  int* c = (int*) mmu_get_physical_address(process_table[process_active], count);
  int* ac = (int*) mmu_get_physical_address(process_table[process_active], act_nr_of_pinfos);
  *ac = process_pinfo(list, *c);
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
		kernel_debug(FILE_UNKNOWN, "Cannot open file");
		*fd = SYSTEM_FD_INVALID;
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

BOOLEAN swi_handle_sys_diropen(int path_address, int dir_stat_address, int return_value_address) {
  const char* path = (const char*) mmu_get_physical_address(process_table[process_active], path_address);
  FL_DIR* dir = (FL_DIR*) mmu_get_physical_address(process_table[process_active], dir_stat_address);
  int* return_value = (int*) mmu_get_physical_address(process_table[process_active], return_value_address);
  *return_value = 0;
  if (fl_opendir(path, dir)) {
    *return_value = 1;
  }
  return FALSE;
}

BOOLEAN swi_handle_sys_dirclose(int dir_stat_address, int return_value_address) {
  FL_DIR* dir_stat = (FL_DIR*) mmu_get_physical_address(process_table[process_active], dir_stat_address);
  int* return_value = (int*) mmu_get_physical_address(process_table[process_active], return_value_address);
  *return_value = fl_closedir(dir_stat);
  return FALSE;
}

BOOLEAN swi_handle_sys_dirread(int dir_stat_address, int dir_entry_address, int return_value_address) {
  FL_DIR* dir_stat = (FL_DIR*) mmu_get_physical_address(process_table[process_active], dir_stat_address);
  fl_dirent* dir_entry = (fl_dirent*) mmu_get_physical_address(process_table[process_active], dir_entry_address);
  int* return_value = (int*) mmu_get_physical_address(process_table[process_active], return_value_address);
  *return_value = fl_readdir(dir_stat, dir_entry);
  return FALSE;
}

BOOLEAN swi_handle_sys_bind(int ns_address, int result_address) {
	const char* namespace = (const char*) mmu_get_physical_address(process_table[process_pid()], ns_address);

	int* result = (int*) mmu_get_physical_address(process_table[process_pid()], result_address);

	*result = ipc_bind(namespace, process_pid());

	return FALSE;
}

BOOLEAN swi_handle_sys_send(int ns_address, int msg_address, int result_address) {
	const char* namespace = (const char*) mmu_get_physical_address(process_table[process_pid()], ns_address);
	message_t* message = (message_t*) mmu_get_physical_address(process_table[process_pid()], msg_address);
	int* result = (int*) mmu_get_physical_address(process_table[process_pid()], result_address);
	void* content = (void*) mmu_get_physical_address(process_table[process_pid()], (unsigned int) message->content);

	*result = ipc_send_msg(namespace, *message, content, process_pid());

	return FALSE;
}

BOOLEAN sys_handle_sys_wait(int ns_address) {
	const char* namespace = (const char*) mmu_get_physical_address(process_table[process_pid()], ns_address);

	if (ipc_lookup_msg_concrete(namespace, process_pid()) == WAITING) {
		process_table[process_pid()]->state = BLOCKED;
		process_table[process_pid()]->blockstate = IPC_WAIT;

		return TRUE;
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_receive(int ns_address, int msg_address, int result_address) {
	const char* namespace = (const char*) mmu_get_physical_address(process_table[process_active], ns_address);
	message_t* message = (message_t*) mmu_get_physical_address(process_table[process_active], msg_address);
	int* result = (int*) mmu_get_physical_address(process_table[process_active], result_address);
	void* content = (void*) mmu_get_physical_address(process_table[process_active], (unsigned int) message->content);

	*result = ipc_receive_msg(namespace, message, content, process_active);

	return FALSE;
}

BOOLEAN swi_handle_sys_memory_info(int meminfo) {
  meminfo_t* info = (meminfo_t*) mmu_get_physical_address(process_table[process_active], meminfo);
  memory_info(info);
  return FALSE;
}

BOOLEAN swi_handle_sys_args_count(int argc_address) {
	int* argc = (int*) mmu_get_physical_address(process_table[process_active], argc_address);

	*argc = process_table[process_active]->argc;

	return FALSE;
}

BOOLEAN swi_handle_sys_args_length(int index, int length_address) {
	int* length = (int*) mmu_get_physical_address(process_table[process_active], length_address);

	*length = strlen(process_table[process_active]->argv[index]);
	// strlen ignores \0
	(*length)++;

	return FALSE;
}

BOOLEAN swi_handle_sys_args_value(int index, int argument_address) {
	char* argument = (char*) mmu_get_physical_address(process_table[process_active], argument_address);

	strcpy(argument, process_table[process_active]->argv[index]);

	return FALSE;
}

BOOLEAN swi_handle_sys_args_free() {

	int i = 0;
	int argc = process_table[process_active]->argc;

	for (i = 0; i < argc; i++) {
		free(process_table[process_active]->argv[i]);
	}

	free(process_table[process_active]->argv);
	process_table[process_active]->argc = 0;

	return FALSE;
}

BOOLEAN swi_handle(unsigned int syscall_nr, unsigned int param1, unsigned int param2, unsigned int param3) {

	// handle interrupts
	switch (syscall_nr) {
	case SYS_YIELD:
		return swi_handle_sys_yield();
	case SYS_EXIT:
	  // param1 = exit state of the process
		return swi_handle_sys_exit(param1);
	case SYS_EXEC:
		// param1 = parameters array
		return swi_handle_sys_create(param1);
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
	case SYS_DIROPEN:
	  // param1 = path
	  // param2 = dir
	  // param3 = return_value
	  return swi_handle_sys_diropen(param1, param2, param3);
	case SYS_DIRCLOSE:
	  // param1 = dir_stat
	  // param2 = return_value
	  return swi_handle_sys_dirclose(param1, param2);
	case SYS_DIRREAD:
	  // param1 = dir_stat
	  // param2 = dir_entry
	  // param3 = return_value
	  return swi_handle_sys_dirread(param1, param2, param3);
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
	case SYS_ARGS_COUNT:
		// param1 = return value (argument count)
		return swi_handle_sys_args_count(param1);
	case SYS_ARGS_VALUE_LENGTH:
		// param1 = argument index
		// param2 = return value (argument length)
		return swi_handle_sys_args_length(param1, param2);
	case SYS_ARGS_VALUE:
		// param1 = argument index
		// param2 = return value (argument)
		return swi_handle_sys_args_value(param1, param2);
	case SYS_ARGS_FREE:
		return swi_handle_sys_args_free();
  case SYS_NR_OF_PROCESS:
    // param1 = count as return value
    return swi_handle_sys_nr_of_process(param1);
  case SYS_PROCESS_INFO:
    // param1 = pinfo_t memory buffer
    // param2 = count
    // param3 = actual number of pinfo_t blocks read by syscall
    return swi_handle_sys_process_info(param1, param2, param3);
  case SYS_MEMORY_INFO:
    // param1 meminfo_t
    return swi_handle_sys_memory_info(param1);
	default:
		// unknown syscall number
		kernel_error(SWI_UNKNOWN_SYSCALL_NR, "Unknown syscall-number. Ignoring.");
		break;
	}

	return FALSE;
}
