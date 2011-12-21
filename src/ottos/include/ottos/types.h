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

typedef char BOOLEAN;

typedef int pid_t;
typedef volatile unsigned int mem_address_t;
typedef volatile unsigned int* address;

typedef int (*function_t)();

typedef unsigned long address_t;
typedef address_t file_t;

typedef struct message_t {
    int pid_t;
} message_t;




#endif /* OTTOS_TYPES_H_ */
