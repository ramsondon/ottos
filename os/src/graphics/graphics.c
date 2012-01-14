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

#include "graphics.h"

static RastPort defrp;
static BitMap fb;

/* can only be called once ... */
RastPort *graphics_init(char *fbaddr, int width, int height, int type) {
  fb.width = width;
  fb.height = height;
  fb.format = type;
  fb.stride = width * 2;
  fb.data = fbaddr;

  defrp.x = defrp.y = 0;
  defrp.point = fb.data;
  defrp.colour = 0x000000;
  defrp.font.romfont = (RomFont*) &graphics_font_misc_fixed;
  defrp.drawable.bitmap = &fb;

  return &defrp;
}

void graphics_set_color(RastPort *rp, unsigned int rgb) {
  // TODO: switch rp->drawable->format ...
  rp->colour = ((rgb & 0xf80000) >> 8) | ((rgb & 0xfc00) >> 5) | ((rgb & 0xf8) >> 3);
}

/*
void setColor(RastPort *rp, unsigned int r, unsigned int g, unsigned int b) {
  // TODO: switch rp->drawable->format ...
  rp->colour = ((rgb & 0xf80000) >> 8) | ((rgb & 0xfc00) >> 5) | ((rgb & 0xf8) >> 3);
}
*/

void graphics_draw_rect(RastPort *rp, int w, int h) {
  int i, j;
  unsigned int colour = rp->colour;
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

void graphics_move_to(RastPort *rp, int x, int y) {
  rp->x = x;
  rp->y = y;
  rp->point = ((unsigned char *) rp->drawable.bitmap->data) + x * 2
      + y * rp->drawable.bitmap->stride;
}

void graphics_draw_pixel(RastPort *rp) {
  *((unsigned short *) rp->point) = rp->colour;
}

void graphics_draw_char(RastPort *rp, unsigned int c, int scale) {
  int i, j, s;
  int w, h;
  unsigned short *outp;
  unsigned int colour = rp->colour;
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

  graphics_move_to(&defrp, curX, curY);

  for (l = 0; l < bmp_header->height; l++) {
    curX = x;
    for (w = 0; w < bmp_header->width; w++) {
      graphics_set_color(&defrp, (curPixel->Red << 6) | (curPixel->Green << 4) | (curPixel->Blue << 2));
      graphics_move_to(&defrp, curX, curY);
      graphics_draw_pixel(&defrp);
      curPixel++;
      curX++;
    }

    curY++;
  }
}


