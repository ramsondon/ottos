/* string.c
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
 *  Created on: 24.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include <ottos/string.h>

size_t strlen(const char* str) {
  const char* s;
  s = str;
  while (*s) s++;
  return s - str;
}

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
  int i=0, sign;
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
