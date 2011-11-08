/* GPTimer.h
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

#ifndef GPTIMER_H_
#define GPTIMER_H_

#include <ottos/types.h>

enum TimerInterruptMode {
  MATCH_INTERRUPT = 0,
  OVERFLOW_INTERRUPT = 1,
  CAPTURE_INTERRUPT = 2
};


class GPTimer {
  public:
    GPTimer();
    GPTimer(address timer_address);
    virtual ~GPTimer();

    void init(int tick_time);
    void init(int ticks, int load_ticks);
    void start();
    void stop();
    void clear();
    void reset();

  private:
    address timer_address_;   /* base hardware address of this timer */
    address tier_;            /* this register is used to specify the interrupt mode and to enable/disable interrupts */
    address tisr_;            /* this register holds the pending interrupts for each interrupt mode */
    address tclr_;            /* this register controls optional features specific to the timer functionality (start/stop timer) */
    address tcrr_;            /* this register holds the value of the internal counter */
    address tldr_;            /* this register holds the timer load values (value with which the timer is initialized) */
    address tmar_;            /* this register holds the value to be compared with the counter value */
};

#endif /* GPTIMER_H_ */
