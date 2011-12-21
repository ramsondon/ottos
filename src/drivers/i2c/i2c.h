/* i2c.h
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
 *  Created on: 16 Dec 2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#ifndef I2C_H_
#define I2C_H_

#include <ottos/types.h>

/* i2c bus */
void i2c_init(void);
void i2c_read(uint32_t base, uint8_t sa, uint8_t addr, uint8_t *buffer, int count);
// address stored as first byte(s) of buffer
void i2c_write(uint32_t base, uint8_t sa, uint8_t *buffer, int count);
// write 8 bites
void i2c_write8(uint32_t base, uint8_t sa, uint8_t addr, uint8_t v);


#endif /* I2C_H_ */
