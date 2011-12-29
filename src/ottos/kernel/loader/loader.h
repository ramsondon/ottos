/* loader.h
 *
 * Copyright (c) 2011 The ottos project. Inspired by the BOSS project.
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
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#ifndef LOADER_H_
#define LOADER_H_

#include <ottos/code.h>
#include "../pm/process.h"

void load_process_code(process_t* process, code_bytes_t* code_bytes);

#endif /* LOADER_H_ */
