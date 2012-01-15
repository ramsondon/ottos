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

// TODO move fs to a different folder?
#include <vfat/fat_filelib.h>
#include <ottos/error.h>
#include <ottos/const.h>
#include <ottos/types.h>
#include <ottos/syscalls.h>
#include <ottos/kernel.h>
#include <ottos/dev/device.h>
#include <ottos/drivers/driver.h>
#include "../pm/process.h"
#include "../mmu/mmu.h"
#include "swi_device_handlers.h"

// forward declarations
BOOLEAN swi_handle_sys_fclose(int* error_code, process_file_descriptor_t* fd);
BOOLEAN swi_handle_sys_fwrite(process_file_descriptor_t* fd_process, const char* buffer, int buffer_size);
BOOLEAN swi_handle_sys_fread(process_file_descriptor_t* fd_process, char* buffer, int count);

BOOLEAN swi_handle_sys_yield() {
	return TRUE;
}

BOOLEAN swi_handle_sys_exit() {
	// delete the active process
	process_delete();
	return TRUE;
}

BOOLEAN swi_handle_sys_create(int priority, int executable_file_address, int block_current) {
	// get the real address of the executable_file_address
	const char* executable_file = (const char*) mmu_get_physical_address(process_table[process_active], executable_file_address);

	long file_length = 0;
	const char mode[] = { 'r' };

	// read executable file
	// the hex data is stored in 4 files
	// the filenames are executable_file.i0, executable_file.i1, executable_file.i2, executable_file.i3
	// where executable_file is the real filename or path
	const char* extension = ".i0";
	int extension_length = strlen(extension);
	int executable_filename_length = strlen(executable_file);
	int hex_filename_length = executable_filename_length + extension_length;

	const char* executable_filename_0 = malloc(sizeof(char) * hex_filename_length);
	const char* executable_filename_1 = malloc(sizeof(char) * hex_filename_length);
	const char* executable_filename_2 = malloc(sizeof(char) * hex_filename_length);
	const char* executable_filename_3 = malloc(sizeof(char) * hex_filename_length);

	void* executable_file_0;
	void* executable_file_1;
	void* executable_file_2;
	void* executable_file_3;

	strcpy(executable_filename_0, executable_file);
	strcat(executable_filename_0, ".i0");

	strcpy(executable_filename_1, executable_file);
	strcat(executable_filename_1, ".i1");

	strcpy(executable_filename_2, executable_file);
	strcat(executable_filename_2, ".i2");

	strcpy(executable_filename_3, executable_file);
	strcat(executable_filename_3, ".i3");

	executable_file_0 = fl_open(executable_filename_0, mode);
	executable_file_1 = fl_open(executable_filename_1, mode);
	executable_file_2 = fl_open(executable_filename_2, mode);
	executable_file_3 = fl_open(executable_filename_3, mode);

	if (executable_file_0 == NULL || executable_file_1 == NULL || executable_file_2 == NULL || executable_file_3 == NULL) {
		kernel_error(FILE_UNKNOWN, "Cannot open hex files for execution. One or more files doesn't exist");
	} else {

		// get the file length
		fl_fseek(executable_file_0, 0, SEEK_END);
		file_length = fl_ftell(executable_file_0);

		char* byte_file_0 = malloc(sizeof(char) * file_length);
		char* byte_file_1 = malloc(sizeof(char) * file_length);
		char* byte_file_2 = malloc(sizeof(char) * file_length);
		char* byte_file_3 = malloc(sizeof(char) * file_length);

		fl_fread(byte_file_0, file_length, file_length, executable_file_0);
		fl_fread(byte_file_1, file_length, file_length, executable_file_1);
		fl_fread(byte_file_2, file_length, file_length, executable_file_2);
		fl_fread(byte_file_3, file_length, file_length, executable_file_3);

		code_bytes_t code;
		code.byte_0 = byte_file_0; // data in executable_file.i0
		code.byte_1 = byte_file_1; // data in executable_file.i1
		code.byte_2 = byte_file_2; // data in executable_file.i2
		code.byte_3 = byte_file_3; // data in executable_file.i3

		process_create(priority, &code);

		free(code.byte_0);
		free(code.byte_1);
		free(code.byte_2);
		free(code.byte_3);
	}

	fl_fclose(executable_file_0);
	fl_fclose(executable_file_1);
	fl_fclose(executable_file_2);
	fl_fclose(executable_file_3);

	free(executable_filename_0);
	free(executable_filename_1);
	free(executable_filename_2);
	free(executable_filename_3);

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
			// set the return value to the error code
			*fd = device_error_code;
		} else {
			// the file descriptor is the device
			*fd = device;

			// add a new process file descriptor
			process_add_device_descriptor(*fd);
		}
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_close(int error_code_address, int fd) {
	// get the real address of the error code
	int* error_code = (int*) mmu_get_physical_address(process_table[process_active], error_code_address);

	// check if the file descriptor is a device or a file
	process_file_descriptor_t* fd_process = process_get_file_descriptor(fd);
	if (fd_process == NULL) {
		kernel_error(PROCESS_UNKNOWN_FILE_DESCRIPTOR, "Cannot close device or file");
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

BOOLEAN swi_handle_sys_write(int fd, int buffer_address, int buffer_size) {
	// get the real address of the buffer_address
	const char* buffer = (const char*) mmu_get_physical_address(process_table[process_active], buffer_address);

	// check if the file descriptor is a device or a file
	process_file_descriptor_t* fd_process = process_get_file_descriptor(fd);
	if (fd_process == NULL) {
		kernel_error(PROCESS_UNKNOWN_FILE_DESCRIPTOR, "Cannot write into device or file");
	} else {
		if (fd_process->type == DEVICE_FILE) {
			device_t device = (device_t) fd;
			driver_get(device)->write(device, buffer_size, (char*) buffer);
		} else {
			swi_handle_sys_fwrite(fd_process, buffer, buffer_size);
		}
	}

	return FALSE;
}

BOOLEAN swi_handle_sys_read(int fd, int buffer_address, int count) {
	// get the real address of the buffer_address
	char* buffer = (char*) mmu_get_physical_address(process_table[process_active], buffer_address);

	// check if the file descriptor is a device or a file
	process_file_descriptor_t* fd_process = process_get_file_descriptor(fd);
	if (fd_process == NULL) {
		kernel_error(PROCESS_UNKNOWN_FILE_DESCRIPTOR, "Cannot read from device or file");
	} else {
		if (fd_process->type == DEVICE_FILE) {
			device_t device = (device_t) fd;
			driver_get(device)->read(device, count, buffer);
		} else {
			swi_handle_sys_fread(fd_process, buffer, count);
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

BOOLEAN swi_handle_sys_fwrite(process_file_descriptor_t* fd_process, const char* buffer, int buffer_size) {

	fl_fwrite(buffer, buffer_size, buffer_size, fd_process->file);

	return FALSE;
}

BOOLEAN swi_handle_sys_fread(process_file_descriptor_t* fd_process, char* buffer, int count) {

	fl_fread(buffer, count, count, fd_process->file);

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
		// param2 = file
		// param3 = block_current
		return swi_handle_sys_create(param1, param2, param3);
	case SYS_OPEN:
		// param1 = return value (fd)
		// param2 = device id
		// param3 = flags
		return swi_handle_sys_open(param1, param2, param3);
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
	default:
		// unknown syscall number
		kernel_error(SWI_UNKNOWN_SYSCALL_NR, "Unknown syscall-number. Ignoring.");
		break;
	}

	return FALSE;
}
