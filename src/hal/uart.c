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
#include <arch/arm/omap353x_intc.h>
#include <ottos/types.h>
#include <bits.h>

#include <stdio.h>

#include "uart.h"
#include "../kernel/intc/irq.h"

/* Baudrate Bitmask for the least significant bits */
#define UART_BAUDRATE_MASK_LSB 0x00FF

/* Returns the mem_address_t* of a specific UART register */
#define UART_MEM_ADD(uart_ptr, reg) (uart_ptr + reg/sizeof(mem_address_t*))

/* UART software reset */
#define UART_SOFTRESET(uart_mem_addr_ptr) (*UART_MEM_ADD(uart_mem_addr_ptr, \
  UART_SYSC_REG) |= (1<<1))

/* UART reset done */
#define UART_RESETDONE(uart_mem_addr_ptr) (int)(*UART_MEM_ADD(uart_mem_addr_ptr, \
  UART_SYSS_REG) & (1<<0))



/*
 * Private Functions
 */
static void uart_software_reset(mem_address_t* uart_base_addr);
static void uart_disable(mem_address_t* uart_base_addr);
static void uart_switch_lcr_mode(mem_address_t* uart_base_addr,
                                 unsigned int lcr_mode);
static void uart_set_protocol_data_len(mem_address_t* uart_base_addr,
                                       unsigned short len);
static void uart_set_protocol_stop_bit(mem_address_t* uart_base_addr,
                                       unsigned short nb_stop);
static void uart_set_protocol_parity(mem_address_t* uart_base_addr,
                                     unsigned int parity);


/*
 * Software Reset and disabling UART unit, to access DLL, DHL register.
 */
void uart_reset(mem_address_t* uart_base_addr) {

  /* reset UART */
  uart_software_reset(uart_base_addr);

  /* disable UART to enable access DLL, DLH registers (for setting baudrate) */
  uart_disable(uart_base_addr);
}

/*
 * Initiates a software reset of UART uart_base_addr
 */
