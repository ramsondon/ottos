/* ottos.cc
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
 *  Created on: Oct 19, 2011
 *      Author: Matthias Schmid <m.schmid@students.fhv.at>
 */

#include <ottos/system.h>
#include <ottos/kernel.h>
#include <ottos/const.h>
#include <ottos/timer.h>

#include "../../bin/led_test.h"
#include "../../bin/serial_test.h"
#include "../../bin/console.h"
#include "../../bin/uptime_test.h"
#include "../../bin/i2c_test.h"
#include "../drivers/i2c/i2c.h"

#include "kernel/intc/irq.h"
#include "kernel/pm/process.h"
#include "dev/devices.h"
#include "../hal/uart.h"

#include "../fs/fs.h"
#include "../fs/vfat/fat_filelib.h"
#include "../../drivers/mmchs/mmchs.h"

#include "../graphics/graphics.h"
#include "../drivers/display/disp.h"

void timer_test() {

  process_table_init();

  process_create(1, (int) toggle_led1);
  process_create(1, (int) toggle_led2);

  devices_init();

  irq_init();

  timer_init();
  //timer_add_handler(toggle_led_1, 5000);
  //timer_add_handler(toggle_led_2, 10000);

  irq_register_context_switch();

  irq_enable();
  kernel_to_user_mode();
}

void devices_test() {
  devices_init();
}

//void process_test() {
//
//
//  process_table_init();
//
//  process_create(1, (int)toggle_led1_yield);
//  process_create(1, (int)toggle_led2_yield);
//
//  devices_init();
//
//  // switch to user mode
//  kernel_to_user_mode();
//  sys_yield();
//
//}

void serial_test() {

  process_table_init();

  //    process_create(1, (int)serial_test_test_yield);
  //    process_create(1, (int)toggle_led1_yield);
  //    process_create(1, (int)toggle_led2_yield);


  //process_create(1, (int) led1_on);
  //process_create(1, (int) led1_off);
  process_create(1, (int) toggle_led1);
  process_create(1, (int) toggle_led2);
  //process_create(1, (int) serial_test_write_1);
  //process_create(1, (int) serial_test_write_2);
  //process_create(1, (int) serial_test_write_3);
  //process_create(1, (int) serial_test_write_4);
  //process_create(1, (int) serial_test_write_5);
  process_create(1, (int) serial_test_calculator);

  devices_init();
  serial_test_create();

  irq_init();

  timer_init();

  irq_register_context_switch();

  irq_enable();
  kernel_to_user_mode();
  //    sys_yield();

  //    serial_test_test();
}

void serial_test_calc() {

  process_table_init();
  process_create(1, (int) toggle_led1);
  process_create(1, (int) serial_test_calculator);

  devices_init();

  irq_init();

  timer_init();

  irq_register_context_switch();

  irq_enable();
  kernel_to_user_mode();
}

void process_exit_test() {

  process_table_init();
  process_create(1, (int) serial_test_write_exit_1);
  process_create(1, (int) serial_test_write_exit_2);
  process_create(1, (int) toggle_led1);

  devices_init();

  irq_init();
  timer_init();
  irq_register_context_switch();
  irq_enable();

  kernel_to_user_mode();
}

void dummy_process() {
  for (;;)
    ;
}

void console_test() {

  process_table_init();
  //process_create(1, (int) toggle_led1);
  //process_create(1, (int) toggle_led2);

  //process_create(1, (int) dummy_process);
  // process_create(1, (int) console_start);

  devices_init();

  irq_init();

  timer_init();

  irq_register_context_switch();

  irq_enable();
  kernel_to_user_mode();
}

void fs_test() {
  file_t* file;
  char buffer[512];
  //char text[] = { 't', 'e', 's', 't', '\n', '\r' };
  char text[] = "new file\n\r\0";

  devices_init();
  mmchs_init();
  fs_init();
  fl_listdirectory("/");

  file = (file_t*) fl_fopen("/test/thenewest.txt", "a");
  fl_fwrite(text, sizeof(text), sizeof(text), file);
  fl_fclose(file);

  fl_listdirectory("/test/");

  file = (file_t*) fl_fopen("/test/thenewest.txt", "r");
  fl_fread(buffer, 512, sizeof(text), file);
  kernel_print(buffer);
  fl_fclose(file);

}

