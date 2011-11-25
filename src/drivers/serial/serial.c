/* serial.c
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
 *  Created on: Nov 19, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <arch/arm/omap353x_uart.h>
#include <ottos/types.h>

#include "serial.h"
#include "../../hal/uart.h"

static int init_uart_rs232_ = FALSE;

int serial_create(device_t dev) {
  if (dev == SERIAL_0 && init_uart_rs232_ == FALSE) {
    uart_init((mem_address_t*) UART3, UART_MODE_16X, uart_protocol_rs232,
            UART_FLOW_CONTROL_DISABLE_FLAG);
    init_uart_rs232_ = TRUE;
    return TRUE;
  }
  return FALSE;
}

int serial_open(device_t dev) {
  return serial_create(dev);
}
int serial_close(device_t dev) {
  return FALSE;
}

int serial_read(device_t dev, int count, char* buffer) {

  mem_address_t* uart = (mem_address_t*)UART3;
  int i = 0;

  for (; i < count; i++, buffer++) {
    // block while waiting for data
    while (uart_is_empty_read_queue(uart))
      ;
    uart_read(uart, buffer);
  }
  return i;
}

int serial_write(device_t dev, int count, char* buffer) {

  mem_address_t* uart = (mem_address_t*) UART3;
  int i = 0;

  for (; i < count; i++, buffer++) {
   // block while queue is full
   while (!uart_is_empty_write_queue(uart))
     ;
   uart_write(uart, buffer);
  }

  return FALSE;
}

int serial_ioctl(device_t dev, ioctl_t msg) {
  return FALSE;
}

