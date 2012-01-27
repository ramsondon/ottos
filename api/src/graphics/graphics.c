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
#include <math.h>

static RastPort* graphics_rastport;
static BitMap* graphics_framebuffer;

/* can only be called once ... */
RastPort* graphics_init(char* framebuffer, int width, int height, int type) {
  graphics_framebuffer->width = width;
  graphics_framebuffer->height = height;
  graphics_framebuffer->format = type;
  graphics_framebuffer->stride = width * 2;
  graphics_framebuffer->data = framebuffer;

  graphics_rastport->x = 0;
  graphics_rastport->y = 0;
  graphics_rastport->point = graphics_framebuffer->data;
  graphics_rastport->color = COLOR_Black;
  graphics_rastport->font.romfont = (RomFont*) &graphics_font_misc_fixed;
  graphics_rastport->drawable.bitmap = graphics_framebuffer;

  return graphics_rastport;
}

void graphics_redraw(RastPort* rp) {
  int fd = sys_open(SYSTEM_VIDEO_0_PATH, SYSTEM_FLAG_WRITE);
  if (fd != SYSTEM_FD_INVALID && rp != NULL) {
    sys_write(fd, (char*)rp->drawable.bitmap->data, rp->drawable.bitmap->height*rp->drawable.bitmap->width*4);
    //sys_close(fd);
  }
}

void graphics_set_color(RastPort *rp, unsigned int rgb) {
  rp->color = ((rgb & 0xf80000) >> 8) | ((rgb & 0xfc00) >> 5) | ((rgb & 0xf8) >> 3);
}

void graphics_draw_rect(RastPort *rp, int w, int h) {
  int i, j;
  unsigned int colour = rp->color;
  unsigned short *outp = rp->point;

  if (w + rp->x > rp->drawable.bitmap->width) {
    w = rp->drawable.bitmap->width - rp->x;
  }

  if (h + rp->y > rp->drawable.bitmap->height) {
    h = rp->drawable.bitmap->height - rp->y;
  }

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      outp[i] = colour;
    }
    outp = (unsigned short *) ((char *) outp + rp->drawable.bitmap->stride);
  }
}

static void graphics_set_pixel(RastPort* rp, int x, int y) {
  graphics_move_to(rp, x, y);
  graphics_draw_pixel(rp);
}

void graphics_draw_ellipse(RastPort* rp, int xm, int ym, int a, int b) {
  int dx = 0, dy = b;
  long a2 = a * a, b2 = b * b;
  long err = b2 - (2 * b - 1) * a2, e2;

  do {
    graphics_draw_line(rp, xm + dx, ym + dy, xm + dx, ym - dy, 1);
    graphics_draw_line(rp, xm - dx, ym + dy, xm - dx, ym - dy, 1);

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
    graphics_draw_line(rp, xm - dx, ym, xm + dx, ym, 1);
  }
}


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

// see: http://de.wikipedia.org/wiki/Bresenham-Algorithmus
void graphics_draw_line(RastPort* rp, int x_start, int y_start, int x_end, int y_end, int width) {
  int dx = abs(x_end - x_start);
  int dy = -abs(y_end - y_start);
  int sy = -1, sx = -1, e2;
  int err = dx + dy;

  // go to start point
  graphics_move_to(rp, x_start, y_start);

  if (y_start < y_end) {
    sy = 1;
  }
  if (x_start < x_end) {
    sx = 1;
  }

  for(;;){
    graphics_move_to(rp, x_start, y_start);
    graphics_draw_pixel(rp);
    if (x_start == x_end && y_start == y_end) {
      break;
    }

    e2 = 2*err;

    if (e2 > dy) {
      err += dy;
      x_start += sx;
    }

    if (e2 < dx) {
      err += dx;
      y_start += sy;
    }
  }
}

void graphics_move_to(RastPort *rp, int x, int y) {
  rp->x = x;
  rp->y = y;
  rp->point = ((unsigned char *) rp->drawable.bitmap->data) + x * 2
      + y * rp->drawable.bitmap->stride;
}

void graphics_draw_pixel(RastPort *rp) {
  *((unsigned short *) rp->point) = rp->color;
}

void graphics_draw_char(RastPort *rp, unsigned int c, int scale) {
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

void graphics_draw_string(RastPort *rp, const char *s, int scale) {
  unsigned int c;
  unsigned int x = rp->x;

  while ((c = *s++)) {
    if (c == '\n') {
      if (rp->y + 20 > rp->drawable.bitmap->height*scale) {
        graphics_move_to(rp, x, 20);
      } else {
        graphics_move_to(rp, x, rp->y + rp->font.romfont->lineheight*scale);
      }
    } else {
      graphics_draw_char(rp, c, scale);
    }
  }
}

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

