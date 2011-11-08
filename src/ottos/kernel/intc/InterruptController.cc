/* InterruptController.cc
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

#include "InterruptController.h"
#include <arch/arm/omap353x_timer.h>
//#include <arch/arm/omap353x_intc.h>
#include <ottos/types.h>
#include <stdio.h>

InterruptController* globalIntHandler;

/* handles the Interrupt Routines IRQ */

#pragma INTERRUPT (IRQ)
extern "C" void handle_irq() {
  /* get the number of the interrupt */
  int irqId =  *(INTCPS_SIR_IRQ);

  /* forward the interrupt to the handler routine */
  globalIntHandler->handle_Irq(irqId);

  *(INTCPS_CONTROL) |= 0x1;
}

InterruptController::InterruptController() {
  globalIntHandler = this;
}

InterruptController::~InterruptController() {

}

void InterruptController::add_handler(int handler_id, void (*fn)(void)) {
  int_handler_[handler_id] = fn;

  int registerNr = handler_id / 32;
  int irqBit = handler_id % 32;

  /* activate the specific interrupt (interrupt mask) */
  switch (registerNr)  {
  case 0:
    *INTCPS_MIR_SET_0 |= (1 << irqBit);
    break;
  case 1:
    *INTCPS_MIR_SET_1 |= (1 << irqBit);
    break;
  case 2:
    *INTCPS_MIR_SET_2 |= (1 << irqBit);
    break;
  }
}


void InterruptController::handle_Irq(int irq_id) {
  /* call handler function */
  //printf("handle irq with id: %i ... \n", irq_id);
  (*int_handler_[irq_id])();
}
