///* io_test.c
// *
// * Copyright (c) 2011 The ottos_led_blinker project.
// *
// * This work is free software; you can redistribute it and/or modify it under
// * the terms of the GNU Lesser General Public License as published by the Free
// * Software Foundation; either version 2.1 of the License, or (at your option)
// * any later version.
// *
// * This work is distributed in the hope that it will be useful, but without
// * any warranty; without even the implied warranty of merchantability or
// * fitness for a particular purpose. See the GNU Lesser General Public License
// * for more details.
// *
// * You should have received a copy of the GNU Lesser General Public License
// * along with this library; if not, write to the Free Software Foundation,
// * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// *
// *
// *  Created on: 14 Jan 2012
// *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
// */

#include <stdio.h>
#include <api/system.h>
#include <api/led.h>
#include <api/io.h>

int main() {

  int argc = 0;
  char** argv = sys_read_arguments(&argc);

  if (argc < 2) {
    print("usage: ottos_led [led_number]\r\n");
    sys_exit();
  } else {

    int led_number = argv[1][0] - '0';
    char buffer[100] = {0};
    sprintf(buffer, "led_number: string:%s int:%d\r\n", argv[1], led_number);
    print(buffer);

    if (led_number < 0 || led_number > 2) {
      print("led_number value must be 1 or 2\r\n");
      sys_exit();
    } else {
      int i = 0;
      led_t led = (led_t) led_number;

      for (i = 0;; i++) {
        if (i % 100000 == 0) {
          led_toggle(led);
        }
      }
    }
  }

  return 0;
}
