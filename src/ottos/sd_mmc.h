/* sd_mmc.h
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
 *  Created on: 19.11.2011
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#ifndef SD_MMC_H_
#define SD_MMC_H_

#include <ottos/const.h>

#define CM_ICLKEN1_CORE   (mem_address_t*)0x48004A10        /* register in which the interface clock is configured */
#define CM_FCLKEN1_CORE   (mem_address_t*)0x48004A00        /* register in which the functional clock is configured */
#define CONTROL_PADCONF_MMC1_CLK  (mem_address_t*)0x48002144

#define CM_CLKEN1_MMC1    24                                /* bit which sets the interface clock for MMC1 */

#define MMCHS_SYSCONFIG_MMC1    (mem_address_t*)0x4809C010
#define MMCHS_SYSSTATUS_MMC1    (mem_address_t*)0x4809C014

#define MMCHS_CON_MMC1          (mem_address_t*)0x4809C02C
#define MMCHS_CMD_MMC1          (mem_address_t*)0x4809C10C
#define MMCHS_HCTL_MMC1         (mem_address_t*)0x4809C128
#define MMCHS_SYSCTL_MMC1       (mem_address_t*)0x4809C12C
#define MMCHS_STAT_MMC1         (mem_address_t*)0x4809C130
#define MMCHS_IE_MMC1           (mem_address_t*)0x4809C134
#define MMCHS_ISE_MMC1          (mem_address_t*)0x4809C138
#define MMCHS_CAPA_MMC1         (mem_address_t*)0x4809C140       /* register to set the supported voltage levels */





EXTERN void sd_mmc_init();
EXTERN void sd_mmc_sendCMD0();
EXTERN void sd_mmc_sendCMD5();
EXTERN void sd_mmc_sendCMD8();

#endif /* SD_MMC_H_ */
