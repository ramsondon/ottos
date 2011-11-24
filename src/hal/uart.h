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

#include <ottos/types.h>

#define UART_PROTOCOL_BAUDRATE_16X115_2 0x001A

#define UART_PROTOCOL_DATA_LENGTH_5     0x0
#define UART_PROTOCOL_DATA_LENGTH_6     0x1
#define UART_PROTOCOL_DATA_LENGTH_7     0x2
#define UART_PROTOCOL_DATA_LENGTH_8     0x3

#define UART_PROTOCOL_NB_STOP_1         0x0
#define UART_PROTOCOL_NB_STOP_2         0x1

/* TODO(ramsondon@gmail.com) add more parity types.
 * for PARITY_TYPE 1 and 2 configurations
 */
#define UART_PROTOCOL_PARITY_NONE       0x0

#define UART_FLOW_CONTROL_DISABLE_FLAG  0x0


/* UART Modes */
#define UART_MODE_13X          UART_MDR1_MODE_SELECT_13X
#define UART_MODE_16X          UART_MDR1_MODE_SELECT_16X
#define UART_MODE_16X_AUTOBAUD UART_MDR1_MODE_SELECT_16X_AUTOBAUD
#define UART_MODE_SIR          UART_MDR1_MODE_SELECT_SIR
#define UART_MODE_MIR          UART_MDR1_MODE_SELECT_MIR
#define UART_MODE_FIR          UART_MDR1_MODE_SELECT_FIR
#define UART_MODE_CIR          UART_MDR1_MODE_SELECT_CIR
#define UART_MODE_DISABLE      UART_MDR1_MODE_SELECT_DISABLE

/*
 * UART protocol format
 */
typedef struct uart_protocol_format_t {
    unsigned int baudrate;
    unsigned short datalen;
    unsigned short stopbit;
    unsigned int parity;
} uart_protocol_format_t;

/*
 * UART RS232 configuration
 * BAUD RATE: 115200
 * DATA: 8 bit
 * PARITY: none
 * STOP: 1bit
 * FLOW CONTROL: none (Critical)
 */
static struct uart_protocol_format_t uart_protocol_rs232 = {
  UART_PROTOCOL_BAUDRATE_16X115_2,
  UART_PROTOCOL_DATA_LENGTH_8,
  UART_PROTOCOL_NB_STOP_1,
  UART_PROTOCOL_PARITY_NONE
};

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

#endif /* UART_H_ */
