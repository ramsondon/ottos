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
 *  Created on: Dec 2, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef DRIVERS_UART_UART_H_
#define DRIVERS_UART_UART_H_

#include "../../hal/uart.h"

/*
 * Inits UART module uart_base_addr. Use defines (mem_address_t*) UART1, UART2,
 * or UART3.
 */
void uart_init(mem_address_t* uart_base_addr, int uart_mode,
               struct uart_protocol_format_t protocol,
               uint8_t flowcontrol);

/*
 * Disables UART and initiates a software reset
 */
void uart_reset(mem_address_t* uart_base_addr);

/*
 * Necessary for FIFO configuration. Sets Config Mode A (0x0080) into LCR
 * register.
 */
void uart_switch_to_config_mode_a(mem_address_t* uart_base_addr);

/* Sets Config Mode B (0x00BF) into LCR register to setup protocol format */
void uart_switch_to_config_mode_b(mem_address_t* uart_base_addr);

/* Sets operational Mode (0x0000) to LCR register */
void uart_switch_to_register_operational_mode(mem_address_t* uart_base_addr);

/*
 * Sets the baudrate. DHL and DLL register MUST be 0x0000 before calling this
 * function.
 */
void uart_set_baudrate(mem_address_t* uart_base_addr, int baudrate);

/*
 * The UART mode to run the uart_base_addr UART unit.
 * Use UART_MODE_xxx as params.
 */
void uart_set_mode(mem_address_t* uart_base_addr, int uart_mode);

/*
 * @see: uart_protocol_rs232
 */
void uart_set_protocol_format(mem_address_t* uart_base_addr,
                              uart_protocol_format_t protocol);

/*
 * Use in Config Mode B
 */
void uart_set_flow_control(mem_address_t* uart_base_addr, uint8_t flow_control);

/*
 * Enables enhanced functionality register access.
 */
void uart_enable_enhanced_func(mem_address_t* uart_base_addr);

/*
 * Enables the access to the MCR (Modem Controller) TCR_TLR register.
 * uart_enable_enhanced_func() MUST be called first.
 */
void uart_enable_tcr(mem_address_t* uart_base_addr);


/*
 * Writes one character to the UART device. This function does not check if the
 * transmission queue is empty. check with uart_is_empty_write_queue() for
 * all bytes to write.
 */
void uart_write(mem_address_t* uart_base_addr, char* buffer);

/*
 * Reads one character from the UART device into the buffer. This function does
 * not check if the receive queue is empty. check with
 * uart_is_empty_read_queue() for all bytes to read.
 */
void uart_read(mem_address_t* uart_base_addr, char* buffer);

/*
 * Returns 1 if the UART transmission queue is empty, else 0
 */
int uart_is_empty_write_queue(mem_address_t* uart_base_addr);

/*
 * Returns 1 if the UART receiving queue is empty, else 0
 */
int uart_is_empty_read_queue(mem_address_t* uart_base_addr);



#endif /* DRIVERS_UART_UART_H_ */
