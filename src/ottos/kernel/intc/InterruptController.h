/* InterruptController.h
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

#ifndef INTERRUPTCONTROLLER_H_
#define INTERRUPTCONTROLLER_H_


#define INTCPS_CONTROL (address) 0x48200048   // MPU_INTC + INTCPS_CONTROL
#define INTCPS_SIR_IRQ (address) 0x48200040   // MPU_INTC + INTCPS_SIR_IRQ

#define INTCPS_MIR_SET_0 (address) 0x48200088 // MPU_INTC +
#define INTCPS_MIR_SET_1 (address) 0x482000A8 // MPU_INTC +
#define INTCPS_MIR_SET_2 (address) 0x482000C8 // MPU_INTC +


/* The Interrupt Routine for IRQs */
//extern "C" void handle_irq();

class InterruptController {
  public:
    InterruptController();
    virtual ~InterruptController();
    void add_handler(int handler_id, void (*fn)(void));
    void handle_Irq(int irq_id);

  private:
    void (*int_handler_[100])(void);
};

#endif /* INTERRUPTCONTROLLER_H_ */
