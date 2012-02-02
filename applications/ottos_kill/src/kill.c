/* kill.c
 * 
 * Copyright (c) 2011 The ottos_kill project.
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
 *  Created on: Feb 2, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

int main() {


  // get argument values (process name)
  int argc = 0;
  char **argv = sys_read_arguments(&argc);

  // check arguments
   if (argc < 2) {
     print("Invalid usage: time <process name> <args>\r\n");
     pexit(-1);
   }



}
