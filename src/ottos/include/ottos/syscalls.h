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
#define SYS_YIELD   1

/**
 * I/O system calls
 */
#define SYS_OPEN    2
#define SYS_CLOSE   3
#define SYS_READ    4
#define SYS_WRITE   5

/*
 * IPC system calls
 */
#define SYS_SEND    6
#define SYS_RECEIVE 7

#endif /* SYSCALLS_H_ */
