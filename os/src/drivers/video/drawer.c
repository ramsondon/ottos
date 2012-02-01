/* drawer.c
 *
 * Copyright (c) 2011 The ottos_os project.
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
 *  Created on: 30.01.2012
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#include "drawer.h"

#include <math.h>
#include <stdio.h>
#include "video.h"

static void drawer_set_color(RastPort *rp, unsigned int rgb) {
  rp->color = ((rgb & 0xf80000) >> 8) | ((rgb & 0xfc00) >> 5) | ((rgb & 0xf8) >> 3);
}

static void drawer_move_to(RastPort *rp, int x, int y) {
  rp->x = x;
  rp->y = y;
  rp->point = ((unsigned char *) rp->drawable.bitmap->data) + x * 2
      + y * rp->drawable.bitmap->stride;
}

void drawer_draw_pixel(RastPort *rp, unsigned int color, int x, int y) {
  drawer_set_color(rp, color);
  drawer_move_to(rp, x, y);
  *((unsigned short *) rp->point) = rp->color;
}

void drawer_draw_rect(RastPort *rp, unsigned int color, int x, int y, int w, int h) {
  int i, j;
  unsigned short *outp = rp->point;

  drawer_move_to(rp, x, y);

  if (w + rp->x > rp->drawable.bitmap->width) {
    w = rp->drawable.bitmap->width - rp->x;
  }

  if (h + rp->y > rp->drawable.bitmap->height) {
    h = rp->drawable.bitmap->height - rp->y;
  }

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      outp[i] = color;
    }
    outp = (unsigned short *) ((char *) outp + rp->drawable.bitmap->stride);
  }
}

void drawer_draw_ellipse(RastPort *rp, unsigned int color, int x, int y, int a, int b) {
  int dx = 0, dy = b;
  long a2 = a * a, b2 = b * b;
  long err = b2 - (2 * b - 1) * a2, e2;

  do {
    drawer_draw_line(rp, color, x + dx, y + dy, x + dx, y - dy);
    drawer_draw_line(rp, color, x - dx, y + dy, x - dx, y - dy);

    e2 = 2 * err;
    if (e2 < ((2 * dx + 1) * b2)) {
      dx++;
      err += (2 * dx + 1) * b2;
    }
    if (e2 > (-(2 * dy - 1) * a2)) {
      dy--;
      err -= (2 * dy - 1) * a2;
    }
  } while (dy >= 0);

  while (dx++ < a) {
    drawer_draw_line(rp, color, x - dx, y, x + dx, y);
  }
}


// see: http://de.wikipedia.org/wiki/Bresenham-Algorithmus
void drawer_draw_line(RastPort *rp, unsigned int color, int x, int y, int x_end, int y_end) {
  int dx = abs(x_end - x);
  int dy = -abs(y_end - y);
  int sy = -1, sx = -1, e2;
  int err = dx + dy;

  // go to start point
  drawer_move_to(rp, x, y);
  drawer_set_color(rp, color);

  if (y < y_end) {
    sy = 1;
  }
  if (x < x_end) {
    sx = 1;
  }

  for(;;){
    drawer_move_to(rp, x, y);
    // set pixel
    *((unsigned short *) rp->point) = rp->color;
    if (x == x_end && y == y_end) {
      break;
    }

    e2 = 2*err;

    if (e2 > dy) {
      err += dy;
      x += sx;
    }

    if (e2 < dx) {
      err += dx;
      y += sy;
    }
  }
}

static void drawer_draw_char(RastPort *rp, unsigned int c, int scale) {
  int i, j, s;
  int w, h;
  unsigned short *outp;
  unsigned int colour = rp->color;
  unsigned const char *inp;

  // check if character is valid for the used font
  if (c < rp->font.romfont->first || c > rp->font.romfont->last) {
    return;
  }

  // get dimension of character
  w = rp->font.romfont->width;
  h = rp->font.romfont->height;
  c = (c - rp->font.romfont->first) * w;
  outp = ((unsigned short *)rp->point) - rp->font.romfont->baseline*scale*rp->drawable.bitmap->stride;
  inp = rp->font.romfont->bitmap + c;

  for (j=0;j<h;j++) {
    for (s=0;s<scale;s++)  {
      for (i=0;i<w*scale;i++) {
        unsigned int b = inp[i/scale];
        if (b) {
          outp[i] = colour;
        }
      }
      outp = (unsigned short *)((char *)outp + rp->drawable.bitmap->stride);
    }
    inp += rp->font.romfont->stride;
  }

  rp->point = ((unsigned short *)rp->point) + w*scale;
  rp->x += w*scale;
}

void drawer_draw_string(RastPort *rp, unsigned int color, int x, int y, const char *s, int scale) {
  unsigned int c;

  drawer_move_to(rp, x, y);

  while ((c = *s++)) {
    if (c == '\n') {
      if (y + 20 > rp->drawable.bitmap->height*scale) {
        drawer_move_to(rp, x, 20);
      } else {
        drawer_move_to(rp, x, y + rp->font.romfont->lineheight*scale);
      }
    } else {
      drawer_draw_char(rp, c, scale);
    }
  }
}

#define GRAPHICS_GRAPH_TEMP_MAX    40
#define GRAPHICS_GRAPH_TEMP_MIN   -12

void drawer_draw_graph(RastPort *rp, unsigned int color, int x0, int y0,
                       GRAPH_DATA* data, int length, int height, int width) {
  int i, y, value, prev_x, time_legend_index, time_legend_gap;
  unsigned int color_background = 0x00FF00;
  unsigned int color_line = 0x000000;
  int margin = 3;
  int span_in_pixel = width / length;
  GRAPH_DATA* cur_data;
  char str[8];
  float degree_in_pixel = height / (abs(GRAPHICS_GRAPH_TEMP_MAX) + abs(GRAPHICS_GRAPH_TEMP_MIN));


  // draw background rectangle
  drawer_draw_rect(rp, color_background, x0, y0, width, height);
  // draw vertical axis
  drawer_draw_line(rp, color_line, x0+margin, y0+margin, x0+margin, y0+height-margin);
  // draw horizontal axis
  drawer_draw_line(rp, color_line, x0+margin, y0+GRAPHICS_GRAPH_TEMP_MAX*degree_in_pixel,
                     x0+width-margin, y0+GRAPHICS_GRAPH_TEMP_MAX*degree_in_pixel);

  // draw dataline
  drawer_set_color(rp, color_line);
  x0 += margin + 1;
  y0 += GRAPHICS_GRAPH_TEMP_MAX*degree_in_pixel;

  for (i = 0; i < length; i++) {
    value = data[i].data;
    drawer_draw_line(rp, x0 + span_in_pixel * i, prev_x, x0 + span_in_pixel * (i + 1), y0 + value/degree_in_pixel, 1);
    prev_x = x0 + value/degree_in_pixel;
  }

  // write timevalues on time axis
  time_legend_index = length / 4;
  time_legend_gap = width / 4;

  // draw 4 time values on timeline
  cur_data = &(data[0]);
  sprintf(str, "%02d:%02d:%02d", cur_data->timestamp.hour, cur_data->timestamp.minute, cur_data->timestamp.second);
  drawer_draw_string(rp, color_line, x0+10, y0, str, 2);

  for (i = 1; i < 4; i++) {
    cur_data = &(data[time_legend_index * i - 1]);
    sprintf(str, "%02d:%02d:%02d", cur_data->timestamp.hour, cur_data->timestamp.minute, cur_data->timestamp.second);
    drawer_draw_string(rp, color_line, x0+time_legend_gap*i, y0, str, 2);
  }

  // draw legend for value axis
  drawer_draw_string(rp, color_line, x0 - 5, y0, "0", 2);

  y = y0 - (degree_in_pixel * GRAPHICS_GRAPH_TEMP_MAX);
  // draw positive values on value axis
  for (i = 1; i > 10; i++) {
    sprintf(str, "%d", i * 5);
    drawer_draw_string(rp, color_line, x0 - margin, y, str, 2);
  }

  // draw negative values on value axis
  for (i = 1; i < -4; i--) {
    sprintf(str, "%d", -i * 5);
    drawer_draw_string(rp, color_line, x0 - margin, y, str, 2);
  }
}

