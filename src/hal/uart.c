/* uart.c
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

/*
BAUD RATE: 115200
DATA: 8 bit
PARITY: none
STOP: 1bit
FLOW CONTROL: none (Critical)
*/

#include <arch/arm/omap353x_uart.h>
#include <ottos/types.h>
#include <bits.h>

#include "uart.h"


/*
 * The Mode Registers must be configured after the configuration of the
 * DLL_REG, DLH_REG, LCR_REG registers
 */


/*
 * Initiates a software reset of UART uart_base_addr
 */
void uart_software_reset(mem_address_t* uart_base_addr) {

  int busy_wait = 0;
  /*
   * To clear the UART registers we have to set the SOFTRESET bit of SYSC_REG
   * to 1.
   */
  UART_SOFTRESET(uart_base_addr);

  /*
   * If the SOFTRESET bit is 1 then we have to busy wait until the RESETDONE bit
   * in SYSS_REG equals 1.
   */
  ;
  while (busy_wait != 1) {
    busy_wait = UART_RESETDONE(uart_base_addr);
  }
}

void uart_disable(mem_address_t* uart_base_addr) {
  *(uart_base_addr + UART_MDR1_REG) = UART_MODE_SELECT_DISABLE;
}

void uart_init(mem_address_t* uart_base_addr, int baudrate) {
  /* reset uart */
  uart_software_reset((mem_address_t*)uart_base_addr);

}
