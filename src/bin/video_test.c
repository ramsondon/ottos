/* video_test.c
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
 *  Created on: 11.01.2012
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#include "video_test.h"

#include "../drivers/video/video.h"
#include "../graphics/graphics.h"
#include "../graphics/bitmap.h"

#include "../fs/vfat/fat_filelib.h"
//#include "../fs/fs.h"

#include <stdlib.h>


#define SUN_16_SIZE     2166

void video_bmp_test() {
  // filesystem & fat stuff
  char* buffer = malloc(SUN_16_SIZE);
  void* framebuffer = malloc(3000000);
  void* file;
  RastPort *rp;

  // bitmap stuff
  RGBA* bmp;
  BITMAP_FILEHEADER* bmpFileHeader = (BITMAP_FILEHEADER*)malloc(sizeof(BITMAP_FILEHEADER));
  BITMAP_HEADER* bmpHeader = (BITMAP_HEADER*)malloc(sizeof(BITMAP_HEADER));

  // load bmp file: sun.bmp | katze.bmp | katze_.bmp
  fs_init();
  file = fl_fopen("/sun.bmp", "r");
  fl_fread(buffer, SUN_16_SIZE, SUN_16_SIZE, file);

  // init video
  video_init(RES_WIDTH, RES_HEIGHT);
  rp = graphics_init(framebuffer, RES_WIDTH, RES_HEIGHT, BM_RGB16);
  video_attach_framebuffer(0, rp->drawable.bitmap);

  graphics_move_to(rp, 0, 0);
  graphics_set_color(rp, 0x3e31a2);
  graphics_draw_rect(rp, WIDTH, HEIGHT);

  bmp = graphics_parse_bmp_picture(buffer, SUN_16_SIZE, bmpFileHeader, bmpHeader);
  graphics_draw_picture(200, 200, bmpHeader, bmp);
}


void video_test() {
  int i = 0;
  int x, y, u;
  RastPort *rp;
  void* framebuffer = malloc(3000000);

  // init
  video_init(RES_WIDTH, RES_HEIGHT);
  rp = graphics_init(framebuffer, RES_WIDTH, RES_HEIGHT, BM_RGB16);
  video_attach_framebuffer(0, rp->drawable.bitmap);

  // also set it to the tv out (top-left corner of same data)
  //video_attach_framebuffer(VID_VID2 | VID_TVOUT, rp->drawable.bitmap);

  graphics_move_to(rp, 0, 0);
  graphics_set_color(rp, 0x3e31a2);
  graphics_draw_rect(rp, WIDTH, HEIGHT);

  graphics_set_color(rp, 0x00ffff);
  graphics_move_to(rp, 20, 20);
  graphics_draw_string(rp, "Hello this weather station is powered by OttOS!",1);
  graphics_set_color(rp, 0xffff00);
  graphics_move_to(rp, 80, 40);
  graphics_draw_string(rp, "Hello this weather station is powered by OttOS!",3);
  graphics_set_color(rp, 0xff00ff);
  graphics_move_to(rp, 500, 300);
  graphics_draw_string(rp, "...:: OttOS ::...",4);

  // draw line
  graphics_draw_line(rp, 300, 300, 400, 600, 1);
  graphics_draw_line(rp, 600, 300, 400, 100, 1);
  graphics_draw_line(rp, 400, 300, 200, 600, 1);

  /*
  // write 'HELLO' with rectangles
  x = 80;
  y = 200;
  u = 10;
  graphics_set_color(rp, 0xffffff);

  // write H
  graphics_move_to(rp, x, y);
  graphics_draw_rect(rp, u, 5*u);
  graphics_move_to(rp, x+3*u, y);
  graphics_draw_rect(rp, u, 5*u);
  graphics_move_to(rp, x+u, y+2*u);
  graphics_draw_rect(rp, 2*u, u);

  x += 5*u;
  // write E
  graphics_move_to(rp, x, y);
  graphics_draw_rect(rp, u, 5*u);
  graphics_move_to(rp, x+u, y);
  graphics_draw_rect(rp, 3*u, u);
  graphics_move_to(rp, x+u, y+2*u);
  graphics_draw_rect(rp, 2*u, u);
  graphics_move_to(rp, x+u, y+4*u);
  graphics_draw_rect(rp, 3*u, u);

  x += 5*u;
  // write L
  graphics_move_to(rp, x, y);
  graphics_draw_rect(rp, u, 5*u);
  graphics_move_to(rp, x+u, y+4*u);
  graphics_draw_rect(rp, 3*u, u);

  x += 5*u;
  // write L
  graphics_move_to(rp, x, y);
  graphics_draw_rect(rp, u, 5*u);
  graphics_move_to(rp, x+u, y+4*u);
  graphics_draw_rect(rp, 3*u, u);

  x += 5*u;
  // write O
  graphics_move_to(rp, x, y);
  graphics_draw_rect(rp, u, 5*u);
  graphics_move_to(rp, x+3*u, y);
  graphics_draw_rect(rp, u, 5*u);
  graphics_move_to(rp, x+u, y);
  graphics_draw_rect(rp, 2*u, u);
  graphics_move_to(rp, x+u, y+4*u);
  graphics_draw_rect(rp, 2*u, u);

  x += 5*u;
  // write !
  graphics_move_to(rp, x, y);
  graphics_draw_rect(rp, u, 3*u);
  graphics_move_to(rp, x, y+4*u);
  graphics_draw_rect(rp, u, u);
  */

  return;
}
