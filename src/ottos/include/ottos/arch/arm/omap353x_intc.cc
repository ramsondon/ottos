/* omap353x_intc.c
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

#include "../../../../kernel/intc/interrupt.h"
#include <stdio.h>

#pragma INTERRUPT(SWI)
extern "C" void c_intSWI(/*unsigned r0, unsigned r1, unsigned r2, unsigned r3*/) {
  _disable_interrupts();
  printf("hello\n");
  _enable_interrupts();
//  asm(" ldrb r3, [lr, #-1]");
//  switch (r3) {
//    case 0:
//    case 1:
//    case 2:
//    case 3:
//    default:
//      /* WRITE_REGISTER_SVC */
//      //*(unsigned int *) r0 = r1;
//      printf("hello swi\n");
//      return;
//    case 1:
//      /* enable FIQ interrupt */
//      asm(" mrs r0, spsr");
//      asm(" bic r0, r0, #0x40");
//      asm(" msr spsr, r0");
//      return;
//    case 2:
//      /* disable FIQ interrupt */
//      asm(" mrs r0, spsr");
//      asm(" orr r0, r0, #0x40");
//      asm(" msr spsr, r0");
//      return;
//    case 3:
//      /* enable IRQ interrupt */
//      asm(" mrs r0, spsr");
//      asm(" bic r0, r0, #0x80");
//      asm(" msr spsr, r0");
//      return;
//    case 4:
//      /* disable IRQ interrupt */
//      asm(" mrs r0, spsr");
//      asm(" orr r0, r0,#0x80");
//      asm(" msr spsr, r0");
//      return;
//    case 5:
//      /* enable both interrupts */
//      asm(" mrs r0, spsr");
//      asm(" bic r0, r0,#0xC0");
//      asm(" msr spsr, r0");
//      return;
//    case 6:
//      /* disable both interrupts */
//      asm(" mrs r0, spsr");
//      asm(" orr r0, r0, #0xC0");
//      asm(" msr spsr, r0");
//      return;
//    }
}

