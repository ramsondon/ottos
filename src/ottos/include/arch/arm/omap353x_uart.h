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
#define UART_RHR_REG        0x000 /* R FIFO read */
#define UART_THR_REG        0x000 /* W FIFO transmission */
#define UART_DLH_REG        0x004 /* RW */
#define UART_IER_REG        0x004 /* RW */

#define UART_IER_RHR_IT       0
#define UART_IER_THR_IT       1
#define UART_IER_LINE_STS_IT  2
#define UART_IER_MODEM_STS_IT 3
#define UART_IER_SLEEP_MODE   4
#define UART_IER_XOFF_IT      5
#define UART_RTS_IT           6
#define UART_CTS_IT           7

#define UART_IIR_REG        0x008 /* R */
#define UART_FCR_REG        0x008 /* W FIFO Control */

#define UART_FCR_FIFO_EN        0
#define UART_FCR_RX_FIFO_CLEAR  1
#define UART_FCR_TX_FIFO_CLEAR  2
#define UART_FCR_DMA_MODE       3

// TODO: delete the following lines and replace them by the commented
// UART_FCR_RX_TRIG_xx etc.
#define UART_FCR_TX_FIFO_TRIG_1 4
#define UART_FCR_TX_FIFO_TRIG_2 5
#define UART_FCR_RX_FIFO_TRIG_1 6
#define UART_FCR_RX_FIFO_TRIG_2 7
// TODO: find a way to use flags in the following way
//#define UART_FCR_RX_TRIG_00      0x00
//#define UART_FCR_RX_TRIG_01      0x40
//#define UART_FCR_RX_TRIG_10      0x80
//#define UART_FCR_RX_TRIG_11      0xC0
//#define UART_FCR_TX_TRIG_00      0x00
//#define UART_FCR_TX_TRIG_01      0x10
//#define UART_FCR_TX_TRIG_10      0x20
//#define UART_FCR_TX_TRIG_11      0x30


#define UART_EFR_REG        0x008 /* RW */

/* bit specification of the UART EFR register */
#define UART_EFR_SWFC_0       0
#define UART_EFR_SWFC_1       1
#define UART_EFR_SWFC_2       2
#define UART_EFR_SWFC_3       3
#define UART_EFR_ENHANCED_EN  4
#define UART_EFR_SPEC_CHAR    5
#define UART_EFR_AUTO_RTS_EN  6
#define UART_EFR_AUTO_CTS_EN  7

#define UART_LCR_REG        0x00C /* RW */

/* UART LCR Configuration mode */
#define UART_LCR_MODE_CONFIG_A 0x0080
#define UART_LCR_MODE_CONFIG_B 0x00BF
#define UART_LCR_MODE_REG_OP   0x0000
/* UART LCR register bits*/
#define UART_LCR_CHAR_LEN_0 0 /* 0 and 1 bit */
#define UART_LCR_CHAR_LEN_1 1
#define UART_LCR_NB_STOP    2
#define UART_LCR_PARITY_EN  3
#define UART_LCR_PARITY_T1  4
#define UART_LCR_PARITY_T2  5
#define UART_LCR_BREAK_EN   6
#define UART_LCR_DIV_EN     7


#define UART_MCR_REG        0x010 /* RW Modem Control */

#define UART_MCR_TCR_TLR    6
#define UART_MCR_LOOPBACK_EN 0x10


#define UART_XON1_ADDR1_REG 0x010 /* RW */
#define UART_LSR_REG        0x014 /* R */

#define UART_LSR_RX_FIFO_E  0 /* 0 BIT */
#define UART_LSR_TX_FIFO_E  5 /* 5 BIT */
#define UART_LSR_TX_SR_E    6 /* 6 BIT */


#define UART_XON2_ADDR2_REG 0x014 /* RW */
#define UART_MSR_REG        0x018 /* R */
#define UART_TCR_REG        0x018 /* RW */
#define UART_XOFF1_REG      0x018 /* RW */
#define UART_SPR_REG        0x01C /* RW */
#define UART_TLR_REG        0x01C /* RW */
#define UART_XOFF2_REG      0x01C /* RW */
#define UART_MDR1_REG       0x020 /* RW Mode Definition CIR/IrDA */

/* UART Mode Select */
#define UART_MDR1_MODE_SELECT_16X          0x000
#define UART_MDR1_MODE_SELECT_SIR          0x001
#define UART_MDR1_MODE_SELECT_16X_AUTOBAUD 0x002
#define UART_MDR1_MODE_SELECT_13X          0x003
#define UART_MDR1_MODE_SELECT_MIR          0x004
#define UART_MDR1_MODE_SELECT_FIR          0x005
#define UART_MDR1_MODE_SELECT_CIR          0x006
#define UART_MDR1_MODE_SELECT_DISABLE      0x007

#define UART_MDR2_REG       0x024 /* RW */
#define UART_SFLSR_REG      0x028 /* R */
#define UART_UASR_REG       0x038 /* R */
#define UART_BLR_REG        0x038 /* RW */

#define UART_SCR_REG        0x040 /* RW Supplementary Control */

#define UART_SCR_DMA_MODE_CTL   0
#define UART_SCR_DMA_MODE_2_0   1
#define UART_SCR_DMA_MODE_2_1   2
#define UART_SCR_RX_TRIG_GRANU1 7
#define UART_SCR_TX_TRIG_GRANU1 6

#define UART_SSR_REG        0x044 /* R */

#define UART_SSR_TX_FIFO_FULL 0

#define UART_MVR_REG        0x050 /* R */
#define UART_SYSC_REG       0x054 /* RW */
#define UART_SYSS_REG       0x058 /* R */
#define UART_WER_REG        0x05C /* RW */


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
