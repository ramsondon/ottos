/* kernel.c
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
 *  Created on: 11.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include <ottos/kernel.h>
#include <ottos/io.h>

#include "../../drivers/serial/serial.h"


void kernel_panic(const char* str) {
  kernel_print("**** KERNEL PANIC ****\n\r");
  kernel_print(str);
  kernel_print("**** KERNEL PANIC ****\n\r");

  kernel_halt();
}

void kernel_error(ERROR_CODE code, const char* message) {
  char code_str[8];
  itoa(code, code_str, 10);

  kernel_print("ERROR [");
  kernel_print(code_str);
  kernel_print("]: ");
  kernel_print(message);
  kernel_print("\n\r");
}

void kernel_debug(ERROR_CODE code, const char* message) {
#ifdef DEBUG
  char code_str[8];
  itoa(code, code_str, 10);

  // TODO (fdomig@gmail.com) has to be refactored, to write into a log file.
  kernel_print("DEBUG [");
  kernel_print(code_str);
  kernel_print("]: ");
  kernel_print(message);
  kernel_print("\n\r");
#endif
}

void kernel_print(const char* str) {
//  // TODO(fdomig@gmail.com) Has to be refactored to use an own printf()
  serial_write((char*)str, strlen(str));
}

void kernel_halt() {
  for(;;);
}

void kernel_sleep(int ms) {
  int i;
  while (ms--) {
    // this is approximately 1ms on Beagleboard C4 without caching
    // @see http://code.google.com/p/puppybits/source/browse/lib/ksleep.c
    for (i=0; i<1000; i++) {
      asm("\t mov r0, r0 ;\n" \
      "\t mov r0, r0 ;\n" \
      "\t mov r0, r0");
    }
  }
}
