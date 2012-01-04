/* ipc.c
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
 *  Created on: Dec 22, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */
#include <string.h>
#include <stdlib.h>

#include <ottos/types.h>

#include "ipc.h"

int ipc_lookup_msg(const char* ns) {
  IPC_MESSAGE* current = ipc_message_queue.head;

  while (current != NULL) {
    if (strcmp(current->ns, ns) == 0) {
      return SUCCESS;
    }
    current = current->next;
  }
  return WAITING;
}

/*
 * Adds the IPC_MESSAGE to the IPC_MESSAGE_QUEUE instance ipc_message_queue.
 * Does not care about namespaces.
 */
static int ipc_add_to_queue(IPC_MESSAGE* ipcmsg) {

  // set head and last if null
  if (ipc_message_queue.head == NULL) {
    ipc_message_queue.head = ipcmsg;
    ipc_message_queue.last = ipcmsg;
  } else {
    // set current as last message
    ipc_message_queue.last->next = ipcmsg;
    ipc_message_queue.last = ipcmsg;
  }
  return SUCCESS;
}

static int ipc_remove_from_queue(IPC_MESSAGE* msg, IPC_MESSAGE* prev) {

  // is this message the head of the list
  if (msg == ipc_message_queue.head) {
    ipc_message_queue.head = msg->next;

    // is this message the only element in the queue
    if (msg->next == NULL) {
      ipc_message_queue.last = msg->next;
    }

    // at this point we know that our msg is not the head but the last in queue
  } else {
    if (msg == ipc_message_queue.last) {
      ipc_message_queue.last = prev;
    }
    prev->next = msg->next;
  }
  // free message
  free(msg->message);
  free(msg);
  return SUCCESS;
}

int ipc_send_msg(const char* ns, message_t msg) {

  // create message
  message_t* msgcpy = malloc(sizeof(message_t));
  IPC_MESSAGE* new_ipc_msg = malloc(sizeof(IPC_MESSAGE));

  msgcpy->code = msg.code;
  msgcpy->pid_t = msg.pid_t;

  new_ipc_msg->message = msgcpy;
  new_ipc_msg->ns = ns;
  new_ipc_msg->next = NULL;

  // add message to queue
  ipc_add_to_queue(new_ipc_msg);

  return SUCCESS;
}

int ipc_receive_msg(const char* ns, message_t* msg) {

  IPC_MESSAGE* current = ipc_message_queue.head;
  IPC_MESSAGE* prev = NULL;
  while (current != NULL) {
    if (strcmp(current->ns, ns) == 0) {

      // set ouptut message
      msg->code = current->message->code;
      msg->pid_t = current->message->pid_t;

      // remove message from queue
      ipc_remove_from_queue(prev, current);

      return SUCCESS;
    }
    prev = current;
    current = current->next;
  }
  return WAITING;
}

