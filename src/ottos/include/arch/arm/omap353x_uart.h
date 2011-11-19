/* omap353x_uart.h
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
 *  Created on: Nov 18, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef OMAP353X_UART_H_
#define OMAP353X_UART_H_

/*
 * UART module base addresses
 * Size 1 Kb
 */
#define UART1 0x4806A000 /* UART only */
#define UART2 0x4806C000 /* UART only */
#define UART3 0x49020000 /* UART, IrDA, CIR */

/*
 * UART Registers offset
 * 8 bit width
 */
#define UART_DLL_REG        0x000 /* RW */
#define UART_RHR_REG        0x000 /* R */
#define UART_THR_REG        0x000 /* W */
#define UART_DLH_REG        0x004 /* RW */
#define UART_IER_REG        0x004 /* RW */
#define UART_IIR_REG        0x008 /* R */
#define UART_FCR_REG        0x008 /* W */
#define UART_EFR_REG        0x008 /* RW */

#define UART_EFR_ENHANCED_EN 4

#define UART_LCR_REG        0x00C /* RW */

/* UART Configuration mode */
#define UART_LCR_CONFIG_MODE_B 0x00BF
#define UART_LCR_REG_OP_MODE   0x0000

/* UART LCR register bits*/
#define UART_LCR_CHAR_LEN   0 /* 0 and 1 bit */
#define UART_LCR_NB_STOP    2
#define UART_LCR_PARITY_EN  3
#define UART_LCR_PARITY_T1  4
#define UART_LCR_PARITY_T2  5
#define UART_LCR_BREAK_EN   6
#define UART_LCR_DIV_EN     7


#define UART_MCR_REG        0x010 /* RW */
#define UART_XON1_ADDR1_REG 0x010 /* RW */
#define UART_LSR_REG        0x014 /* R */
#define UART_XON2_ADDR2_REG 0x014 /* RW */
#define UART_MSR_REG        0x018 /* R */
#define UART_TCR_REG        0x018 /* RW */
#define UART_XOFF1_REG      0x018 /* RW */
#define UART_SPR_REG        0x01C /* RW */
#define UART_TLR_REG        0x01C /* RW */
#define UART_XOFF2_REG      0x01C /* RW */
#define UART_MDR1_REG       0x020 /* RW Mode Definition CIR/IrDA */
#define UART_MDR2_REG       0x024 /* RW */
#define UART_SFLSR_REG      0x028 /* R */
#define UART_UASR_REG       0x038 /* R */
#define UART_BLR_REG        0x038 /* RW */

#define UART_SCR_REG        0x040 /* RW */
#define UART_SSR_REG        0x044 /* R */

#define UART_MVR_REG        0x050 /* R */
#define UART_SYSC_REG       0x054 /* RW */
#define UART_SYSS_REG       0x058 /* R */
#define UART_WER_REG        0x05C /* RW */



/* UART Mode Select */
#define UART_MODE_SELECT_16X          0x000
#define UART_MODE_SELECT_SIR          0x001
#define UART_MODE_SELECT_16XAUTO_BAUD 0x002
#define UART_MODE_SELECT_13XMODE      0x003
#define UART_MODE_SELECT_MIR          0x004
#define UART_MODE_SELECT_FIR          0x005
#define UART_MODE_SELECT_CIR          0x006
#define UART_MODE_SELECT_DISABLE      0x007




/*
 * UART3 specific registers offset
 * 8 bit width
 */
#define UART3_TXFLL_REG     0x028 /* W */
#define UART3_RESUME_REG    0x02C /* R */
#define UART3_TXFLH_REG     0x02C /* W */
#define UART3_SFREGL_REG    0x030 /* R */
#define UART3_RXFLL_REG     0x030 /* W */
#define UART3_SFREGH_REG    0x034 /* R */
#define UART3_RXFLH_REG     0x034 /* W */
#define UART3_ACREG_REG     0x03C /* RW */
#define UART3_EBLR_REG      0x048 /* RW */
#define UART3_CFPS_REG      0x060 /* RW */





#endif /* OMAP353X_UART_H_ */
