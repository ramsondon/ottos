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

/* OMAP3530 General Purpose Timer Base Addresses */
#define GPTIMER1 (address)0x48318000
#define GPTIMER2 (address)0x49032000
#define GPTIMER3 (address)0x49034000
#define GPTIMER4 (address)0x49036000
#define GPTIMER5 (address)0x49038000
#define GPTIMER6 (address)0x4903A000
#define GPTIMER7 (address)0x4903C000
#define GPTIMER8 (address)0x4903E000
#define GPTIMER9 (address)0x49040000
#define GPTIMER10 (address)0x48086000
#define GPTIMER11 (address)0x48088000

/* General Purpose Timer Registers */
#define GPT_TIDR 0x00        /* Base Address */
#define GPT_TIOCP_CFG 0x10   /*  */
#define GPT_TISTAT 0x14
#define GPT_TISR 0x18        /* Pending interrupts in the module */
#define GPT_TIER 0x1C        /* controls enable/disable interrupt events */
#define GPT_TWER 0x20
#define GPT_TCLR 0x24        /*  */
#define GPT_TCRR 0x28
#define GPT_TLDR 0x2C        /* Timer Load Register */
#define GPT_TTGR 0x30
#define GPT_TWPS 0x34
#define GPT_TMAR 0x38
#define GPT_TCAR1 0x3C
#define GPT_TSICR 0x40
#define GPT_TCAR2 0x44
#define GPT_TPIR 0x48
#define GPT_TNIR 0x4C
#define GPT_TCVR 0x50
#define GPT_TOCR 0x54
#define GPT_TOWR 0x58

/* TIER / TISR flags */
#define TISR_CAPTURE 2      /* capture interrupts */
#define TISR_OVERFLOW 1     /* overflow interrupts */
#define TISR_MATCH 0        /* match interrupts */

/* TCLR flags */
#define TCLR_COMPARE 6
#define TCLR_AUTORELOAD 1
#define TCLR_START 0

#endif /* TIMER_H_ */
