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

#ifndef API_SYSTEM_H_
#define API_SYSTEM_H_

#include <ottos/types.h>
#include <ottos/const.h>

#define SYSTEM_DEV_PATH  "/dev"
#define SYSTEM_DEV_ID_INVALID -1

#define SYSTEM_SERIAL_0_PATH SYSTEM_DEV_PATH "/serial0"

#define SYSTEM_TEMP_0_PATH SYSTEM_DEV_PATH "/temp0"
#define SYSTEM_PRESSURE_0_PATH SYSTEM_DEV_PATH "/pressure0"
#define SYSTEM_SOLAR_0_PATH SYSTEM_DEV_PATH "/solar0"

#define SYSTEM_RTC_0_PATH SYSTEM_DEV_PATH "/rtc0"

#define SYSTEM_LED_0_PATH SYSTEM_DEV_PATH "/led0"
#define SYSTEM_LED_1_PATH SYSTEM_DEV_PATH "/led1"

#define SYSTEM_VIDEO_0_PATH SYSTEM_DEV_PATH "/video0"

// binary flags. increase new flag number in binary order (e.g.: 1, 2, 4, 8, ...)
#define SYSTEM_FLAG_READ 1
#define SYSTEM_FLAG_WRITE 2

EXTERN int sys_open(const char* path, int flags);
EXTERN size_t sys_read(int fd, const char* buffer, size_t count);
EXTERN size_t sys_write(int fd, const char* buffer, size_t nbytes);
EXTERN int sys_close(int fd);

EXTERN unsigned int sys_physical_address_of(const void* address);

EXTERN int sys_execute(int priority, BOOLEAN block_current, const char* path, int argc, char** argv);

EXTERN unsigned int sys_pcount();

EXTERN int sys_pinfo(pinfo_t* mem, int count);
/**
 * IPC system calls
 */
/*
 * Binds a namespace of a sending process. Must be called before sending.
 * @param success - output parameter if the the method succeeded
 */
EXTERN void sys_bind(const char* ns, int* success);

/*
 * Sends a message to namespace ns. sys_bind must be called first. The method
 * will be handled successfully if a receiving process is running, else it will
 * result in an error.
 * @param success - output parameter if the the method succeeded
 */
EXTERN void sys_send(const char* ns, message_t* msg, int* success);

/*
 * Waits for an IPC message.
 * The receive method has to be called afterwards to receive the correct message
 */
EXTERN void sys_wait_msg(const char* ns);

/*
 * Receives a message_t sent to namespace ns of a sending process. sys_wait_msg
 * must be called first.
 * @param success - output parameter if the the method succeeded
 */
EXTERN void sys_receive(const char* ns, message_t* msg, int* success);

/*
 * Reads the arguments of a process and returns the arguments
 * @param argc - the read argument count
 */
EXTERN char** sys_read_arguments(int* argc);

/*
 * Deletes the active process (the caller)
 */
EXTERN void sys_exit(int state);

/**
 * Open a directory for listing files.
 *
 * @param path - the path to be opened
 * @param dir - the reference to the directory status struct
 */
EXTERN int sys_opendir(const char* path, dir_t* dir);

/**
 * Close a directory after reading.
 *
 * @param dir_stat - the directory to close
 */
EXTERN int sys_closedir(dir_t* dir_stat);

/**
 * Read the next directory entry in a previously opened directory.
 * @param dir_stat - the reference to the opened directory
 * @param dir_entry - the reference of the entry in which to read
 */
EXTERN int sys_readdir(dir_t* dir_stat, dir_entry_t* dir_entry);

/**
 * Returns the curren time
 */
EXTERN time_t sys_get_time();

#endif /* API_SYSTEM_H_ */
