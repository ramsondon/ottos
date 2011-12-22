/* dss.h
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
 *  Created on: Dec 16, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef HAL_OMAP353X_DSS_H_
#define HAL_OMAP353X_DSS_H_

/* Display subsystem base address */

#define DSS 0x48050000


/* Display subsystem register offsets */

#define DSS_REVISIONNUMBER  0x000
#define DSS_SYSCONFIG       0x010
#define DSS_SYSSTATUS       0x014
#define DSS_IRQSTATUS       0x018
#define DSS_CONTROL         0x040
#define DSS_SDI_CONTROL     0x044
#define DSS_PLL_CONTROL     0x048
#define DSS_SDI_STATUS      0x05C


#endif /* HAL_OMAP353X_DSS_H_ */
