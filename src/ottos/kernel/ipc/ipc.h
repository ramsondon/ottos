/* ipc.h
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

#ifndef OTTOS_KERNEL_IPC_IPC_H_
#define OTTOS_KERNEL_IPC_IPC_H_

/*
 * TODO:  - Critical Regions
 *        - Mutual Exclusion with Busy Waiting
 *        - Sleep and Wakeup
 *        - Semaphores
 *        - Message Passing - send, receive
 *        - waitpid(pid_t pid)
 *
 * TODO:  implement BLOCK process while waiting for a message for a certain
 *        namespace
 */

#include <ottos/const.h>
#include <ottos/types.h>

#define SUCCESS           1
#define WAITING           0

/*
 * IPC message
 */
typedef struct ipc_message_t {
    pid_t sender;
    const char* ns;
    struct message_t* message;
    struct ipc_message_t* next;
} IPC_MESSAGE;

/*
 * IPC message queue
 */
typedef struct ipc_message_queue_t {
    int pending;
    IPC_MESSAGE* head;
    IPC_MESSAGE* last;
} IPC_MESSAGE_QUEUE;

/*
 * IPC message queue global reference
 */
static IPC_MESSAGE_QUEUE ipc_message_queue = {
   0,
   NULL,
   NULL
};

/*
 * Returns SUCCESS if a message is available for a certain namespace ns, else
 * WAITING.
 */
EXTERN int ipc_lookup_msg(const char* ns);

/*
 * Sends a message_t to the a process listening to namespace ns
 */
EXTERN int ipc_send_msg(const char* ns, message_t msg);

/*
 * Receives all message_t sent to namespace msg
 *
 * This method does not block the kernel!! If no message is available for that
 * namespace the result will be WAITING
 *
 * @param ns Namespace
 * @param msg message received message
 * @return SUCCESS = 1, WAITING = 0
 */
EXTERN int ipc_receive_msg(const char* ns, message_t* msg);

/*
 * Removes all messages of process with pid_t pid. Has to be called on Process
 * deletion.
 */
EXTERN void ipc_remove_all_msg(pid_t pid);


#endif /* OTTOS_KERNEL_IPC_IPC_H_ */
