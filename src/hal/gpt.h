/* gpt.h
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

#ifndef GPT_H_
#define GPT_H_

#include <ottos/dev/device.h>

enum TimerInterruptMode {
  MATCH_INTERRUPT     = 0,
  OVERFLOW_INTERRUPT  = 1,
  CAPTURE_INTERRUPT   = 2
};

void gpt_init(device_t gpt_timer, enum TimerInterruptMode interrupt_mode, int ticks, int load_ticks);
void gpt_start(device_t gpt_timer, enum TimerInterruptMode interrupt_mode);
void gpt_stop(device_t gpt_timer);
void gpt_clear(device_t gpt_timer);
void gpt_reset(device_t gpt_timer);

#endif /* GPT_H_ */
