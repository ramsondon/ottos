/* ipc.h
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

#ifndef API_IPC_H_
#define API_IPC_H_

#define IPC_SUCCESS 1
#define IPC_WAITING 0


/*
 * Binds a namespace of a sending process. Must be called before sending.
 */
EXTERN int bind(const char* ns);

/*
 * Sends a message to namespace ns. sys_bind must be called first. The method
 * will be handled successfully if a receiving process is running, else it will
 * result in an error.
 */
EXTERN int send(const char* ns, message_t* msg);

/*
 * Receives a message_t sent to namespace ns of a sending process.
 * Returns 1 if succeeded else 0
 */
EXTERN int receive(const char* ns, message_t* msg);

#endif /* API_IPC_H_ */