static void uart_software_reset(mem_address_t* uart_base_addr) {

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
static void uart_disable(mem_address_t* uart_base_addr) {
  *UART_MEM_ADD(uart_base_addr, UART_MDR1_REG) = UART_MDR1_MODE_SELECT_DISABLE;
}

static void uart_switch_lcr_mode(mem_address_t* uart_base_addr,
                                 unsigned int lcr_mode) {
  *UART_MEM_ADD(uart_base_addr, UART_LCR_REG) = lcr_mode;
}

void uart_switch_to_config_mode_a(mem_address_t* uart_base_addr) {
  uart_switch_lcr_mode(uart_base_addr, UART_LCR_MODE_CONFIG_A);
}

void uart_switch_to_config_mode_b(mem_address_t* uart_base_addr) {
  uart_switch_lcr_mode(uart_base_addr, UART_LCR_MODE_CONFIG_B);
}

void uart_switch_to_register_operational_mode(mem_address_t* uart_base_addr) {
  uart_switch_lcr_mode(uart_base_addr, UART_LCR_MODE_REG_OP);
}

/* Table for Baudrate settings in OMAP353x Ref manual (p.2716) */
void uart_set_baudrate(mem_address_t* uart_base_addr, int baudrate) {

  /* set least significant bits */
  *UART_MEM_ADD(uart_base_addr, UART_DLL_REG) = (baudrate & UART_BAUDRATE_MASK_LSB);

  /* set most significatn bits*/
  *UART_MEM_ADD(uart_base_addr, UART_DLH_REG) = (baudrate >> 8);
}

void uart_set_mode(mem_address_t* uart_base_addr, int uart_mode) {
  *UART_MEM_ADD(uart_base_addr, UART_MDR1_REG) = uart_mode;
}

/*
 * Use defines specified below as valid parameter values.
 * @param: len
 *    UART_PROTOCOL_DATA_LEN_5,
 *    UART_PROTOCOL_DATA_LEN_6,
 *    UART_PROTOCOL_DATA_LEN_7,
 *    UART_PROTOCOL_DATA_LEN_8
 */
static void uart_set_protocol_data_len(mem_address_t* uart_base_addr,
                                       unsigned short len) {
  /* clear the len bits in LCR register */
  *UART_MEM_ADD(uart_base_addr, UART_LCR_REG) &= ~(len);
  /* set new len bits in LCR register */
  *UART_MEM_ADD(uart_base_addr, UART_LCR_REG) |= len;
}

static void uart_set_protocol_stop_bit(mem_address_t* uart_base_addr,
                                       unsigned short nb_stop) {
  switch (nb_stop) {
    case UART_PROTOCOL_NB_STOP_1:
      CLEAR_BIT(UART_MEM_ADD(uart_base_addr, UART_LCR_REG), UART_LCR_NB_STOP);
      break;
    case UART_PROTOCOL_NB_STOP_2:
    default:
      SET_BIT(UART_MEM_ADD(uart_base_addr, UART_LCR_REG), UART_LCR_NB_STOP);
      break;
  }
}

static void uart_set_protocol_parity(mem_address_t* uart_base_addr,
                                     unsigned int parity) {
  // XXX(ramsondon@gmail.com): Implement more parity types
  // check partiy_type_1 and parity_type_2 arguments

  switch (parity) {
    case UART_PROTOCOL_PARITY_NONE:
    default:
      CLEAR_BIT(UART_MEM_ADD(uart_base_addr, UART_LCR_REG), UART_LCR_PARITY_EN);
      break;
  }
}

void uart_set_protocol_format(mem_address_t* uart_base_addr,
                              uart_protocol_format_t protocol) {

  /* clear LCR DIV and BREAK field */
  CLEAR_BIT(UART_MEM_ADD(uart_base_addr, UART_LCR_REG), UART_LCR_DIV_EN);
  CLEAR_BIT(UART_MEM_ADD(uart_base_addr, UART_LCR_REG), UART_LCR_BREAK_EN);

  /* set baudrate */
  //  uart_set_baudrate(uart_base_addr, protocol.baudrate);
  /* set the character length for this protocol */
  uart_set_protocol_data_len(uart_base_addr, protocol.datalen);
  /* configure stop bits */
  uart_set_protocol_stop_bit(uart_base_addr, protocol.stopbit);
  /* configure parity options */
  uart_set_protocol_parity(uart_base_addr, protocol.parity);
}

void uart_set_flow_control(mem_address_t* uart_base_addr, uint8_t flow_control) {
  *UART_MEM_ADD(uart_base_addr, UART_EFR_REG) = flow_control;
}

void uart_enable_enhanced_func(mem_address_t* uart_base_addr) {
  SET_BIT(UART_MEM_ADD(uart_base_addr, UART_EFR_REG), UART_EFR_ENHANCED_EN);
}

void uart_enable_tcr(mem_address_t* uart_base_addr) {
  SET_BIT(UART_MEM_ADD(uart_base_addr, UART_MCR_REG), UART_MCR_TCR_TLR);
}

/*
 * clear interrupts and set sleep mode
 */
void uart_clear_interrupts(mem_address_t* uart_base_addr) {

  /* clear IER and goto SLEEP MODE */
  *UART_MEM_ADD(uart_base_addr, UART_IER_REG) = 0x0;
}

void uart_enable_loopback(mem_address_t* uart_base_addr) {
  *UART_MEM_ADD(uart_base_addr, UART_MCR_REG) |= UART_MCR_LOOPBACK_EN;
}

/*
 * The Mode Registers must be configured after the configuration of the
 * DLL_REG, DLH_REG, LCR_REG registers
 */
void uart_init(mem_address_t* uart_base_addr, int uart_mode,
               struct uart_protocol_format_t protocol, uint8_t flowcontrol) {

  uart_reset(uart_base_addr);

  /* switch to config mode b to write to EFR and LCR */
  uart_switch_to_config_mode_b(uart_base_addr);

  /* change the baudrate and clocksettings */
  uart_set_baudrate(uart_base_addr, protocol.baudrate);

  /* set flow control flags */
  uart_set_flow_control(uart_base_addr, flowcontrol);

  /* set protocol format */
  uart_set_protocol_format(uart_base_addr, protocol);

  /* load the new UART mode */
  uart_set_mode(uart_base_addr, uart_mode);
}

/*
 * Returns 1 if uart read fifo queue is empty.
 * If the Queue has at least one character the result will be 1.
 */
int uart_is_empty_read_queue(mem_address_t* uart_base_addr) {
  int status = (int) READ_BIT(UART_MEM_ADD(uart_base_addr, UART_LSR_REG),
                              UART_LSR_RX_FIFO_E);
  return (status == 0);
}

/*
 * Returns 1 if uart transmission fifo queue is empty.
 * If the Queue has at least one character the result will be 1.
 */
int uart_is_empty_write_queue(mem_address_t* uart_base_addr) {
 int status = READ_BIT(UART_MEM_ADD(uart_base_addr, UART_LSR_REG),
                       UART_LSR_TX_FIFO_E);
  return (status > 0);
}

/* writes one character to the UART device */
void uart_write(mem_address_t* uart_base_addr, char* buffer) {
  *UART_MEM_ADD(uart_base_addr, UART_THR_REG) = *buffer;
}

/* reads one character from the UART device */
void uart_read(mem_address_t* uart_base_addr, char* buffer) {
  *buffer = *UART_MEM_ADD(uart_base_addr, UART_RHR_REG);
}
