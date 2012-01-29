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
 * TODO:  implement global maximum number of messages to be sent
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
    pid_t receiver;
    const char* ns;
    struct message_t* message;
    struct ipc_message_t* next;
} IPC_MESSAGE;

/*
 * IPC message queue
 */
typedef struct ipc_message_queue_t {
    int size;
    IPC_MESSAGE* head;
    IPC_MESSAGE* last;
} IPC_MESSAGE_QUEUE;

/*
 * IPC receiver
 */
typedef struct ipc_client_t {
    pid_t pid;
    struct ipc_client_t* next;
} IPC_CLIENT;

typedef struct ipc_client_queue_t {
    int size;
    IPC_CLIENT* head;
} IPC_CLIENT_QUEUE;


typedef struct ipc_namespace_t {
    const char* ns;
    struct ipc_client_queue_t receivers;
    struct ipc_client_queue_t senders;
    struct ipc_namespace_t* next;
} IPC_NAMESPACE;

/*
 * IPC Namespace register
 */
typedef struct ipc_namespace_queue_t {
    int size;
    IPC_NAMESPACE* head;
} IPC_NAMESPACE_QUEUE;


/*
 * Returns SUCCESS if a message is available for a certain namespace ns, else
 * WAITING.
 */
EXTERN int ipc_lookup_msg(const char* ns);

EXTERN int ipc_lookup_msg_for(pid_t pid);

EXTERN int ipc_lookup_msg_concrete(const char* ns, pid_t pid);

/*
 * Bind has to be called by the receiver process
 * Binds the calling process as a receiver for messages at namespace ns
 */
EXTERN int ipc_bind(const char* ns, pid_t pid);

/*
 * Unbinds a namespace of a receiving process
 */
EXTERN int ipc_unbind(const char* ns, pid_t pid);

/*
 * Sends a message_t to the a process listening to namespace ns
 */
EXTERN int ipc_send_msg(const char* ns, message_t msg, const void* content, pid_t pid);

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
EXTERN int ipc_receive_msg(const char* ns, message_t* msg, void* content, pid_t pid);

/*
 * removes namespaces and pending messages for process with pid
 * should only be called on garbage collection or process deletion
 */
EXTERN int ipc_kill_receiver(pid_t pid);

#endif /* OTTOS_KERNEL_IPC_IPC_H_ */
