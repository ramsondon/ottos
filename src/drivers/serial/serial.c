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

int serial_create(device_t dev) {

  uart_init((mem_address_t*) UART3, UART_MODE_16X, uart_protocol_rs232, 0x0);
  return TRUE;
}

int serial_open(device_t dev) {
  return FALSE;
}
int serial_close(device_t dev) {
  return FALSE;
}
int serial_read(device_t dev, int count, char* buffer) {
  buffer[0] = uart_read((mem_address_t*)UART3);
  return FALSE;
}
int serial_write(device_t dev, int count, char* buffer) {
  uart_write((mem_address_t*) UART3, buffer[0]);
  return FALSE;
}
int serial_ioctl(device_t dev, ioctl_t msg) {
  return FALSE;
}