void system_time_test() {
  process_table_init();

  process_create(1, (int) toggle_led1);
  process_create(1, (int) uptime_test);

  devices_init();
  irq_init();
  timer_init();
  irq_register_context_switch();

  irq_enable();
  kernel_to_user_mode();
}

void i2c_test() {
  devices_init();

  i2c_init();
  pulse_leds();
}

#define RES_WIDTH 1024
#define RES_HEIGHT 768
#define WIDTH  1024
#define HEIGHT 768
#define FBADDR ((void *)0x83000000)
void display_test() {
  int i = 0;
  int x, y, u;
  RastPort *rp;

  video_init(RES_WIDTH, RES_HEIGHT); // --> video.c (ganz unten)
  rp = graphics_init(FBADDR, RES_WIDTH, RES_HEIGHT, BM_RGB16); // graphics.c
  omap_attach_framebuffer(0, rp->drawable.bitmap); // --> video.c

  // also set it to the tv out (top-left corner of same data)
  //omap_attach_framebuffer(VID_VID2 | VID_TVOUT, rp->drawable.bitmap);

  moveTo(rp, 0, 0);
  setColour(rp, 0x3e31a2);
  drawRect(rp, WIDTH, HEIGHT);

  setColour(rp, 0x00ffff);
  moveTo(rp, 20, 20);
  drawString(rp, "Hello this weather station is powered by OttOS!",1);
  setColour(rp, 0xffff00);
  moveTo(rp, 80, 40);
  drawString(rp, "Hello this weather station is powered by OttOS!",2);
  setColour(rp, 0xff00ff);
  moveTo(rp, 120, 60);
  drawString(rp, "...:: OttOS ::...",3);

  setColour(rp, 0xffffff);

  x = 80;
  y = 200;
  u = 10;

  // write H
  moveTo(rp, x, y);
  drawRect(rp, u, 5*u);
  moveTo(rp, x+3*u, y);
  drawRect(rp, u, 5*u);
  moveTo(rp, x+u, y+2*u);
  drawRect(rp, 2*u, u);

  x += 5*u;
  // write E
  moveTo(rp, x, y);
  drawRect(rp, u, 5*u);
  moveTo(rp, x+u, y);
  drawRect(rp, 3*u, u);
  moveTo(rp, x+u, y+2*u);
  drawRect(rp, 2*u, u);
  moveTo(rp, x+u, y+4*u);
  drawRect(rp, 3*u, u);

  x += 5*u;
  // write L
  moveTo(rp, x, y);
  drawRect(rp, u, 5*u);
  moveTo(rp, x+u, y+4*u);
  drawRect(rp, 3*u, u);

  x += 5*u;
  // write L
  moveTo(rp, x, y);
  drawRect(rp, u, 5*u);
  moveTo(rp, x+u, y+4*u);
  drawRect(rp, 3*u, u);

  x += 5*u;
  // write O
  moveTo(rp, x, y);
  drawRect(rp, u, 5*u);
  moveTo(rp, x+3*u, y);
  drawRect(rp, u, 5*u);
  moveTo(rp, x+u, y);
  drawRect(rp, 2*u, u);
  moveTo(rp, x+u, y+4*u);
  drawRect(rp, 2*u, u);

  x += 5*u;
  // write !
  moveTo(rp, x, y);
  drawRect(rp, u, 3*u);
  moveTo(rp, x, y+4*u);
  drawRect(rp, u, u);

  return;
}


int main(int argc, char **argv) {

//  process_test();
//  timer_test();
//  serial_test();
//  serial_test_calc();
//  process_exit_test();
//  console_test();
//  fs_test();
//  i2c_test();
//  system_time_test();
//    uptime_test();
  display_test();

  for(;;);

  return 0;
}
