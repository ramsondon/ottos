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

#include <ottos/error.h>
#include <ottos/types.h>

#include "../../hal/platform.h"
#include "../../drivers/uart/uart.h"
#include "serial.h"

static BOOLEAN init_uart_rs232_ = FALSE;

int serial_create_(device_t dev) {
  if (dev == SERIAL_0 && init_uart_rs232_ == FALSE) {
    uart_init((mem_address_t*) UART3, UART_MODE_16X, uart_protocol_rs232,
              UART_FLOW_CONTROL_DISABLE_FLAG);
    init_uart_rs232_ = TRUE;
  }

  if(init_uart_rs232_ == TRUE) {
    return DRIVER_NO_ERROR;
  }
  return DRIVER_ERROR_CANNOT_OPEN;
}

int serial_open_(device_t dev) {
  return serial_create_(dev);
}
int serial_close_(device_t dev) {
  return DRIVER_NO_ERROR;
}

int serial_read_(device_t dev, int count, char* buffer) {

  mem_address_t* uart = (mem_address_t*) UART3;

  if (uart_is_empty_read_queue(uart)) {
    return 0;
  }
  buffer[0] = uart_read(uart);

  return 1;
}

int serial_write_(device_t dev, int count, char* buffer) {

  mem_address_t* uart = (mem_address_t*) UART3;
  int i = 0;

  for (; i < count; i++, buffer++) {
    // block while queue is full
    while (!uart_is_empty_write_queue(uart))
      ;
    uart_write(uart, *buffer);
    /*if (*buffer == '\n') {
      uart_write(uart, '\r');
    }*/
  }

  return FALSE;
}

int serial_ioctl_(device_t dev, ioctl_t msg) {
  return FALSE;
}

int serial_read(char* buf, int count) {
  serial_create_(SERIAL_0);
  return serial_read_(SERIAL_0, count, buf);
}

int serial_write(const char* buf, int count) {

  serial_create_(SERIAL_0);
  return serial_write_(SERIAL_0, count, (char*) buf);
}

