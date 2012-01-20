/* main.c
 * 
 * Copyright (c) 2011 The IPCHost project.
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
 *  Created on: Jan 13, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <stdlib.h>
#include <api/io.h>
#include <api/ipc.h>
#include <ottos/io.h>

void busy_wait() {
  int i = 0;
  for (i=0; i < 100000; i++) {
      ;
    }
}

int main(int argc, char **argv) {

  int message_code = 0;

  while(1) {

    char msgtype[10] = {0};
    int content[4] = {0};
    message_t msg;
    msg.type = message_code++;
    msg.size = sizeof(content);
    msg.count = 1;
    msg.content = &content;

    content[0] = 4;
    content[1] = 7;
    content[2] = 1;
    content[3] = 1;

    // wait some time to send message
    busy_wait();
    print("[CLIENT] [SEND] [TYPE] ");

    if (send("test", &msg) == IPC_SUCCESS) {
      itoa(msg.type, msgtype, 10);
      print((char*)msgtype);
    } else {
      print("[ERROR WHILE SENDING]");
    }
    print("\n\r");
  }

  return 0;
}
