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

#define UART_FLOW_CONTROL_DISABLE_FLAG  0x0000


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
 * UART flow control configuration structure
 * set specific flags to 1 to enable, 0 to disable and call
 * uart_set_flow_control(UART , uart_flow_control_t);
 */
//typedef struct uart_flow_control_t {
//    unsigned short auto_rts; /* hardware flow control RTS */
//    unsigned short auto_cts; /* hardware flow control CTS */
//    unsigned short tx1;      /* software flow control Transmit xon1/xoff1 */
//    unsigned short tx2;      /* software flow control Transmit xon2/xoff2 */
//    unsigned short rx1;      /* software flow control Receive xon1/xoff1 */
//    unsigned short rx2;      /* software flow control Receive xon2/xoff2 */
//} uart_flow_control_t;


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
static struct uart_protocol_format_t uart_protocol_rs232 =
    { UART_PROTOCOL_BAUDRATE_16X115_2, UART_PROTOCOL_DATA_LENGTH_8,
      UART_PROTOCOL_NB_STOP_1, UART_PROTOCOL_PARITY_NONE };

void uart_init(mem_address_t* uart_base_addr, int uart_mode,
               struct uart_protocol_format_t protocol,
               uint_8_t flowcontrol);

void uart_write(mem_address_t* uart_base_addr, char* buffer);

void uart_software_reset(mem_address_t* uart_base_addr);

#endif /* UART_H_ */
