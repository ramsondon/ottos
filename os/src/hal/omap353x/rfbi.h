/* rfbi.h
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
 *  Created on: Dec 15, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef HAL_OMAP3530X_RFBI_H_
#define HAL_OMAP3530X_RFBI_H_

/*
 * Remote frame buffer interface
 */

/* RFBI base address */
#define RFBI 0x48050800

/* RFBI register offset */

#define RFBI_REVISION       0x00
#define RFBI_SYSCONFIG      0x10
#define RFBI_SYSSTATUS      0x14
#define RFBI_CONTROL        0x40
#define RFBI_PIXEL_CNT      0x44
#define RFBI_LINE_NUMBER    0x48
#define RFBI_CMD            0x4C
#define RFBI_PARAM          0x50
#define RFBI_DATA           0x54
#define RFBI_READ           0x58
#define RFBI_STATUS         0x5C


/* i registers range from i = 0..1 */

#define RFBI_CONFIG(i)      (0x60 + (i* 0x18))
#define RFBI_ONOFF_TIME(i)  (0x64 + (i* 0x18))
#define RFBI_CYCLE_TIME(i)  (0x68 + (i* 0x18))
#define RFBI_DATA_CYCLE1(i) (0x6C + (i* 0x18))
#define RFBI_DATA_CYCLE2(i) (0x70 + (i* 0x18))
#define RFBI_DATA_CYCLE3(i) (0x74 + (i* 0x18))

#define RFBI_VSYNC_WIDTH  0x90
#define RFBI_HSYNC_WIDTH  0x94


#endif /* HAL_OMAP3530X_RFBI_H_ */
