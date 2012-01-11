/* kernel.h
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
 *  Created on: 11.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <ottos/const.h>
#include <ottos/error.h>

/**
 * Must be executed without doing a function call. That is why this function
 * has to be inlined by the compiler.
 */
inline void kernel_to_user_mode() {
	asm("\t CPS 0x10");
}

/**
 * Print a kernel panic message and halt the system.
 */
EXTERN void kernel_panic(const char* str);

/**
 * Print a kernel message to STDOUT.
 */
EXTERN void kernel_print(const char* str);

/**
 * Print a kernel ERROR message to STDOUT.
 */
EXTERN void kernel_error(ERROR_CODE code, const char* message);

/**
 * Print a kernel debug message to STDOUT.
 */
EXTERN void kernel_debug(ERROR_CODE code, const char* message);

/**
 * Halt the system. This should only be called within a kernel_panic call
 * or at shutdown.
 */
EXTERN void kernel_halt();

/**
 * Sleep for a given time in milliseconds.
 */
EXTERN void kernel_sleep(int ms);

#endif /* KERNEL_H_ */
