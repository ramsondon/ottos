/* sd_mmc.c
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

#include "sd_mmc.h"
#include <bits.h>
#include <ottos/types.h>

void sd_mmc_init() {
  /*** SET CLOCKS  //  PAGE 3160 ***/
  /* enable interface clock */
  SET_BIT(CM_ICLKEN1_CORE, CM_CLKEN1_MMC1);

  /* enable functional clock */
  SET_BIT(CM_FCLKEN1_CORE, CM_CLKEN1_MMC1);

  /*** SOFTRESET  //  PAGE 3161 ***/
  /* do software reset for MMC1 module */
  SET_BIT(MMCHS_SYSCONFIG_MMC1, 1);

  /* TODO: wait until MMCHS_SYSSTATUS[0] RESETDONE bit is 0x1 */
  //while (READ_BIT(MMCHS_SYSSTATUS_MMC1, 0) == 0x1) {}


  /*** SET DEFAULT CAPABILITIES  //  PAGE 3161 ***/
  /* set 1.8 voltage as supported */
  SET_BIT(MMCHS_CAPA_MMC1, 26);
  /* set 3.0 voltage as supported */
  SET_BIT(MMCHS_CAPA_MMC1, 25);
  /* disable support for 3.3 voltage */
  CLEAR_BIT(MMCHS_CAPA_MMC1, 24);
  /* TODO: set CUR_CAPA ???????? */


  /*** WAKE-UP CONFIGURATION  //  PAGE 3162 ***/
  /* enable ENAWAKEUP */
  SET_BIT(MMCHS_SYSCONFIG_MMC1, 2);
  /* enable interrupt wakeup */
  SET_BIT(MMCHS_HCTL_MMC1, 24);


  /*** CONTROLLER BUS CONFIGURATION  //  PAGE 3163 ***/
  /* Controller Default Initialization  //  PAGE 3178 */
  *(MMCHS_HCTL_MMC1) |= (0x5 << 9); /* set SDVS to 1.8 Volt --> set MMCHS1_HCTL[11:9] = 0x5 */
  SET_BIT(MMCHS_CON_MMC1, 0); /* set OD / open drain */
  SET_BIT(MMCHS_HCTL_MMC1, 1); /* set DTW / data bus width to 1 */
  SET_BIT(MMCHS_SYSCTL_MMC1, 0); /* enable internal clock */
  SET_BIT(MMCHS_SYSCTL_MMC1, 2); /* enable internal clock */
  SET_BIT(MMCHS_SYSCTL_MMC1, 8); /* bus power up SDBP */
  *MMCHS_SYSCTL_MMC1 = 0x0000A007; /* set clock frequency to 150 kHz */

  // TODO: check if MMCHS_HTCL[8] SDBP bit is 0x1 !


  /*** INIT PROCEDURE START  //  PAGE 3179 ***/
  *MMCHS_CON_MMC1 |= 0x00000002;
  *MMCHS_CMD_MMC1 = 0x00000000;
  /* enable input buffer for the clock output */
  SET_BIT(CONTROL_PADCONF_MMC1_CLK, 8);


  /*** PRE-CARD IDENTIFICATION CONFIGURATION ***/
  *MMCHS_HCTL_MMC1 = 0x00000B00;
  *MMCHS_SYSCTL_MMC1 = 0x00003C07;
  *MMCHS_CON_MMC1 = 0x00000001;
}

void sd_mmc_identification() {
  /* module have to be initialized first!!! --> sd_mmc_init() */

  SET_BIT(MMCHS_CON_MMC1, 1);

  *MMCHS_CMD_MMC1 = 0x00000000;

  /* TODO: wait 1ms */

  SET_BIT(MMCHS_STAT_MMC1, 0);

  CLEAR_BIT(MMCHS_CON_MMC1, 1);

  *MMCHS_STAT_MMC1 = 0xFFFFFFFF;

  sd_mmc_sendCMD0();

  sd_mmc_sendCMD5();

  /* MMCHS_STAT CC Bit should be 0x1  -->  the card is an SDIO card */
  /* MMCHS_STAT CTO Bit should be 0x1  -->  the card is an SD card */

  SET_BIT(MMCHS_SYSCTL_MMC1, 25);
  /* Wait until MMCHS_SYSCTL[25] is 0x0 */

  sd_mmc_sendCMD8();

  /* MMCHS_STAT CC == 0x0 ?  -->  no, we use an SD-1.1 card */
  /* MMCHS_STAT CTO Bit should be 0x1  -->  the card is an SD card */

  SET_BIT(MMCHS_SYSCTL_MMC1, 25);
  // WTF go on with the flow diagram page 3165
}

/* This command resets the MMC card. */
void sd_mmc_sendCMD0() {
  *(MMCHS_CON_MMC1) = 0x00000001;
  *(MMCHS_IE_MMC1) = 0x00040001;
  *(MMCHS_ISE_MMC1) = 0x00040001;
  *(MMCHS_CMD_MMC1) = 0x00000000;
}

/* This command asks a SDIO card to send its operating conditions. */
void sd_mmc_sendCMD5() {
  *(MMCHS_CON_MMC1) = 0x00000001;
  *(MMCHS_IE_MMC1) = 0x00050001;
  *(MMCHS_ISE_MMC1) = 0x00050001;
  *(MMCHS_CMD_MMC1) = 0x05020000;
}

/* This command asks a SD card version 2.X to send its operating conditions. */
void sd_mmc_sendCMD8() {
  *(MMCHS_CON_MMC1) = 0x00000001;
  *(MMCHS_IE_MMC1) = 0x100F0001;
  *(MMCHS_ISE_MMC1) = 0x100F0001;
  *(MMCHS_CMD_MMC1) = 0x081A0000; // ---> lt. Fam. Guide: 0x81A0000 nur 7 Byte!!!
}

/* This command asks a SD card version 1.X to send its operating conditions. */
void sd_mmc_sendCMD55() {
  *(MMCHS_CON_MMC1) = 0x00000001;
  *(MMCHS_IE_MMC1) = 0x100F0001;
  *(MMCHS_ISE_MMC1) = 0x100F0001;
  *(MMCHS_CMD_MMC1) = 0x371A0000;
}
