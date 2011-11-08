/* GPTimerController.h
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

#ifndef GPTIMERCONTROLLER_H_
#define GPTIMERCONTROLLER_H_

#include <arch/arm/omap353x_timer.h>
#include "GPTimer.h"

//class GPTimer;
/*
enum Timer {
  GPTimer1  = 1,
  GPTimer2  = 2,
  GPTimer3  = 3,
  GPTimer4  = 4,
  GPTimer5  = 5,
  GPTimer6  = 6,
  GPTimer7  = 7,
  GPTimer8  = 8,
  GPTimer9  = 9,
  GPTimer10 = 10,
  GPTimer11 = 11
};
*/

class GPTimerController {
  public:
    GPTimerController();
    virtual ~GPTimerController();

    void init(int timer, int tick_time);
    void start(int timer);
    void stop(int timer);
    void clear(int timer);
    void reset(int timer);

  private:
    GPTimer timer_[GPTIMER_COUNT];
    GPTimer getTimer(int timer);
};

#endif /* GPTIMERCONTROLLER_H_ */
