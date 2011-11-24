/* bits.h
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
 *  Created on: 11.11.2011
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#ifndef BITS_H_
#define BITS_H_

#define BIT0  (1 << 1)
#define BIT1  (1 << 2)
#define BIT2  (1 << 3)
#define BIT3  (1 << 4)
#define BIT4  (1 << 5)
#define BIT5  (1 << 6)
#define BIT6  (1 << 7)
#define BIT7  (1 << 8)
#define BIT8  (1 << 9)
#define BIT9  (1 << 10)
#define BIT10 (1 << 11)
#define BIT11 (1 << 12)
#define BIT12 (1 << 13)
#define BIT13 (1 << 14)
#define BIT14 (1 << 15)
#define BIT15 (1 << 16)
#define BIT16 (1 << 17)
#define BIT17 (1 << 18)
#define BIT18 (1 << 19)
#define BIT19 (1 << 20)
#define BIT20 (1 << 21)
#define BIT21 (1 << 22)
#define BIT22 (1 << 23)
#define BIT23 (1 << 24)
#define BIT24 (1 << 25)
#define BIT25 (1 << 26)
#define BIT26 (1 << 27)
#define BIT27 (1 << 28)
#define BIT28 (1 << 29)
#define BIT29 (1 << 30)
#define BIT30 (1 << 31)
#define BIT31 (1 << 32)

#define SET_BIT(addr, offset) *addr |= (1 << offset);
#define CLEAR_BIT(addr, offset) *addr &= ~(1 << offset);
#define TOGGLE_BIT(addr, offset) *(addr) ^= (1 << offset);
#define READ_BIT(addr, offset) (*(addr) & (1 << offset));

#endif /* BITS_H_ */
