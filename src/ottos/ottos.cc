/* ottos.cc
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
 *  Created on: Oct 19, 2011
 *      Author: Matthias Schmid <m.schmid@students.fhv.at>
 */

#include <ottos/system.h>

#include "../../bin/led_test.h"
#include "kernel/intc/InterruptHandler.h"
#include "kernel/timer/GPTimerController.h"
#include "kernel/pm/Process_c.h"

#include <stdio.h>

Scheduler* sched;

#define GPIO5_OE 0x49054034       /* GPIO5 output enable address */
#define GPIO5_DATAOUT 0x4905603C  /* GPIO5 data out register address*/

#define SET_BIT(bit) (1<<bit)     /* Sets the specified bit to 1 */

void toggle_led_1() {
  volatile int i;
  //printf("Timer 3 fired interrupt... \n");

  GPTimerController gptc;
  gptc.clear(3);

  *(volatile unsigned long *)GPIO5_DATAOUT ^= SET_BIT(22);

  gptc.reset(3);
}

void toggle_led_2() {
  volatile int i;
  //printf("Timer 4 fired interrupt... \n");

  GPTimerController gptc;
  gptc.clear(4);

  *(volatile unsigned long *)GPIO5_DATAOUT ^= SET_BIT(21);

  gptc.reset(4);
}


int main(int argc, char **argv) {
  /*
  Kernel k = Kernel();
  k.init();
  k.run();
  */

  // set up gptimer 1
  InterruptController ic = InterruptController();
  ic.add_handler(39, toggle_led_1);
  ic.add_handler(40, toggle_led_2);

  printf("init handler... \n");

  GPTimerController gptController;
  gptController.init(3, 1000000);
  gptController.start(3, MATCH_INTERRUPT);
  printf("init GPT2... \n");

  gptController.init(4, 1500000);
  gptController.start(4, MATCH_INTERRUPT);
  printf("init GPT3... \n");

  // enable IRQs
  _enable_interrupts();

  gptController.reset(3);
  gptController.reset(4);

  while (true) {}

  return 0;
}


int main_(int argc, char **argv) {

//	kernel = new Kernel();
//	kernel->init();
//	kernel->run();



	started = FALSE;

	init_process_table();
	create_process(1, (int)toggle_led1);
	create_process(1, (int)toggle_led2);

	// switch to user mode
	asm("\t CPS 0x10");
	sys_yield();

//	// schedule the next process
//	Process* nextProcess = kernel->process_manager()->process_table()->at(kernel->scheduler()->next());
//	kernel->process_manager()->set_current_process(nextProcess->pid());
//
//	printf("next pid: %d\n", nextProcess->pid());
//
//
//		// switch to the process stack
//	bla = nextProcess->stack_pointer();
//	bla2 = (int)nextProcess->func();
//	asm("\t LDR r5, bla_a \n" \
//		"\t LDR sp, [r5] \n" \
//
//	// start the process
//
//		"\t LDR r5, bla2_a \n" \
//		"\t LDR r5, [r5] \n" \
//
//	// switch to usermode
//		"\t CPS 0x10 \n" \
//
//	// jump to process
//		"\t MOV pc, r5");

	return 0;
}
