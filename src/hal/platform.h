/* platform.h
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
 *  Created on: Dec 2, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef HAL_PLATFORM_H_
#define HAL_PLATFORM_H_

/* *****************************************************************************
 * XXX: TO NOTIFY BY THE PROGRAMMER
 * *****************************************************************************
 *
 * This is the main include file for the OttOS hardware abstraction layer.
 * All modules which require one part of the OttOS HAL must only include this
 * header file
 *
 *
 * To get this piece to work we have to configure the compiler settings.
 * Add the following definition to the command line pattern of your compiler:
 * -DOMAP353X
 */
#ifdef OMAP353X
#include "omap353x/platform.h"
#endif

#endif /* HAL_PLATFORM_H_ */
