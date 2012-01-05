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
#include <ottos/kernel.h>
#include <ottos/const.h>
#include <ottos/timer.h>

//#include "../../bin/ipc_test.h"
#include "../../bin/led_test.h"
#include "../../bin/serial_test.h"
#include "../../bin/uptime_test.h"
#include "../../bin/i2c_test.h"
#include "../drivers/i2c/i2c.h"

#include "kernel/intc/irq.h"
#include "kernel/ipc/ipc.h"
#include "kernel/pm/process.h"
#include "dev/devices.h"

#include "../fs/fs.h"
#include "../tty/tty.h"

void timer_test() {

  process_create(1, (int) toggle_led1);
  process_create(1, (int) toggle_led2);

  irq_init();
  timer_init();

  // timer_add_handler(toggle_led_1, 5000);
  // timer_add_handler(toggle_led_2, 10000);

  irq_register_context_switch();
  irq_enable();
  kernel_to_user_mode();
}

void serial_test() {

  // process_create(1, (int)serial_test_test_yield);
  // process_create(1, (int)toggle_led1_yield);
  // process_create(1, (int)toggle_led2_yield);


  //process_create(1, (int) led1_on);
  //process_create(1, (int) led1_off);
  process_create(1, (int) toggle_led1);
  process_create(1, (int) toggle_led2);
  //process_create(1, (int) serial_test_write_1);
  //process_create(1, (int) serial_test_write_2);
  //process_create(1, (int) serial_test_write_3);
  //process_create(1, (int) serial_test_write_4);
  //process_create(1, (int) serial_test_write_5);
  process_create(1, (int) serial_test_calculator);

  serial_test_create();
  irq_init();
  timer_init();

  irq_register_context_switch();
  irq_enable();
  kernel_to_user_mode();

  // sys_yield();
  // serial_test_test();
}

void serial_test_calc() {

  process_create(1, (int) toggle_led1);
  process_create(1, (int) serial_test_calculator);

  irq_init();
  timer_init();
  irq_register_context_switch();
  irq_enable();
  kernel_to_user_mode();
}

//void ipc_test_send_receive() {
//
//  process_table_init();
//  process_create(1, (int) toggle_led1);
//  process_create(1, (int) toggle_led2);
//  process_create(1, (int) ipc_test_sender);
//  process_create(1, (int) ipc_test_receiver);
//
//  devices_init();
//
//  irq_init();
//
//  timer_init();
//
//  irq_register_context_switch();
//
//  irq_enable();
//  kernel_to_user_mode();
//}

void process_exit_test() {

  process_create(1, (int) serial_test_write_exit_1);
  process_create(1, (int) serial_test_write_exit_2);
  process_create(1, (int) toggle_led1);

  irq_init();
  timer_init();
  irq_register_context_switch();
  irq_enable();
  kernel_to_user_mode();
}

void system_time_test() {

  process_create(1, (int) toggle_led1);
  process_create(1, (int) uptime_test);

  irq_init();
  timer_init();
  irq_register_context_switch();
  irq_enable();
  kernel_to_user_mode();
}

void i2c_test() {
  i2c_init();
  pulse_leds();
}

void test_ipc_module_sender() {

  const char* namespace = "foobar";
  char * output1 = "";
  static int code = 0;
  while (1) {
    message_t msg;
    msg.type = code++;

    sprintf(output1, "send: %d\n", msg.type);
    //kernel_print(output);
    printf(output1);
    ipc_send_msg(namespace, msg);
  }
}

void test_ipc_module_receiver() {

  const char* namespace = "foobar";
  while (1) {
    message_t message;
    char* output = "";
    while (ipc_receive_msg(namespace, &message) == WAITING) {
      ;
    }
    sprintf(output, "received: %d\n", message.type);
    //kernel_print(output);
    printf(output);
  }
}

void test_ipc_module() {

  process_create(1, (int) test_ipc_module_sender);
  process_create(1, (int) test_ipc_module_receiver);

  devices_init();

  irq_init();

  timer_init();

  irq_register_context_switch();

  irq_enable();
  kernel_to_user_mode();

  //  while (1) {
  //    int i = 0;
  //    test_ipc_module_sender();
  //    for (i = 0; i < 10000; i++) {
  //      ;
  //    }
  //    test_ipc_module_receiver();
  //    for (i = 0; i < 10000; i++) {
  //      ;
  //    }
  //  }

}

static void loop_forever() {
  for (;;)
    ;
}

int main(int argc, char **argv) {

  // these methods has to be called for EVERY test method
  process_table_init();
  devices_init();

  // these methods are the specific tests for every module

  //  process_test();
  //  timer_test();
  //  serial_test();
  //  serial_test_calc();
  //  process_exit_test();

  // fs_test();
  //  i2c_test();
  //  uptime_test();
  tty_test();
  //  system_time_test();
  //  uptime_test();

  // do an endless loop
  loop_forever();
  return 0;
}
