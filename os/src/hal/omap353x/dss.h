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

#define DSS_BASE 0x48050000


/* Display subsystem register offsets */

#define DSS_REVISIONNUMBER    (DSS_BASE + 0x000)
#define DSS_SYSCONFIG         (DSS_BASE + 0x010)
#define DSS_SYSSTATUS         (DSS_BASE + 0x014)
#define DSS_IRQSTATUS         (DSS_BASE + 0x018)
#define DSS_CONTROL           (DSS_BASE + 0x040)
#define DSS_SDI_CONTROL       (DSS_BASE + 0x044)
#define DSS_PLL_CONTROL       (DSS_BASE + 0x048)
#define DSS_SDI_STATUS        (DSS_BASE + 0x05C)

#define DSS_AUTOIDLE                BIT0
#define DSS_SOFTRESET               BIT1
#define DSS_RESETDONE               BIT0
#define DSS_DSI_IRQ                 BIT1
#define DSS_DISPC_IRQ               BIT0
#define DSS_VENC_OUT_SEL            BIT6    /* set s-video */
#define DSS_DAC_POWERDN_BGZ         BIT5
#define DSS_DAC_DEMEN               BIT4
#define DSS_VENC_CLOCK_4X_ENABLE    BIT3
#define DSS_VENC_CLOCK_MODE         BIT2    /* set square pixel */
#define DSS_DSI_CLK_SWITCH          BIT1    /* select DSI2 PLL FCLK otherwise DSS1 ALWON FCLK */
#define DSS_DSS1_CLK_SWITCH         BIT0    /* same */


#endif /* HAL_OMAP353X_DSS_H_ */
