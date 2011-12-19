/* i2c.c
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

#include <ottos/types.h>
#include <ottos/kernel.h>

#include <ottos/memory.h>

#include "../../hal/omap353x/i2c.h"

#include "i2c.h"

#define TIMEOUT 10000

/* CORE_CM registers S 4.14.1.5 */
#define CM_CORE_BASE 0x48004A00

#define CM_FCLKEN1_CORE 0x00000000
#define CM_ICLKEN1_CORE 0x00000010
// These bits apply to all of the (CM|PM)*1_CORE registers as EN_, ST_, and AUTO_* bits
#define CM_CORE_EN_I2C1 (1<<15)

static void waitidle(uint32_t base) {
  int timeout;

  // wait for bus-busy off
  timeout = TIMEOUT * 10;
  while (MMIO_READ16(base + I2C_STAT) & I2C_STAT_BB) {
    if (!timeout--) {
      kernel_print("i2c_write: wait-idle timeout\r\n");
      return;
    }
  }

}

// i2c state machine
// used for read or write or parts thereof
// con indicates what to do
//
// See OMAP TRM S 18.5.1.3: Figure 18-29 `I2C Master Transmitter Mode, Polling Method'
static void doit(uint32_t base, uint16_t con, uint8_t sa, uint8_t *buffer,
                 int count) {
  int timeout;
  uint16_t st;
  int i = 0;
  BOOLEAN redoreg = TRUE;

  do {
    MMIO_WRITE16(base + I2C_SA, sa);
    MMIO_WRITE16(base + I2C_CNT, count);
    MMIO_WRITE16(base + I2C_CON, con);

    timeout = TIMEOUT * 10;

    while (i < count) {
      st = MMIO_READ16(base + I2C_STAT);

      if (st & I2C_STAT_NACK) {
        MMIO_WRITE16(base + I2C_STAT, I2C_STAT_NACK);
        break;
        //goto redoreg;
      } else if (st & I2C_STAT_AL) {
        MMIO_WRITE16(base + I2C_STAT, I2C_STAT_AL);
        break;
        //goto redoreg;
      } else if (st & I2C_STAT_ARDY) {
        MMIO_WRITE16(base + I2C_STAT, I2C_STAT_ARDY);
        continue;
      } else if (st & I2C_STAT_RDR) {
        // not sure if i need this ...
        MMIO_WRITE16(base + I2C_STAT, I2C_STAT_XDR);

        redoreg = FALSE;
      } else if (st & I2C_STAT_XRDY) {
        MMIO_WRITE16(base + I2C_DATA, buffer[i++]);
        MMIO_WRITE16(base + I2C_STAT, I2C_STAT_XRDY);

        redoreg = FALSE;
      } else if (st & I2C_STAT_RRDY) {
        buffer[i++] = MMIO_READ16(base + I2C_DATA);
        MMIO_WRITE16(base + I2C_STAT, I2C_STAT_RRDY);

        redoreg = FALSE;
      } else if (timeout-- == 0) {
        kernel_print("i2c_read: receive timeout\r\n");
        return;
      }
    }
  } while (redoreg == TRUE);

  // wait for transfer complete?
  timeout = TIMEOUT;
  while ((MMIO_READ16(base + I2C_STAT) & I2C_STAT_ARDY) == 0)
    if (!timeout--) {
      kernel_print("i2c_write: wait-complete timeout\r\n");
      return;
    }
  MMIO_WRITE16(base + I2C_STAT, I2C_STAT_ARDY);
}

void bus_i2c_read(uint32_t base, uint8_t sa, uint8_t addr, uint8_t *buffer,
                  int count) {
  waitidle(base);
  // send address with no stop
  doit(base, 0x8601, sa, &addr, 1);
  // send rest with stop
  doit(base, 0x8403, sa, buffer, count);
}

// address encoded in buffer
void bus_i2c_write(uint32_t base, uint8_t sa, uint8_t *buffer, int count) {
  waitidle(base);
  doit(base, 0x8603, sa, buffer, count);
}

void bus_i2c_write8(uint32_t base, uint8_t sa, uint8_t addr, uint8_t v) {
  uint8_t buffer[2];// = { addr, v };
  buffer[0] = addr;
  buffer[1] = v;

  bus_i2c_write(base, sa, buffer, 2);
}

void bus_i2c_init(void) {
  uint32_t address = CM_CORE_BASE + CM_FCLKEN1_CORE;

  // enable i2c1 (ROM has alredy done it, but may as well make sure)
  MMIO_AND_THEN_OR32(address, ~CM_CORE_EN_I2C1, CM_CORE_EN_I2C1);
  MMIO_AND_THEN_OR32(address, ~CM_ICLKEN1_CORE, CM_CORE_EN_I2C1);

  // Sets I2C speed to 100Khz F/S mode
  MMIO_WRITE16(I2C1_BASE + I2C_PSC, 0x17);
  MMIO_WRITE16(I2C1_BASE + I2C_SCLL, 0x0d);
  MMIO_WRITE16(I2C1_BASE + I2C_SCLH, 0x0f);
}

