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
  rp->point = ((unsigned char *) rp->bitmap->data) + x * 2
      + y * rp->bitmap->stride;
}

void drawer_draw_pixel(RastPort *rp, unsigned int color, int x, int y) {
  drawer_set_color(rp, color);
  drawer_move_to(rp, x, y);
  *((unsigned short *) rp->point) = rp->color;
}

void drawer_draw_rect(RastPort *rp, unsigned int color, int x, int y, int w, int h) {
  int i, j;
  unsigned short *outp;

  drawer_move_to(rp, x, y);
  drawer_set_color(rp, color);
  outp = rp->point;

  if (w + rp->x > rp->bitmap->width) {
    w = rp->bitmap->width - rp->x;
  }

  if (h + rp->y > rp->bitmap->height) {
    h = rp->bitmap->height - rp->y;
  }

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      outp[i] = rp->color;
    }
    outp = (unsigned short *) ((char *) outp + rp->bitmap->stride);
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
  unsigned const char *inp;

  // check if character is valid for the used font
  if (c < rp->romfont->first || c > rp->romfont->last) {
    return;
  }

  // get dimension of character
  w = rp->romfont->width;
  h = rp->romfont->height;
  c = (c - rp->romfont->first) * w;
  outp = ((unsigned short *)rp->point) - rp->romfont->baseline*scale*rp->bitmap->stride;
  inp = rp->romfont->bitmap + c;

  for (j=0;j<h;j++) {
    for (s=0;s<scale;s++)  {
      for (i=0;i<w*scale;i++) {
        unsigned int b = inp[i/scale];
        if (b) {
          outp[i] = rp->color;
        }
      }
      outp = (unsigned short *)((char *)outp + rp->bitmap->stride);
    }
    inp += rp->romfont->stride;
  }

  rp->point = ((unsigned short *)rp->point) + w*scale;
  rp->x += w*scale;
}

void drawer_draw_string(RastPort *rp, unsigned int color, int x, int y, const char *s, int scale) {
  unsigned int c;

  drawer_move_to(rp, x, y);
  drawer_set_color(rp, color);

  while ((c = *s++)) {
    if (c == '\n') {
      if (y + 20 > rp->bitmap->height*scale) {
        drawer_move_to(rp, x, 20);
      } else {
        drawer_move_to(rp, x, y + rp->romfont->lineheight*scale);
      }
    } else {
      drawer_draw_char(rp, c, scale);
    }
  }
}

void drawer_draw_triangle(RastPort *rp, unsigned int color, int x, int y, int xh, int yh) {
  int h, w, i = 0, len, cur_len;
  int x_start, y_start;

  if (x == xh) {
    // north or south arrow
    len = y - yh;
    len = abs(len);
    y_start = yh;
    while (len > 0) {
      cur_len = y > yh ? y_start+len : y_start-len;
      drawer_draw_line(rp, color, x-i, y_start, x-i, cur_len);
      drawer_draw_line(rp, color, x+i, y_start, x+i, cur_len);
      len--;
      i++;
    }
  } else if (y == yh) {
    // east or west arrow
    len = x - xh;
    len = abs(len);
    x_start = xh;
    while (len > 0) {
      cur_len = x > xh ? x_start+len : x_start-len;
      drawer_draw_line(rp, color, x_start, y-i, cur_len, y-i);
      drawer_draw_line(rp, color, x_start, y+i, cur_len, y+i);
      len--;
      i++;
    }
  } else {
    drawer_draw_string(rp, color, x, y, "baaam", 2);
  }
}
