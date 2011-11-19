/* uart.h
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

#ifndef UART_H_
#define UART_H_

#define UART_PROTOCOL_SERIAL   0x0003
#define UART_BAUDRATE_16X115_2 0x001A


/*
  BAUD RATE: 115200
  DATA: 8 bit
  PARITY: none
  STOP: 1bit
  FLOW CONTROL: none (Critical)
*/
void uart_init(mem_address_t* uart_base_addr, int baudrate, int uart_mode, int protocol);

void uart_software_reset(mem_address_t* uart_base_addr);

#endif /* UART_H_ */
