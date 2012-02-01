/* types.h
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
 *  Created on: 21.10.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef OTTOS_TYPES_H_
#define OTTOS_TYPES_H_

#include <ottos/const.h>
#include <stdint.h>

#ifndef _SIZE_T
#define _SIZE_T
typedef uint32_t size_t;
#endif

typedef unsigned char byte_t;
typedef char BOOLEAN;

typedef int pid_t;
typedef volatile unsigned int mem_address_t;
// TODO (thomas.bargetz@gmail.com) use mem_address_t!
typedef volatile unsigned int* address;

typedef int (*function_t)();

typedef unsigned long address_t;
typedef address_t file_t;

/*
 * A message_t is a message to pass from some process to some other process.
 * It is used for the IPC module.
 * Systemcalls to use:
 *  receiver process: sys_bind(namespace), sys_wait_msg(namesapce),
 *    sys_receive(namespace, message_t)
 *  sender process: sys_send(namespace, message_t)
 *
 *  @see: systemcalls
 */
typedef struct message_t {
    int type;       /* the message type - to determine by process   */
    size_t size;       /* the size of one entry of the message content */
    int count;      /* the count of a message content's entry       */
    void* content;  /* the actual message content                   */
} message_t;

/*
 * process information struct
 */
#define PINFO_MAX_CMD_LENGTH 100
typedef struct pinfo_t {
    pid_t pid;            /* process pid */
    pid_t parent;
    int tty;              /* tty identifier */
    int stat;             /* process status (running, ) */
    int prio;             /* process priority */
    uint64_t time;        /* uptime of process */
    double mem;           /* memory consumed by process */
    char cmd[PINFO_MAX_CMD_LENGTH];  /* command line pattern to start process */
} pinfo_t;

typedef struct meminfo_t {
    double used_intram;
    double used_extddr;
    double total_intram;
    double total_extddr;
} meminfo_t;


typedef struct {
    int year;
    int month;
    int days;
    int hours;
    int minutes;
    int seconds;
    int miliseconds;
} time_t;

typedef enum system_file_type {
	DEVICE_FILE, NON_DEVICE_FILE
} system_file_type_t;

typedef struct {
  uint32_t sector;
  uint32_t cluster;
  uint8_t  offset;
} dir_t;

#ifndef FATFS_MAX_LONG_FILENAME
#  define FATFS_MAX_LONG_FILENAME 260
#endif

typedef struct {
  char     filename[FATFS_MAX_LONG_FILENAME];
  uint8_t  is_dir;
  uint32_t cluster;
  uint32_t size;
} dir_entry_t;

#endif /* OTTOS_TYPES_H_ */
