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
#include <ottos/io.h>
#include <api/io.h>
#include <api/ipc.h>

void array_init(char * array, int size) {
  int i = 0;
  for (i = 0; i < size; i++) {
    array[i] = 0;
  }
}

int main(int argc, char **argv) {

  print("server try bind\n\r");

  if (bind("test") == IPC_SUCCESS) {
    print("namespace bind successfull\n\r");
  } else {
    print("namespace binding error\n\r");
  }

  while (1) {
    char msgtype[10] = { 0 };
    int* content = malloc(sizeof(int)*4);

    message_t msg;
    msg.type = 0;
    msg.size = sizeof(int);
    msg.count = 4;
    msg.content = content;

    print("[SERVER] [TYPE] ");

    // receive message
    if (receive("test", &msg) == IPC_SUCCESS) {
      int i = 0;
      int* c = (int*)msg.content;
      itoa(msg.type, msgtype, 10);
      print(msgtype);

      print(" [MESSAGE] ");

      // print message
      for (i = 0; i < 4; i++) {
          char convert[1] = {'\0'};
          itoa(c[i], convert, 10);
          print(convert);
      }
    } else {
      print("[RECEIVING ERROR]");
    }
    print("\n\r");
    free(content);
  }
  return 0;
}
