/* ipc_test.c
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
 *  Created on: Jan 4, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <ottos/system.h>
#include <string.h>
#include <stdio.h>
#include <ottos/kernel.h>

#include "../kernel/ipc/ipc.h"

#include "ipc_test.h"

#define CODE_PRINT 0

static const char* namespace = "foobaz";

void ipc_test_sender() {
  char * output;
  while (1) {
    int i = 0;
    message_t msg;
    msg.type = CODE_PRINT;
    for (i = 0; i < 10000; i++) {
      ;
    }
    sprintf(output, "send: %d\n\r", msg.type);
    kernel_print(output);
    sys_send(namespace, &msg);
  }

}

void ipc_test_receiver() {

  message_t message;
  char* output;
  while (1) {
//    while (message == NULL) {
    // TODO: check return value
      sys_receive(namespace, &message);
//    }
    sprintf(output, "received: %d\n\r", message.type);
    kernel_print(output);
  }
}

void ipc_test_module_sender() {
  char * output = "";
  message_t msg;
  msg.type = CODE_PRINT;

  sprintf(output, "send: %d\n\r", msg.type);
  //kernel_print(output);
  printf(output);
  ipc_send_msg(namespace, msg);
}

void ipc_test_module_receiver() {

  message_t message;
  char* output = "";
  while (ipc_receive_msg(namespace, &message) == WAITING) {
    ;
  }
  sprintf(output, "received: %d\n\r", message.type);
  //kernel_print(output);
  printf(output);
}

void ipc_test_module() {

while (1) {
  int i = 0;
  ipc_test_module_sender();
  for (i = 0; i < 10000; i++) {
    ;
  }
  ipc_test_module_receiver();
  for (i = 0; i < 10000; i++) {
    ;
  }
}

}
