/* graphics.c
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
 *  Created on: 30.12.2011
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#include <api/graphics.h>
#include <api/colors.h>
#include <api/system.h>
#include <api/io.h>

#include <ottos/video_types.h>

#include <stdlib.h>


void graphics_draw_pixel(unsigned int rgb, int x, int y, BOOLEAN redraw) {
  GRAPHIC_ELEMENT* graphic = NULL;
  int fd = sys_open(SYSTEM_VIDEO_0_PATH, SYSTEM_FLAG_WRITE);
  if (fd != SYSTEM_FD_INVALID) {
    graphic = (GRAPHIC_ELEMENT*) malloc(sizeof(GRAPHIC_ELEMENT));

    // set type
    graphic->id = GRAPHIC_ELEMENT_PIXEL;

    // set arguments
    graphic->x = x;
    graphic->y = y;
    graphic->rgb_color = rgb;
    graphic->redraw = redraw;

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_ellipse(unsigned int rgb, int x, int y, int a, int b, BOOLEAN redraw) {
  GRAPHIC_ELEMENT* graphic = NULL;
  int fd = sys_open(SYSTEM_VIDEO_0_PATH, SYSTEM_FLAG_WRITE);
  if (fd != SYSTEM_FD_INVALID) {
    graphic = (GRAPHIC_ELEMENT*) malloc(sizeof(GRAPHIC_ELEMENT));

    // set type
    graphic->id = GRAPHIC_ELEMENT_ELLIPSE;

    // set arguments
    graphic->x = x;
    graphic->y = y;
    graphic->rgb_color = rgb;
    graphic->p1 = a;
    graphic->p2 = b;
    graphic->redraw = redraw;

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_rect(unsigned int rgb, int x, int y, int w, int h, BOOLEAN redraw) {
  GRAPHIC_ELEMENT* graphic = NULL;
  int fd = sys_open(SYSTEM_VIDEO_0_PATH, SYSTEM_FLAG_WRITE);
  if (fd != SYSTEM_FD_INVALID) {
    graphic = (GRAPHIC_ELEMENT*) malloc(sizeof(GRAPHIC_ELEMENT));

    // set type
    graphic->id = GRAPHIC_ELEMENT_RECTANGLE;

    // set arguments
    graphic->x = x;
    graphic->y = y;
    graphic->rgb_color = rgb;
    graphic->p1 = w;
    graphic->p2 = h;
    graphic->redraw = redraw;

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_line(unsigned int rgb, int x_start,
                        int y_start, int x_end, int y_end, BOOLEAN redraw) {
  GRAPHIC_ELEMENT* graphic = NULL;
  int fd = sys_open(SYSTEM_VIDEO_0_PATH, SYSTEM_FLAG_WRITE);
  if (fd != SYSTEM_FD_INVALID) {
    graphic = (GRAPHIC_ELEMENT*) malloc(sizeof(GRAPHIC_ELEMENT));

    // set type
    graphic->id = GRAPHIC_ELEMENT_LINE;

    // set arguments
    graphic->x = x_start;
    graphic->y = y_start;
    graphic->rgb_color = rgb;
    graphic->p1 = x_end;
    graphic->p2 = y_end;
    graphic->redraw = redraw;

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_string(unsigned int rgb, int x, int y, const char* s, int scale, BOOLEAN redraw) {
  GRAPHIC_ELEMENT* graphic = NULL;
  int fd = sys_open(SYSTEM_VIDEO_0_PATH, SYSTEM_FLAG_WRITE);
  if (fd != SYSTEM_FD_INVALID) {
    graphic = (GRAPHIC_ELEMENT*) malloc(sizeof(GRAPHIC_ELEMENT));

    // set type
    graphic->id = GRAPHIC_ELEMENT_STRING;

    // set arguments
    graphic->x = x;
    graphic->y = y;
    graphic->rgb_color = rgb;
    graphic->p1 = scale;
    graphic->text = (unsigned int)s;
    graphic->redraw = redraw;

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_arrow(unsigned int rgb, int x, int y, int width, int height, int direction, BOOLEAN redraw) {
  GRAPHIC_ELEMENT* graphic = NULL;
  int w, h, xr, yr, xt, yt;
  int fd = sys_open(SYSTEM_VIDEO_0_PATH, SYSTEM_FLAG_WRITE);
  if (fd != SYSTEM_FD_INVALID) {
    graphic = (GRAPHIC_ELEMENT*) malloc(sizeof(GRAPHIC_ELEMENT));

    switch (direction) {
    case GRAPHIC_ARROW_DIRECTION_NORTH:
      w = 60;
      h = 140;
      xr = x - 30;
      yr = y + 60;
      xt = x;
      yt = y + 60;
      break;
    case GRAPHIC_ARROW_DIRECTION_EAST:
      w = 140;
      h = 60;
      xr = x - 60 - w;
      yr = y - 30;
      xt = x - 60;
      yt = y;
      break;
    case GRAPHIC_ARROW_DIRECTION_WEST:
      w = 140;
      h = 60;
      xr = x + 60;
      yr = y - 30;
      xt = x + 60;
      yt = y;
      break;
    case GRAPHIC_ARROW_DIRECTION_SOUTH:
      w = 60;
      h = 140;
      xr = x - 30;
      yr = y - 60 - h;
      xt = x;
      yt = y - 60;
      break;
    }

    // set type
    graphic->id = GRAPHIC_ELEMENT_TRIANGLE;
    // set arguments
    graphic->x = x;
    graphic->y = y;
    graphic->rgb_color = rgb;
    graphic->p1 = xt;
    graphic->p2 = yt;

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));

    // set type
    graphic->id = GRAPHIC_ELEMENT_RECTANGLE;
    // set arguments
    graphic->x = xr;
    graphic->y = yr;
    graphic->rgb_color = rgb;
    graphic->p1 = w;
    graphic->p2 = h;
    graphic->redraw = redraw;

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));

    free(graphic);
  }
}
