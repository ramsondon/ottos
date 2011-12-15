/*
 * i2c.c
 *
 *  Created on: 14.12.2011
 *      Author: Fox
    bus_i2c.c: i2c bus stuff

    Copyright (C) 2010 Michael Zucchi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "i2c.h"

#define TIMEOUT 10000

/*
static void waitidle(uint32_t base) {
        int timeout;

        // wait for bus-busy off
        timeout = TIMEOUT*10;
        while (reg16r(base, I2C_STAT) & I2C_STAT_BB)
                if (!timeout--) { dprintf("i2c_write: wait-idle timeout\n"); return; }
}

// i2c state machine
// used for read or write or parts thereof
// con indicates what to do
//
// See OMAP TRM S 18.5.1.3: Figure 18-29 `I2C Master Transmitter Mode, Polling Method'
static void doit(uint32_t base, uint16_t con, uint8_t sa, uint8_t *buffer, int count) {
        int timeout;
        uint16_t st;

        //dprintf("doit: con = %016b\n", con);

 redoreg:
        reg16w(base, I2C_SA, sa);
        reg16w(base, I2C_CNT, count);
        reg16w(base, I2C_CON, con);

        timeout = TIMEOUT*10;
        int i = 0;
        while (i < count) {
                st = reg16r(base, I2C_STAT);
                //dprintf("doit: stat = %04x\n", st);

                if (st & I2C_STAT_NACK) {
                        reg16w(base, I2C_STAT, I2C_STAT_NACK);
                        goto redoreg;
                } else if (st & I2C_STAT_AL) {
                        reg16w(base, I2C_STAT, I2C_STAT_AL);
                        goto redoreg;
                } else if (st & I2C_STAT_ARDY) {
                        reg16w(base, I2C_STAT, I2C_STAT_ARDY);
                        continue;
                } else if (st & I2C_STAT_RDR) {
                        // not sure if i need this ...
                        reg16w(base, I2C_STAT, I2C_STAT_XDR);
                } else if (st & I2C_STAT_XRDY) {
                        //dprintf("doit: sending byte\n");
                        reg16w(base, I2C_DATA, buffer[i++]);
                        reg16w(base, I2C_STAT, I2C_STAT_XRDY);
                } else if (st & I2C_STAT_RRDY) {
                        //dprintf("doit: received byte\n");
                        buffer[i++] = reg16r(base, I2C_DATA);
                        reg16w(base, I2C_STAT, I2C_STAT_RRDY);
                } else if (timeout-- == 0) {
                        dprintf("i2c_read: receive timeout\n");
                        return;
                }
        }

        // wait for transfer complete?
        timeout = TIMEOUT;
        while ((reg16r(base, I2C_STAT) & I2C_STAT_ARDY) == 0)
                if (!timeout--) { dprintf("i2c_write: wait-complete timeout\n"); return; }
        reg16w(base, I2C_STAT, I2C_STAT_ARDY);
}

void bus_i2c_read(uint32_t base, uint8_t sa, uint8_t addr, uint8_t *buffer, int count) {
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
        uint8_t buffer[2] = { addr, v };

        bus_i2c_write(base, sa, buffer, 2);
}

void bus_i2c_init(void) {
        uint32_t base = I2C1_BASE;

        // enable i2c1 (ROM has alredy done it, but may as well make sure)
        reg32s(CM_CORE_BASE, CM_FCLKEN1_CORE, CM_CORE_EN_I2C1, CM_CORE_EN_I2C1);
        reg32s(CM_CORE_BASE, CM_ICLKEN1_CORE, CM_CORE_EN_I2C1, CM_CORE_EN_I2C1);

        // Sets I2C speed to 100Khz F/S mode
        reg16w(base, I2C_PSC, 0x17);
        reg16w(base, I2C_SCLL, 0x0d);
        reg16w(base, I2C_SCLH, 0x0f);
}
*/
