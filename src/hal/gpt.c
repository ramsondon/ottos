/* gpt.c
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

#include "gpt.h"
#include <bits.h>
#include <ottos/types.h>
#include <arch/arm/omap353x_timer.h>

static int gpt_baseaddresses[GPTIMER_COUNT] = { GPTIMER1_BASE, GPTIMER2_BASE, GPTIMER3_BASE,
                                                    GPTIMER4_BASE, GPTIMER5_BASE, GPTIMER6_BASE,
                                                    GPTIMER7_BASE, GPTIMER8_BASE, GPTIMER9_BASE,
                                                    GPTIMER10_BASE, GPTIMER11_BASE };

void gpt_init(device_t gpt_timer, enum TimerInterruptMode interrupt_mode, int ticks, int load_ticks) {
  int overflow_ticks = 1;
  int pos_inc = ((((int)(CLOCK_FREQUENZE*overflow_ticks))+1)*1000000)-(CLOCK_FREQUENZE*overflow_ticks*1000000);
  int neg_inc = (((int)(CLOCK_FREQUENZE*overflow_ticks))*1000000)-(CLOCK_FREQUENZE*overflow_ticks*1000000);

  /* TODO: implement different interrupt modes; currently only overflow interrupts work */
  gpt_stop(gpt_timer);

  switch (gpt_timer) {
    case GPTIMER_1:
    case GPTIMER_2:
    case GPTIMER_10:
      /* millisecond timers */
      *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TLDR)) = 0xFFFFFFE0; // ~1ms
      *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TCRR)) = 0xFFFFFFE0; // ~1ms
      *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TPIR)) = pos_inc;
      *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TNIR)) = neg_inc;
      break;

    default:
      *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TLDR)) = load_ticks;
      *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TMAR)) = ticks;
      break;
  }

  /* clear int status bits */
  *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TISR)) = ~0;

  /* counts the number of overflows; if this register matches the GPT_TOWR register an interrupt is thrown */
  *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TOCR)) = 0;

  /* number of overflow which have to occurs until an interrupt is thrown */
  *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TOWR)) = 10;

  /* set clock source to the 32 */
  CLEAR_BIT((mem_address_t)(CM_CLKSEL_PER), CM_CLKSEL_GPT2);

  SET_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TCLR), TCLR_COMPARE);
  SET_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TCLR), TCLR_AUTORELOAD);

  /* enable overflow and match trigger */
  SET_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TCLR), TCLR_TRG_OVF_MAT);

  /* disable prescale */
  CLEAR_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TCLR), TCLR_PRESCALE);
}

void gpt_start(device_t gpt_timer, enum TimerInterruptMode interrupt_mode) {
  SET_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TIER), interrupt_mode);
  SET_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TCLR), TCLR_START);
}

/*
 * Stops the timer and disable interrupts from this timer
 */
void gpt_stop(device_t gpt_timer) {
  *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TIER)) &= 0;
  CLEAR_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TCLR), TCLR_START);
  gpt_reset(gpt_timer);
}

/*
 * Clear all pending timer interrupts
 */
void gpt_clear(device_t gpt_timer) {
  SET_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TISR), TISR_CAPTURE);
  SET_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TISR), TISR_OVERFLOW);
  SET_BIT((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TISR), TISR_MATCH);
}

/*
 * Resets the timer --> set the timer counter register back to 0
 */
void gpt_reset(device_t gpt_timer) {
  *((mem_address_t)(gpt_baseaddresses[gpt_timer]+GPT_TCRR)) = 0;
}
