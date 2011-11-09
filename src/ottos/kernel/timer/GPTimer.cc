/* GPTimer.cpp
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
 *  Created on: 5 Nov 2011
 *      Author: Florian Gopp <go.goflo@gmail.com>
 */


#include "GPTimer.h"
#include <ottos/types.h>
#include <arch/arm/omap353x_timer.h>


/* bit operations by itmOS/BOSS */
void setBit(address addr, int offset) {
    *(addr) |= (1 << offset);
}

void unsetBit(address addr, int offset) {
    *(addr) &= ~(1 << offset);
}

void toggleBit(address addr, int offset) {
    *(addr) ^= (1 << offset);
}

bool readBit(address addr, int offset) {
    return (*(addr) & (1 << offset));
}




GPTimer::GPTimer() { }

GPTimer::GPTimer(address timer_address) {
  timer_address_ = timer_address;

  /* calculate addresses of registers */
  tier_ = (address)((int)timer_address_ + GPT_TIER);
  tisr_ = (address)((int)timer_address_ + GPT_TISR);
  tclr_ = (address)((int)timer_address_ + GPT_TCLR);
  tcrr_ = (address)((int)timer_address_ + GPT_TCRR);
  tldr_ = (address)((int)timer_address_ + GPT_TLDR);
  tmar_ = (address)((int)timer_address_ + GPT_TMAR);
}

GPTimer::~GPTimer() { }

void GPTimer::init(int ticks) {
  init(ticks, 0);
}

void GPTimer::init(int ticks, int load_ticks) {
  stop();

  *(tldr_) = load_ticks;
  *(tmar_) = ticks;
  setBit(tclr_, TCLR_COMPARE);
  setBit(tclr_, TCLR_AUTORELOAD);
}

void GPTimer::start(TimerInterruptMode interrupt_mode) {
  setBit(tier_, interrupt_mode);
  setBit(tclr_, TCLR_START);
}

void GPTimer::stop() {
  *(tier_) &= 0;
  unsetBit(tclr_, TCLR_START);
  reset();
}

void GPTimer::clear() {
  setBit(tisr_, TISR_CAPTURE);
  setBit(tisr_, TISR_OVERFLOW);
  setBit(tisr_, TISR_MATCH);
}

void GPTimer::reset() {
  *(tcrr_) = 0;
}

