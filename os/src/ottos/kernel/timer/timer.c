/* timer.h
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
 *  Created on: 10.11.2011
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#include <ottos/timer.h>
#include <ottos/io.h>
#include <ottos/types.h>
#include <ottos/dev/device.h>
#include <ottos/platform.h>

/*********************************
 * TODO:(ramsondon@gmail.com) HACK FOR DRIVER REFACTORING
 *********************************/
#include "../../drivers/gpt/gpt.h"
/*********************************
 * END HACK
 *********************************/


#include "../intc/irq.h"

static uint64_t jiffies = 0;

static system_timer_t timers_[MAX_TIMER_COUNT];
static int timer_count_ = 0;

/* this function is called by the irq handler when an interrupt occurs */
static void timer_interrupt_handler() {
  int i = 0;
  gpt_clear(GPTIMER_2);
  jiffies++;
  for (i = 0; i < timer_count_; i++) {
    timers_[i].curr_ticks -= DEFAULT_TICKS;
    if ((timers_[i].init_ticks >= 0) && (timers_[i].curr_ticks <= 0)) {
      timers_[i].curr_ticks = timers_[i].init_ticks;
      timers_[i].handle();
    }
  }

  //gpt_reset(GPTIMER_3); // not needed when overflow mode is used
}

void timer_init() {
  int i = 0;

  for (i = 0; i < MAX_TIMER_COUNT; i++) {
    timers_[i].handle = NULL;
    timers_[i].init_ticks = -1;
    timers_[i].curr_ticks = -1;
  }

  // add handler for the irq of gpt2
  irq_add_handler(GPT2_IRQ, timer_interrupt_handler);

  // initialize gpt2 for 1-ms clock generation
  gpt_init(GPTIMER_2, OVERFLOW_INTERRUPT, DEFAULT_TICKS, 0);
  gpt_start(GPTIMER_2, OVERFLOW_INTERRUPT);
}

void timer_add_handler(void(*handle)(void), int ticks) {
  int i = 0;

  if (timer_count_ >= MAX_TIMER_COUNT) return;

  for (i = 0; i < MAX_TIMER_COUNT; i++) {
    if (timers_[i].init_ticks <= 0) {
      timers_[i].handle = handle;
      timers_[i].init_ticks = ticks;
      timers_[i].curr_ticks = ticks;
      break;
    }
  }

  timer_count_++;
}

void timer_remove_handler(void(*handle)(void)) {
  int i = 0;
  for (i = 0; i < timer_count_; i++) {
    if (timers_[i].handle == handle) {
      /* timers_[i].handle = null; */
      timers_[i].init_ticks = 0;
      timers_[i].curr_ticks = 0;
      timer_count_--;
    }
  }
}

uint64_t timer_system_uptime() {
  // FIXME: something is wrong with the timer irq
  return (jiffies * DEFAULT_TICKS / 2);
}

#define ONE_SEC    1000
#define ONE_MINUTE (ONE_SEC * 60)
#define ONE_HOUR   (ONE_MINUTE * 60)
#define ONE_DAY    (ONE_HOUR * 24)

void timer_parse_time(time_t* time, uint64_t ms) {

  time->days = (int) (ms / ONE_DAY);
  time->hours = (int) (ms / ONE_HOUR - time->days * 24);
  time->minutes = (int) (ms / ONE_MINUTE - (time->hours + time->days * 24) * 60);
  time->seconds = (int) (ms / ONE_SEC - (time->minutes + (time->hours + time->days * 24) * 60) * 60);
  time->miliseconds = (int) (ms % 1000);
}

void timer_sprint_time(time_t time, char* buffer) {
  sprintf(buffer, "%dd %dh %dm %ds", time.days, time.hours,
          time.minutes, time.seconds);
}
