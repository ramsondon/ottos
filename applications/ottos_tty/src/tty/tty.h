/* tty.h
 * 
 * Copyright (c) 2011 The tty project.
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
 *  Created on: Dec 22, 2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef TTY_H_
#define TTY_H_

#include <ottos/const.h>

#define HOSTNAME "beagleboard"
#define USERNAME "root"
#define TTY_NAME "ottsh"

#define MAX_PATH_LENGTH             100 // this is optimistic that our paths do not exceed 1024 chars
#define SPLIT_CHARS                 " " // may be an array of characters to split CMDs
#define DIRECTORY_SEPARATOR         '/' // must be a single char
#define START_IN_BACKGROUND_SYSMBOL '&' // must be a single char

#define HOME_DIRECTORY              "/root" // since we do not have any other users, this seems to be OK
#define BIN_DIRECTORY               "/bin" // anything else does not make any sense
#define LOG_DIRECTORY               "/var/log" // again, this is standard UNIX

EXTERN void tty_run();

#endif /* TTY_H_ */
