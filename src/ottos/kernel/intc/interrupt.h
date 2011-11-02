/* interrupt.h
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

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdio.h>

/* software interrupt function identifiers */
//#define INTC_DISABLE_IRQ 0
//#define INTC_ENABLE_IRQ 1
//#define INTC_DISABLE_FIQ 2
//#define INTC_ENABLE_FIQ 3
//
//
///* software interrupt function declarations */
//#pragma SWI_ALIAS(INTC_DISABLE_IRQ)
//extern void disable_irq();
//
//#pragma SWI_ALIAS(INTC_ENABLE_IRQ)
//extern void enable_irq();
//
//#pragma SWI_ALIAS(INTC_DISABLE_FIQ)
//extern void disable_fiq();
//
//#pragma SWI_ALIAS(INTC_ENABLE_FIQ)
//extern void enable_fiq();

//#pragma SWI_ALIAS(48)
//extern void swi();

//extern "C" void c_intSWI(/*unsigned r0, unsigned r1, unsigned r2, unsigned r3*/);
extern "C" void c_intIRQ();

#endif /* INTERRUPT_H_ */
