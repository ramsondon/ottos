/* error.h
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
 *  Created on: 24.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef ERROR_H_
#define ERROR_H_

#define ERROR_CODE unsigned int

#define SWI_UNKNOWN_SYSCALL_NR 1

#define PROCESS_UNKNOWN_FILE_DESCRIPTOR 10

#define DEVICE_UNKNOWN_ID 20

#define FILE_UNKNOWN 30

#define MMU_DATA_ABORT 40
#define MMU_DATA_ABORT_ILLEGAL 41
#define MMU_PREFETCH_ABORT 42

#define PARSER_ERROR 50

#define DRIVER_NO_ERROR 0
#define DRIVER_ERROR_CANNOT_OPEN 1
#define DRIVER_ERROR_CANNOT_CLOSE 2
#define DRIVER_ERROR_CANNOT_READ 3
#define DRIVER_ERROR_CANNOT_WRITE 4
#define DRIVER_ERROR_NOT_SUPPORTED 5

#endif /* ERROR_H_ */
