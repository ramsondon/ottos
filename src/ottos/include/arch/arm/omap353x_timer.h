/* timer.h
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
 *  Created on: Oct 28, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 *
 *
 *  This header file is hardware dependent to the OMAP3530 Processor
 */


#ifndef OMAP353X_TIMER_H_
#define OMAP353X_TIMER_H_

#define GPTIMER_COUNT 11

#define CLOCK_FREQUENZE 32.768

/* OMAP3530 General Purpose Timer Base Addresses */
#define GPTIMER1_BASE  0x48318000
#define GPTIMER2_BASE  0x49032000
#define GPTIMER3_BASE  0x49034000
#define GPTIMER4_BASE  0x49036000
#define GPTIMER5_BASE  0x49038000
#define GPTIMER6_BASE  0x4903A000
#define GPTIMER7_BASE  0x4903C000
#define GPTIMER8_BASE  0x4903E000
#define GPTIMER9_BASE  0x49040000
#define GPTIMER10_BASE 0x48086000
#define GPTIMER11_BASE 0x48088000

/* General Purpose Timer Registers */
#define GPT_TIDR      0x00        /* Base Address */
#define GPT_TIOCP_CFG 0x10
#define GPT_TISTAT    0x14
#define GPT_TISR      0x18        /* Pending interrupts in the module */
#define GPT_TIER      0x1C        /* controls enable/disable interrupt events */
#define GPT_TWER      0x20
#define GPT_TCLR      0x24        /* Timer counter load register --> value which is loaded into TCRR when the timer is reseted */
#define GPT_TCRR      0x28        /* Timer counter register */
#define GPT_TLDR      0x2C        /* Timer Load Register */
#define GPT_TTGR      0x30
#define GPT_TWPS      0x34
#define GPT_TMAR      0x38
#define GPT_TCAR1     0x3C
#define GPT_TSICR     0x40
#define GPT_TCAR2     0x44
#define GPT_TPIR      0x48        /* Timer positive increment --> used to correct the tick time so that ms ticks are correct */
#define GPT_TNIR      0x4C        /* Timer negative increment --> used to correct the tick time so that ms ticks are correct */
#define GPT_TCVR      0x50
#define GPT_TOCR      0x54        /* Timer overflow counter register --> in this register the overflows are counted */
#define GPT_TOWR      0x58        /* Timer overflow match --> if this register matches with the count in TOCR an interrupt occurs */

/* TIER / TISR flags */
#define TISR_CAPTURE  2         /* capture interrupts */
#define TISR_OVERFLOW 1         /* overflow interrupts */
#define TISR_MATCH    0         /* match interrupts */

/* TCLR flags */
#define TCLR_PRESCALE     5
#define TCLR_TRG_OVF      10
#define TCLR_TRG_OVF_MAT  11
#define TCLR_COMPARE      6
#define TCLR_AUTORELOAD   1
#define TCLR_START        0

/* Register in which the clock selection for GPT2 - GPT9 could be made */
#define CM_CLKSEL_PER   0x48005040    /* Clock Selection Register */
#define CM_CLKSEL_GPT2  0
#define CM_CLKSEL_GPT3  1
#define CM_CLKSEL_GPT4  2
#define CM_CLKSEL_GPT5  3
#define CM_CLKSEL_GPT6  4
#define CM_CLKSEL_GPT7  5
#define CM_CLKSEL_GPT8  6
#define CM_CLKSEL_GPT9  7


#endif /* TIMER_H_ */
