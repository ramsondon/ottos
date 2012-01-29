/* io.c
 * 
 * Copyright (c) 2011 The ottos_api project.
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
 *  Created on: 13 Jan 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <string.h>
#include <api/system.h>
#include <api/io.h>

char* strrev(char* str) {
  char *p1, *p2;
  if (!str || !*str) return str;
  for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
    *p1 ^= *p2;
    *p2 ^= *p1;
    *p1 ^= *p2;
  }
  return str;
}

char* itoa(int n, char* s, int b) {
  static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  int i = 0, sign;
  if ((sign = n) < 0) {
    n = -n;
  }
  do {
    s[i++] = digits[n % b];
  } while ((n /= b) > 0);
  if (sign < 0) {
    s[i++] = '-';
  }
  s[i] = '\0';
  return strrev(s);
}

void print(const char* buffer) {
  int fd = sys_open(SYSTEM_SERIAL_0_PATH, SYSTEM_FLAG_WRITE);
  if (fd != SYSTEM_FD_INVALID) {
    sys_write(fd, buffer, strlen(buffer));
    // do not close serial 0!
    //sys_close(fd);
  }
}

int waiting() {
  int i = 0;
  for(i = 0; i < 10000; i++);
  return i;
}

size_t read_serial_with_end_char(char* buffer, size_t count, char end_character) {
  int fd = sys_open(SYSTEM_SERIAL_0_PATH, SYSTEM_FLAG_READ);
  char c = 0;
  int i = 0;

  do {
    while (0 == sys_read(fd, &c, 1)) {
      // wait for input
    }
    buffer[i] = c;
    i++;
  } while (i < count && c != end_character);

  return i;
}

size_t read_serial(char* buffer, size_t count) {
  return read_serial_with_end_char(buffer, count, '\n');
}
