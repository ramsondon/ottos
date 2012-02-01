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


void graphics_draw_pixel(unsigned int rgb, int x, int y) {
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

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_ellipse(unsigned int rgb, int x, int y, int a, int b) {
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

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_rect(unsigned int rgb, int x, int y, int w, int h) {
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

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_line(unsigned int rgb, int x_start,
                        int y_start, int x_end, int y_end, int width) {
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

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}

void graphics_draw_string(unsigned int rgb, int x, int y, const char* s, int scale) {
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

    sys_write(fd, (char*) graphic, sizeof(GRAPHIC_ELEMENT));
    //sys_close(fd);
    free(graphic);
  }
}



/*
void graphics_draw_graph(RastPort *rp, GRAPH_DATA* data, int length, int timespan, int height, int width,
                         unsigned int color_line, unsigned int color_background) {
  int i, y, value, prev_x, time_legend_index, time_legend_gap;
  int x0 = rp->x;
  int y0 = rp->y;
  int margin = 3;
  int span_in_pixel = width / length;
  GRAPH_DATA* cur_data;
  char str[8];
  float degree_in_pixel = height / (abs(GRAPHICS_GRAPH_TEMP_MAX) + abs(GRAPHICS_GRAPH_TEMP_MIN));


  // draw background rectangle
  graphics_set_color(rp, color_background);
  graphics_draw_rect(rp, width, height);
  // draw vertical axis
  graphics_set_color(rp, COLOR_Black);
  graphics_draw_line(rp, x0+margin, y0+margin, x0+margin, y0+height-margin, 1);
  // draw horizontal axis
  graphics_draw_line(rp, x0+margin, y0+GRAPHICS_GRAPH_TEMP_MAX*degree_in_pixel,
                     x0+width-margin, y0+GRAPHICS_GRAPH_TEMP_MAX*degree_in_pixel, 1);

  // draw dataline
  graphics_set_color(rp, color_line);
  x0 += margin + 1;
  y0 += GRAPHICS_GRAPH_TEMP_MAX*degree_in_pixel;

  for (i = 0; i < length; i++) {
    value = data[i].data;
    graphics_draw_line(rp, x0 + span_in_pixel * i, prev_x, x0 + span_in_pixel * (i + 1), y0 + value/degree_in_pixel, 1);
    prev_x = x0 + value/degree_in_pixel;
  }

  // write timevalues on time axis
  time_legend_index = length / 4;
  time_legend_gap = width / 4;

  // draw 4 time values on timeline
  cur_data = &(data[0]);
  graphics_set_color(rp, COLOR_Black);
  graphics_move_to(rp, x0 + 10, y0);
  sprintf(str, "%02d:%02d:%02d", cur_data->timestamp.hour, cur_data->timestamp.minute, cur_data->timestamp.second);
  graphics_draw_string(rp, str, 2);

  for (i = 1; i < 4; i++) {
    cur_data = &(data[time_legend_index * i - 1]);
    graphics_set_color(rp, COLOR_Black);
    graphics_move_to(rp, x0 + time_legend_gap*i, y0);
    sprintf(str, "%02d:%02d:%02d", cur_data->timestamp.hour, cur_data->timestamp.minute, cur_data->timestamp.second);
    graphics_draw_string(rp, str, 2);
  }

  // draw legend for value axis
  graphics_move_to(rp, x0 - 5, y0);
  graphics_draw_string(rp, "0", 2);

  y = y0 - (degree_in_pixel * GRAPHICS_GRAPH_TEMP_MAX);
  // draw positive values on value axis
  for (i = 1; i > 10; i++) {
    sprintf(str, "%d\0", i * 5);
    graphics_move_to(rp, x0 - margin, y);
    graphics_draw_string(rp, str, 2);
  }

  // draw negative values on value axis
  for (i = 1; i < -4; i--) {
    sprintf(str, "%d\0", -i * 5);
    graphics_move_to(rp, x0 - margin, y);
    graphics_draw_string(rp, str, 2);
  }
}
*/



/*
void graphics_draw_picture(int x, int y, BITMAP_HEADER* bmp_header, RGBA* data) {
  int w, l;
  int curX = x;
  int curY = y;
  RGBA* curPixel = data;

  graphics_move_to(graphics_rastport, curX, curY);

  for (l = 0; l < bmp_header->height; l++) {
    curX = x;
    for (w = 0; w < bmp_header->width; w++) {
      graphics_set_color(graphics_rastport, (curPixel->Red << 6) | (curPixel->Green << 4) | (curPixel->Blue << 2));
      graphics_move_to(graphics_rastport, curX, curY);
      graphics_draw_pixel(graphics_rastport);
      curPixel++;
      curX++;
    }

    curY++;
  }
}
*/

