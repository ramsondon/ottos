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


/* OMAP3530 General Purpose Timer Base Addresses */
#define GPTIMER1 0x48318000
#define GPTIMER2 0x49032000
#define GPTIMER3 0x49034000
#define GPTIMER4 0x49036000
#define GPTIMER5 0x49038000
#define GPTIMER6 0x4903A000
#define GPTIMER7 0x4903C000
#define GPTIMER8 0x4903E000
#define GPTIMER9 0x49040000
#define GPTIMER10 0x48086000
#define GPTIMER11 0x48088000

/* General Purpose Timer Registers */
#define GPT_TIDR 0x000        /* Base Address */
#define GPT_TIOCP_CFG 0x010   /*  */
#define GPT_TISTAT 0x014
#define GPT_TISR 0x018        /* Pending interrupts in the module */
#define GPT_TIER 0x01C        /* controls enable/disable interrupt events */
#define GPT_TWER 0x020
#define GPT_TCLR 0x024        /*  */
#define GPT_TCRR 0x028
#define GPT_TLDR 0x02C        /* Timer Load Register */
#define GPT_TTGR 0x030
#define GPT_TWPS 0x034
#define GPT_TMAR 0x038
#define GPT_TCAR1 0x03C
#define GPT_TSICR 0x040
#define GPT_TCAR2 0x044
#define GPT_TPIR 0x048
#define GPT_TNIR 0x04C
#define GPT_TCVR 0x050
#define GPT_TOCR 0x054
#define GPT_TOWR 0x058

/* TIER / TISR flags */
#define TCAR_IT_FLAG (1<<2)   /* capture interrupts */
#define OVF_IT_FLAG (1<<1)    /* overflow interrupts */
#define MAT_IT_FLAG (1<<0)    /* match interrupts */




#endif /* TIMER_H_ */
