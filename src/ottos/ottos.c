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
#include "../../bin/serial_test.h"
#include <ottos/kernel.h>
#include "kernel/intc/irq.h"
#include "kernel/pm/process.h"
#include "kernel/timer/timer.h"
#include "dev/devices.h"
#include "../hal/uart.h"

void timer_test() {
  irq_started = FALSE;

  process_table_init();

  process_create(1, (int)toggle_led1);
  process_create(1, (int)toggle_led2);

  devices_init();

  irq_init();

  timer_init();
  //timer_add_handler(toggle_led_1, 5000);
  //timer_add_handler(toggle_led_2, 10000);

  irq_register_context_switch();

  irq_enable();
  kernel_to_user_mode();
}

void devices_test() {
  devices_init();
}

void process_test() {
  irq_started = FALSE;

  process_table_init();

  process_create(1, (int)toggle_led1_yield);
  process_create(1, (int)toggle_led2_yield);

  devices_init();

  // switch to user mode
  kernel_to_user_mode();
  sys_yield();

}

void serial_test() {

  devices_init();

  serial_test_create();
  serial_test_start_msg();
  while(1) {
    serial_test_communicate();
  }
}


int main(int argc, char **argv) {

  process_test();
  //timer_test();
  //serial_test();


  for(;;);

	return 0;
}
