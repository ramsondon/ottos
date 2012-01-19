/* i2c.h
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
 *  Created on: Dec 14, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef HAL_OMAP353X_I2C_H_
#define HAL_OMAP353X_I2C_H_

/* I2C base addresses */

#define I2C1_BASE 0x48070000
#define I2C2_BASE 0x48072000
#define I2C3_BASE 0x48060000


/* I2C register offsets */

#define I2C_REV     0x00  /* revision register */
#define I2C_IE      0x04
#define I2C_STAT    0x08
#define I2C_WE      0x0C
#define I2C_SYSS    0x10
#define I2C_BUF     0x14
#define I2C_CNT     0x18
#define I2C_DATA    0x1C
#define I2C_SYSC    0x20
#define I2C_CON     0x24
#define I2C_OA0     0x28
#define I2C_SA      0x2C
#define I2C_PSC     0x30
#define I2C_SCLL    0x34
#define I2C_SCLH    0x38
#define I2C_SYSTEST 0x3C
#define I2C_BUFSTAT 0x40
#define I2C_OA1     0x44
#define I2C_OA2     0x48
#define I2C_OA3     0x4C
#define I2C_ACTOA   0x50
#define I2C_SBLOCK  0x54

// I2C_STAT bits
#define I2C_STAT_XDR (1<<14)
#define I2C_STAT_RDR (1<<13)
#define I2C_STAT_BB (1<<12)
#define I2C_STAT_XRDY (1<<4)
#define I2C_STAT_RRDY (1<<3)
#define I2C_STAT_ARDY (1<<2)
#define I2C_STAT_NACK (1<<1)
#define I2C_STAT_AL (1<<0)
// I2C_CON bits
#define I2C_CON_MST (1<<10)
#define I2C_CON_TRX (1<<9)
#define I2C_CON_STP 2
#define I2C_CON_STT 1



#endif /* HAL_OMAP353X_I2C_H_ */
