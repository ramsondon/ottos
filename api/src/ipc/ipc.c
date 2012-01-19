/* ipc.c
 * 
 * Copyright (c) 2011 The api project.
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
 *  Created on: Jan 19, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */


#include <api/system.h>
#include <api/ipc.h>

/*
 * Binds a namespace of a sending process. Must be called before sending.
 */
int bind(const char* ns) {

  int success = 0;

  // bind the namespace or add the caller as a receiver to the namespace if
  // namespace already exists.
  sys_bind(ns, &success);

  return success;
}

/*
 * Sends a message to namespace ns. sys_bind must be called first. The method
 * will be handled successfully if a receiving process is running, else it will
 * result in an error.
 */
int send(const char* ns, message_t* msg) {

  int success = 0;
  sys_send(ns, msg, &success);

  return success;
}

/*
 * Receives a message_t sent to namespace ns of a sending process.
 * Returns 1 if succeeded else 0
 */
int receive(const char* ns, message_t* msg) {

  int success = 0;

  // wait until message received
  sys_wait_msg(ns);

  // continue if message received and read from through kernel syscall
  sys_receive(ns, msg, &success);

  return success;
}
