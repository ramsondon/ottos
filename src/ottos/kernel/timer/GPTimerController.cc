/* GPTimerController.cpp
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

#include <arch/arm/omap353x_timer.h>
#include "GPTimerController.h"
#include "GPTimer.h"


/************** PUBLIC **************/

GPTimerController::GPTimerController() {
  timer_[0] = GPTimer(GPTIMER1);
  timer_[1] = GPTimer(GPTIMER2);
  timer_[2] = GPTimer(GPTIMER3);
  timer_[3] = GPTimer(GPTIMER4);
  timer_[4] = GPTimer(GPTIMER5);
  timer_[5] = GPTimer(GPTIMER6);
  timer_[6] = GPTimer(GPTIMER7);
  timer_[7] = GPTimer(GPTIMER8);
  timer_[8] = GPTimer(GPTIMER9);
  timer_[9] = GPTimer(GPTIMER10);
  timer_[10] = GPTimer(GPTIMER11);
}

GPTimerController::~GPTimerController() {

}

void GPTimerController::init(int timer, int tick_time) {
  timer_[timer-1].init(tick_time);
}

void GPTimerController::start(int timer) {
  timer_[timer-1].start();
}

void GPTimerController::stop(int timer) {
  timer_[timer-1].stop();
}

void GPTimerController::clear(int timer) {
  timer_[timer-1].clear();
}

void GPTimerController::reset(int timer) {
  timer_[timer-1].reset();
}


/************** PRIVATE **************/

GPTimer GPTimerController::getTimer(int timer){
  return timer_[timer-1];
}
