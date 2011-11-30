/* io.h
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
 *  Created on: 04.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef OTTOS_IO_H_
#define OTTOS_IO_H_

#include <stdio.h>
#include <string.h>

#include <ottos/types.h>
#include <ottos/const.h>

#define STDIN    0
#define STDOUT   1
#define STDERR   2

EXTERN char tolower(char c);

EXTERN char toupper(char c);

EXTERN char* itoa(int n, char* s, int b);

EXTERN char* strrev(char* str);

#endif /* OTTOS_IO_H_ */
