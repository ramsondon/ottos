/* irq.h
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
 *  Created on: 11.11.2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_

#include <ottos/const.h>

// flag to check if the interrupt is called the first time
extern int irq_started;

EXTERN void irq_register_context_switch();

EXTERN void irq_init();

EXTERN void irq_enable();

EXTERN void irq_disable();

#pragma INTERRUPT(irq_handle, IRQ)
EXTERN void irq_handle();

#pragma INTERRUPT(irq_handle_swi, SWI)
EXTERN void irq_handle_swi(unsigned int syscall_nr, unsigned int param1, unsigned int param2, unsigned int param3);

#pragma INTERRUPT(irq_handle_udef, UDEF)
EXTERN void irq_handle_udef();

#pragma INTERRUPT(irq_handle_dabt, DABT)
EXTERN void irq_handle_dabt();

#pragma INTERRUPT(irq_handle_pabt, PABT)
EXTERN void irq_handle_pabt();

EXTERN void irq_add_handler(int irq_id, void(*fn)(void));

#endif /* INTERRUPTHANDLER_H_ */
