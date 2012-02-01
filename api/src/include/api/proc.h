/* proc.h
 * 
 * Copyright (c) 2011 The ottos_api project.
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
 *  Created on: Jan 27, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef API_PROC_H_
#define API_PROC_H_

#include <ottos/const.h>
#include <ottos/types.h>

/*
 * returns the number of process information entries and sets the pinfo_t pinfo
 * pointer.
 */
EXTERN uint32_t pinfo(pinfo_t* pinfo, uint32_t count);

/*
 * returns the number of running processes. the result will be at least 1 for
 * the reason that this process is the only one which would be running.
 */
EXTERN uint32_t pcount();

/*
 * maps a pinfo->stat entry to a readable char*
 */
EXTERN const char* pstate_readable(int stat);

/*
 * exits the current process
 */
EXTERN void pexit(int state);

#endif /* API_PROC_H_ */
