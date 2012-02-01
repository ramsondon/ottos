/* syscalls.h
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

#ifndef SYSCALLS_H_
#define SYSCALLS_H_

/*
 * Process system calls
 */
#define SYS_YIELD             1
#define SYS_EXIT              2
#define SYS_EXEC	            3
#define SYS_NR_OF_PROCESS     4
#define SYS_PHYSICAL_ADDRESS  5
#define SYS_PROCESS_INFO      6

/**
 * I/O system calls
 */
#define SYS_OPEN    10
#define SYS_CLOSE   11
#define SYS_READ    12
#define SYS_WRITE   13



/**
 * File system calls
 */
#define SYS_FOPEN    20
// directory operations
#define SYS_DIROPEN  21
#define SYS_DIRCLOSE 22
#define SYS_DIRREAD  23

/*
 * IPC system calls
 */
#define SYS_BIND_NAMESPACE  30
#define SYS_SEND            31
#define SYS_WAIT_MSG        32
#define SYS_RECEIVE         33

/**
 * WTF system calls
 */
#define SYS_ARGS_COUNT        40
#define SYS_ARGS_VALUE_LENGTH 41
#define SYS_ARGS_VALUE        42
#define SYS_ARGS_FREE         43

/*
 * MEMORY system calls
 */
#define SYS_MEMORY_INFO 50

#endif /* SYSCALLS_H_ */
