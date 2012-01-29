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
#include <ottos/memory.h>

#include <ottos/types.h>
#include <ottos/kernel.h>
#include <ottos/io.h>

#include "ipc.h"
/*
 * IPC message queue global instance
 */
static IPC_MESSAGE_QUEUE ipc_message_queue = {
      0, /* number of pending messages in queue */
      NULL, /* IPC_MESSAGE* head of list */
      NULL /* IPC_MESSAGE* last element of list */
};

/*
 * IPC namespace queue global instance
 */
static IPC_NAMESPACE_QUEUE ipc_namespace_queue = {
      0, /* namespace size */
      NULL /* IPC_NAMESPACE head of list */
};

static IPC_NAMESPACE* ipc_lookup_namespace(const char* ns) {
  /* iterate through the namespace register and lookup for matches*/
  IPC_NAMESPACE* current = ipc_namespace_queue.head;
  while (current != NULL) {
    if (strcmp(current->ns, ns) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

static IPC_CLIENT* ipc_lookup_client(IPC_CLIENT_QUEUE queue, pid_t pid) {
  /* we are now at the correct namespace */
  IPC_CLIENT* client = queue.head;

  while (client != NULL) {
    if (client->pid == pid) {

      /* we found our receiver */
      return client;
    }
    client = client->next;
  }
  return NULL;
}

/*
 * Returns IPC_RECEIVER* if a receiver with pid_t pid is registered at the namespace
 * ns, else NULL
 */
static IPC_CLIENT* ipc_lookup_receiver(IPC_NAMESPACE* namespace, pid_t pid) {
  return ipc_lookup_client(namespace->receivers, pid);
}

static IPC_CLIENT* ipc_lookup_sender(IPC_NAMESPACE* namespace, pid_t pid) {
  return ipc_lookup_client(namespace->senders, pid);
}

/*
 * Returns the number of receivers registered for IPC_NAMESPACE ns
 */
static int ipc_nr_of_receiver(IPC_NAMESPACE* ns) {
  return ns->receivers.size;
}

/*
 * Returns the number of senders registered for IPC_NAMESPACE ns
 */
static int ipc_nr_of_sender(IPC_NAMESPACE* ns) {
  return ns->senders.size;
}

/*
 * Creates a new namespace. allocates memory which must be
 * freed outside.
 */
static IPC_NAMESPACE* ipc_create_namespace(const char* ns) {

  IPC_NAMESPACE* namespace = malloc(sizeof(IPC_NAMESPACE));
  namespace->next = NULL;
  namespace->ns = ns;
  namespace->receivers.head = NULL;
  namespace->receivers.size = 0;
  namespace->senders.head = NULL;
  namespace->senders.size = 0;
  return namespace;
}

/*
 * Create a new IPC_CLIENT. Allocates memory which must be
 * freed ouside.
 */
static IPC_CLIENT* ipc_create_client(pid_t pid) {

  IPC_CLIENT* client = malloc(sizeof(IPC_CLIENT));
  client->pid = pid;
  client->next = NULL;
  return client;
}

static IPC_NAMESPACE* ipc_do_register_namespace(const char* ns) {

  IPC_NAMESPACE* namespace = ipc_lookup_namespace(ns);

  /* register namespace if not exists */
  if (namespace == NULL) {
    namespace = ipc_create_namespace(ns);
    namespace->next = ipc_namespace_queue.head;
    ipc_namespace_queue.head = namespace;

    /* increment namspace count */
    ipc_namespace_queue.size++;
  }
  return namespace;
}

static int ipc_do_unregister_all_clients(IPC_CLIENT_QUEUE* queue) {

  IPC_CLIENT* client = queue->head;
  while (client != NULL) {
    free(client);
    client = client->next;
  }
  queue->head = NULL;
  queue->size = 0;
  return SUCCESS;
}

static int ipc_do_unregister_namespace(const char* ns, pid_t pid) {

  IPC_NAMESPACE* namespace = ipc_namespace_queue.head;
  IPC_NAMESPACE* prev = NULL;
  while (namespace != NULL) {
    if (strcmp(namespace->ns, ns) == 0) {
      if (prev != NULL) {
        prev->next = namespace->next;
      }
      namespace->ns = NULL;
      namespace->next = NULL;

      // remove all registered receivers and senders
      ipc_do_unregister_all_clients(&namespace->senders);
      ipc_do_unregister_all_clients(&namespace->receivers);

      free(namespace);
      namespace = NULL;
      ipc_namespace_queue.size--;

      if (ipc_namespace_queue.size <= 0) {
        ipc_namespace_queue.head = NULL;
      }
      return SUCCESS;
    }
    prev = namespace;
    namespace = namespace->next;
  }
  return WAITING;
}

static IPC_CLIENT* ipc_do_register_sender(IPC_NAMESPACE* namespace, pid_t pid) {

  IPC_CLIENT* sender = ipc_lookup_sender(namespace, pid);

  if (sender == NULL) {
    /* allocate a new IPC_CLIENT */
    sender = ipc_create_client(pid);
    sender->next = namespace->senders.head;
    namespace->senders.head = sender;

    /* increment sender count */
    namespace->senders.size++;
  }
  return sender;
}

static IPC_CLIENT* ipc_do_register_receiver(IPC_NAMESPACE* namespace, pid_t pid) {

  IPC_CLIENT* receiver = ipc_lookup_receiver(namespace, pid);

  if (receiver == NULL) {

    /* register receiver in existing namespace */
    receiver = ipc_create_client(pid);
    receiver->next = namespace->receivers.head;
    namespace->receivers.head = receiver;

    /* increment receiver count for namespace */
    namespace->receivers.size++;
  }
  return receiver;
}

/*
 * Removes the cur pointer from the queue. Does not free any memory
 * cur and prev must be members of the queue
 */
static int ipc_remove_client_from_queue(IPC_CLIENT_QUEUE* queue,
                                        IPC_CLIENT* cur, IPC_CLIENT* prev) {

  /*
   * if the previous is NULL then we just can set our head of the queue to the
   * current's next pointer
   */
  if (prev == NULL) {
    queue->head = cur->next;
  } else {
    prev->next = cur->next;
  }
  queue->size--;

  return SUCCESS;
}

static int ipc_do_unregister_client(IPC_CLIENT_QUEUE* queue, pid_t pid) {

  IPC_CLIENT* cur = queue->head;
  IPC_CLIENT* prev = NULL;

  while (cur != NULL) {
    if (cur->pid == pid) {
      /* we found our IPC_CLIENT - so free it in namespace */
      ipc_remove_client_from_queue(queue, cur, prev);

      // free  memory for IPC_CLIENT
      free(cur);
      cur = NULL;
      return SUCCESS;
    }
    prev = cur;
    cur = cur->next;
  }
  return SUCCESS;
}

static int ipc_do_unregister_receiver(IPC_NAMESPACE* namespace, pid_t pid) {
  return ipc_do_unregister_client(&namespace->receivers, pid);
}

static int ipc_do_unregister_sender(IPC_NAMESPACE* namespace, pid_t pid) {
  return ipc_do_unregister_client(&namespace->senders, pid);
}

static int ipc_can_send(const char* ns, pid_t pid) {

  IPC_NAMESPACE* namespace = ipc_lookup_namespace(ns);

  if (namespace == NULL || ipc_nr_of_receiver(namespace) <= 0) {
    return WAITING;
  }
  return SUCCESS;
}

/*
 * Adds the IPC_MESSAGE to the IPC_MESSAGE_QUEUE instance ipc_message_queue.
 * Does not care about namespaces.
 */
static int ipc_add_to_queue(IPC_MESSAGE_QUEUE* queue, IPC_MESSAGE* ipcmsg) {

  // set head and last if null
  if (queue->head == NULL) {
    queue->head = ipcmsg;
    queue->last = ipcmsg;
  } else {
    // set current as last message
    queue->last->next = ipcmsg;
    queue->last = ipcmsg;
  }
  queue->size++;

  return SUCCESS;
}

static int ipc_remove_from_queue(IPC_MESSAGE_QUEUE* queue, IPC_MESSAGE* msg,
                                 IPC_MESSAGE* prev) {

  // is this message the head of the list
  if (msg == queue->head) {
    queue->head = msg->next;

    // is this message the only element in the queue
    if (msg->next == NULL) {
      queue->last = NULL;
    }

    // at this point we know that our msg is not the head but the last in queue
  } else {
    if (msg == queue->last) {
      queue->last = prev;
    }
    prev->next = msg->next;
  }
  queue->size--;

  return SUCCESS;
}

/*
 * Removes all messages to be received of process with pid_t pid.
 */
static void ipc_remove_all_msg(pid_t pid) {

  IPC_MESSAGE* current = ipc_message_queue.head;
  IPC_MESSAGE* prev = NULL;

  while (current != NULL) {
    if (current->receiver == pid) {
      // remove message from queue
      ipc_remove_from_queue(&ipc_message_queue, current, prev);

      // free message
      free(current->message);
      current->message = NULL;
      free(current);
      current = NULL;
      return;
    }
    prev = current;
    current = current->next;
  }
}

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

int ipc_lookup_msg_for(pid_t pid) {
  IPC_MESSAGE* current = ipc_message_queue.head;

  while (current != NULL) {
    if (current->receiver == pid) {
      return SUCCESS;
    }
    current = current->next;
  }
  return WAITING;
}

int ipc_lookup_msg_concrete(const char* ns, pid_t pid) {
  IPC_MESSAGE* current = ipc_message_queue.head;

  while (current != NULL) {
    if (strcmp(current->ns, ns) == 0 && current->receiver == pid) {
      return SUCCESS;
    }
    current = current->next;
  }
  return WAITING;
}

/*
 * Binds the calling process as a sender to the namespace ns
 */
int ipc_bind(const char* ns, pid_t pid) {

  IPC_NAMESPACE* namespace = ipc_do_register_namespace(ns);
  ipc_do_register_receiver(namespace, pid);
  return SUCCESS;
}

/*
 * Unbinds the calling process as a receiver from the namespace
 */
int ipc_unbind(const char* ns, pid_t pid) {

  IPC_NAMESPACE* namespace = ipc_lookup_namespace(ns);

  if (namespace != NULL) {
    // unregister sender process from namespace
    ipc_do_unregister_receiver(namespace, pid);
    // unregister namespace if no sending process
    if (ipc_nr_of_receiver(namespace) <= 0) {
      ipc_do_unregister_namespace(ns, pid);
      ipc_remove_all_msg(pid);
      return SUCCESS;
    }
  }
  return WAITING;
}

/*
 * Makes a copy of the message_t msg and adds it to the message buffer.
 * If no receiver is available the result will be waiting.
 */
int ipc_send_msg(const char* ns, message_t msg, const void* content, pid_t sender) {

  message_t* msgcpy = NULL;
  IPC_MESSAGE* new_ipc_msg = NULL;
  IPC_NAMESPACE* namespace = NULL;
  IPC_CLIENT* receiver = NULL;

  /****************************************************************************
   *  TODO: ramsondon@gmail.com
   *
   *  BEGIN REFACTOR
   */
  // throw message away if ns is not registered or no process is listening
  if (ipc_can_send(ns, sender) == WAITING) {
    return WAITING;
  }

  namespace = ipc_lookup_namespace(ns);
  receiver = namespace->receivers.head;

  /*
   * END REFACTOR
   * **************************************************************************
   */

  while (receiver != NULL) {

    size_t size = msg.count * msg.size;

    // create message
    msgcpy = malloc(sizeof(message_t));
    msgcpy->type = msg.type;
    msgcpy->count = msg.count;
    msgcpy->size = msg.size;

    msgcpy->content = malloc(size);

    // copy user space content into kernel message copy content
    memcpy(msgcpy->content, content, size);
    new_ipc_msg = malloc(sizeof(IPC_MESSAGE));

    /* set the pid_t of the sender */
    new_ipc_msg->sender = sender;
    new_ipc_msg->receiver = receiver->pid;
    new_ipc_msg->message = msgcpy;
    new_ipc_msg->ns = ns;
    new_ipc_msg->next = NULL;

    // add message to queue
    ipc_add_to_queue(&ipc_message_queue, new_ipc_msg);
    receiver = receiver->next;
  }
  return SUCCESS;
}

int ipc_receive_msg(const char* ns, message_t* msg, void* content, pid_t pid) {

  IPC_MESSAGE* current = ipc_message_queue.head;
  IPC_MESSAGE* prev = NULL;
  IPC_NAMESPACE* namespace = ipc_lookup_namespace(ns);

  // check if namespace has been registered by a sender
  if (namespace != NULL && ipc_lookup_receiver(namespace, pid) != NULL) {

    while (current != NULL) {

      if (strcmp(current->ns, ns) == 0) {

        size_t size = msg->count * msg->size;

        // set ouptut message
        msg->type = current->message->type;
        msg->count = current->message->count;
        msg->size = current->message->size;

        // copy kernel space message content into user space content pointer
        memcpy(content, current->message->content, size);

        // remove message from queue
        ipc_remove_from_queue(&ipc_message_queue, current, prev);

        // free message
        free(current->message->content);
        free(current->message);
        free(current);
        return SUCCESS;
      }
      prev = current;
      current = current->next;
    }
  }
  return WAITING;
}

/*
 * Kills all pending Namespaces and messages for pid_t pid if not in use
 */
int ipc_kill_receiver(pid_t pid) {

  IPC_NAMESPACE* curns = ipc_namespace_queue.head;
  IPC_NAMESPACE* prev = NULL;

  /*
   * TODO: (ramsondon@gmail.com) Add a flag to Process control block for pending
   * messages if no pending messages for proc with pid_t pid do not handle this
   * function
   */

  while (curns != NULL) {
    IPC_NAMESPACE* temp = curns;

    // we found our pid as a receiver in the current namespace
    if (ipc_lookup_receiver(temp, pid) != NULL) {

      // so we have to unregister it
      ipc_do_unregister_receiver(temp, pid);

      // if we now have no more receivers of this namespace we can remove it.
      if (ipc_nr_of_receiver(temp) <= 0) {

        // free all registered senders
        ipc_do_unregister_all_clients(&temp->senders);

        if (prev == NULL) {
          ipc_namespace_queue.head = curns->next;
          free(temp);
        } else {
          prev->next = curns->next;
          free(temp);
        }
        temp = NULL;
        curns = curns->next;
        ipc_namespace_queue.size--;
        // remove all messages sent by this pid
        ipc_remove_all_msg(pid);
        // the namespace has not been removed

        /* TODO: (ramsondon@gmail.com) refactor the following lines */
      } else {
        prev = curns;
        curns = curns->next;
      }
    } else {
      prev = curns;
      curns = curns->next;
    }
  }
  return SUCCESS;
}
