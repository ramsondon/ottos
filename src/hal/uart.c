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

/* UART software reset */
#define UART_SOFTRESET(uart_mem_addr_ptr) (*(uart_mem_addr_ptr + \
  UART_SYSC_REG/sizeof(mem_address_t)) |= (1<<1))

/* UART reset done */
#define UART_RESETDONE(uart_mem_addr_ptr) (int)(*(uart_mem_addr_ptr + \
    UART_SYSS_REG/sizeof(mem_address_t)) & (1<<0))

#define READ_REGISTER(uart_mem_addr_ptr, reg) \
    *(uart_mem_addr_ptr + reg/sizeof(mem_address_t))

#define UART_BAUDRATE_MASK_LSB 0x00FF

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
   */UART_SOFTRESET(uart_base_addr);

  /*
   * If the SOFTRESET bit is 1 then we have to busy wait until the RESETDONE bit
   * in SYSS_REG equals 1.
   */
  while (busy_wait != 1) {
    busy_wait = UART_RESETDONE(uart_base_addr);
  }
}

/*
 * Disables the UART instance
 */
void uart_disable(mem_address_t* uart_base_addr) {
  *(uart_base_addr + UART_MDR1_REG / sizeof(mem_address_t))
      = UART_MODE_SELECT_DISABLE;
}

void uart_switch_to_config_mode_b(mem_address_t* uart_base_addr) {
  *(uart_base_addr + UART_LCR_REG / sizeof(mem_address_t))
      = UART_LCR_CONFIG_MODE_B;
}

void uart_switch_to_register_operational_mode(mem_address_t* uart_base_addr) {
  *(uart_base_addr + UART_LCR_REG / sizeof(mem_address_t))
      = UART_LCR_REG_OP_MODE;
}

/* Table for Baudrate settings in OMAP353x Ref manual (p.2716) */
void uart_set_baudrate(mem_address_t* uart_base_addr, int baudrate) {

  /* set least significant bits */
  *(uart_base_addr + UART_DLL_REG / sizeof(mem_address_t)) = (baudrate
      & UART_BAUDRATE_MASK_LSB);

  /* set most significatn bits*/
  *(uart_base_addr + UART_DLH_REG / sizeof(mem_address_t)) = (baudrate >> 8);
}

void uart_set_mode(mem_address_t* uart_base_addr, int uart_mode) {
  *(uart_base_addr + UART_MDR1_REG / sizeof(mem_address_t)) = uart_mode;
}

void uart_set_protocol_format(mem_address_t* uart_base_addr, int protocol) {

  *(uart_base_addr + UART_LCR_REG/sizeof(mem_address_t)) |= protocol;
}

void uart_init(mem_address_t* uart_base_addr, int baudrate, int uart_mode,
               int protocol) {

  int efr_enh = 0;

  /* reset UART */
  uart_software_reset(uart_base_addr);

  /* disable UART to enable access DLL and DLH registers */
  uart_disable(uart_base_addr);

  /* enable access to ERF register */
  uart_switch_to_config_mode_b(uart_base_addr);

  /* enable access to IER register [7:4] */
  efr_enh = READ_BIT((uart_base_addr + UART_EFR_REG/sizeof(mem_address_t)),
      UART_EFR_ENHANCED_EN);
  SET_BIT((uart_base_addr + UART_EFR_REG/sizeof(mem_address_t)),
      UART_EFR_ENHANCED_EN);

  /* switch to operational mode to enable access to IER register */
  // TODO(ramsondon@gmail.com): check registers values
  uart_switch_to_register_operational_mode(uart_base_addr);

  /* clear IER and goto SLEEP MODE */
  *(uart_base_addr + UART_IER_REG / sizeof(mem_address_t)) = 0x0000;

  /* enable access to DLL and DLH register */
  uart_switch_to_config_mode_b(uart_base_addr);

  /* to change the baudrate and clocksettings */
  uart_set_baudrate(uart_base_addr, baudrate);

  uart_switch_to_register_operational_mode(uart_base_addr);

  /* TODO(ramsondon@gmail.com): STEP 9, load interrupt configuration in IER register */



  /* switch back to config mode b */
  uart_switch_to_config_mode_b(uart_base_addr);

  /* restore enhanced mode setting */
  if (efr_enh > 0) {
    SET_BIT((uart_base_addr + UART_EFR_REG/sizeof(mem_address_t)), UART_EFR_ENHANCED_EN);
  } else {
    CLEAR_BIT((uart_base_addr + UART_EFR_REG/sizeof(mem_address_t)), UART_EFR_ENHANCED_EN);
  }

  /* clear LCR div and break field */
  CLEAR_BIT((uart_base_addr + UART_LCR_REG/sizeof(mem_address_t)), UART_LCR_DIV_EN);
  CLEAR_BIT((uart_base_addr + UART_LCR_REG/sizeof(mem_address_t)), UART_LCR_BREAK_EN);

  // load protocol formatting
  uart_set_protocol_format(uart_base_addr, protocol);

  /* switch back to operational mode */
  uart_switch_to_register_operational_mode(uart_base_addr);

  /* load the new UART mode */
  uart_set_mode(uart_base_addr, uart_mode);
}
