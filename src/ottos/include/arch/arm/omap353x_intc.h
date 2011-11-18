/* omap353x_intc.h
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
 *  Created on: Oct 30, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef OMAP353X_INTC_H_
#define OMAP353X_INTC_H_

/* macros for n and m multiplicative register of interrupt controller */
#define INTCPS_N_REG 0x20
#define INTCPS_M_REG 0x4
/* (mn times t) multiplication */
#define MULTI_REG(addr, mn, t) (addr + (mn * t))

/* interrupt controller base addresses */
#define MPU_INTC 0x48200000
#define MODEM_INTC 0x480C7000

/* mpu interrupt controller register address offset - all 32 bits width */
#define INTCPS_REVISION 0x0000  /* mpu base address */
#define INTCPS_SYSCONFIG 0x0010  /* RW 32 */
#define INTCPS_SYSSTATUS 0x0014 /* R 32 */
#define INTCPS_SIR_IRQ 0x0040
#define INTCPS_SIR_FIQ 0x0044
#define INTCPS_CONTROL 0x0048
#define INTCPS_PROTECTION 0x004C
#define INTCPS_IDLE 0x0050
#define INTCPS_IRQ_PRIORITY 0x0060
#define INTCPS_FIQ_PRIORITY 0x0064
#define INTCPS_THRESHOLD 0x0068

/* mpu n and m multiplicative registers base offset*/
#define INTCPS_ITR 0x0080
#define INTCPS_MIR 0x0084
#define INTCPS_MIR_CLEAR 0x0088
#define INTCPS_MIR_SET 0x008C
#define INTCPS_ISR_SET 0x0090
#define INTCPS_ISR_CLEAR 0x0094
#define INTCPS_PENDING_IRQ 0x0098
#define INTCPS_PENDING_FIQ 0x009C
#define INTCPS_ILR 0x0100

/*
 * multiplicative usage for m and n register
 * @param n 0..2
 * @param m 0..95
 */
#define INTCPS_ITRn(n) MULTI_REG(INTCPS_ITR, INTCPS_N_REG, n)
#define INTCPS_MIRn(n) MULTI_REG(INTCPS_MIR, INTCPS_N_REG, n)
#define INTCPS_MIR_CLEARn(n) MULTI_REG(INTCPS_MIR_CLEAR, INTCPS_N_REG, n)
#define INTCPS_MIR_SETn(n) MULTI_REG(INTCPS_MIR_SET, INTCPS_N_REG, n)
#define INTCPS_ISR_SETn(n) MULTI_REG(INTCPS_ISR_SET, INTCPS_N_REG, n)
#define INTCPS_ISR_CLEARn(n) MULTI_REG(INTCPS_ISR_CLEAR, INTCPS_N_REG, n)
#define INTCPS_PENDING_IRQn(n) MULTI_REG(INTCPS_PENDING_IRQ, INTCPS_N_REG, n)
#define INTCPS_PENDING_FIQn(n) MULTI_REG(INTCPS_PENDING_FIQ, INTCPS_N_REG, n)
#define INTCPS_ILRm(m) MULTI_REG(INTCPS_ILR, INTCPS_M_REG, m)

/* modem interrupt controller register address offset - all 32 bits width */
#define INTC_SYSCONFIG 0x0010 /* RW */
#define INTC_IDLE 0x0050 /* RW */

/* Number of IRQs */
#define IRQ_MAX_COUNT 95

/* IRQ IDs */
#define GPT1_IRQ      37
#define GPT2_IRQ      38
#define GPT3_IRQ      39
#define GPT4_IRQ      40
#define GPT5_IRQ      41
#define GPT6_IRQ      42
#define GPT7_IRQ      43
#define GPT8_IRQ      44
#define GPT9_IRQ      45
#define GPT10_IRQ     46
#define GPT11_IRQ     47

#endif /* OMAP353X_INTC_H_ */
