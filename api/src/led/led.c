/* io.h
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
 *  Created on: 14 Jan 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <ottos_api/system.h>
#include <ottos_api/led.h>

static const char led_on_buffer[] = { 1 };
static const char led_off_buffer[] = { 0 };

#define LED_ON led_on_buffer
#define LED_OFF led_off_buffer

const char* led_get_path(led_t led) {
	switch (led) {
	case LED_1:
		return SYSTEM_LED_0_PATH;
	case LED_2:
		return SYSTEM_LED_1_PATH;
	}

	return SYSTEM_LED_0_PATH;
}

void led_on(led_t led) {
	int fd = sys_open(led_get_path(led), SYSTEM_FLAG_WRITE);
	if (fd != SYSTEM_FD_INVALID) {
		sys_write(fd, LED_ON, 1);
		sys_close(fd);
	}
}

void led_off(led_t led) {
	int fd = sys_open(led_get_path(led), SYSTEM_FLAG_WRITE);
	if (fd != SYSTEM_FD_INVALID) {
		sys_write(fd, LED_OFF, 1);
		sys_close(fd);
	}
}

void led_toggle(led_t led) {
	int fd = sys_open(led_get_path(led), SYSTEM_FLAG_WRITE);
	if (fd != SYSTEM_FD_INVALID) {
		char buffer[1];

		sys_read(fd, buffer, 1);
		if (buffer[0] == led_on_buffer[0]) {
			sys_write(fd, LED_OFF, 1);
		} else {
			sys_write(fd, LED_ON, 1);
		}
		sys_close(fd);
	}
}
