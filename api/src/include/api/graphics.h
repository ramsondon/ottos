/* graphics.h
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

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <ottos/types.h>
#include <ottos/const.h>
#include <api/bitmap.h>

typedef struct {
  const char* name;
  int width;
  int height;
  int stride;
  int first;
  int last;
  int baseline;
  int lineheight;
  unsigned const char* bitmap;
} RomFont;

typedef struct {
  int width;
  int height;
  int format;
  int stride;
  void* data;
} BitMap;

/* NB: these mirror the omap modes */
#define BM_BITMAP1    0
#define BM_BITMAP2    1
#define BM_BITMAP4    2
#define BM_BITMAP8    3
#define BM_RGB16      6
#define BM_RGB23      9
#define BM_ARGB32     12
#define BM_YUV2       10
#define BM_UYVY       11

#define RESOLUTION_HEIGHT   768
#define RESOLUTION_WIDTH    1024

// correct the height of the ellipse --> this is needed because we use a 16:9 display but a 4:3 resolution
#define GRAPHICS_RESOLUTION_CORRECTION_FACTOR   1.333

typedef struct {
  int x;                /* width position of the current pixel on the screen */
  int y;                /* height position of the current pixel on the screen */
  void* point;          /* pointer to the memory where the current pixel resides */
  unsigned int color;  /* current color which is used to draw pixels */

  union {
    RomFont* romfont;
  } font;               /* default font */

  union {
    BitMap* bitmap;
  } drawable;           /* the region into which the data are written */
} RastPort;


typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    union {
      unsigned short hour;
      unsigned short minute;
      unsigned short second;
    } timestamp;

    int data;
} GRAPH_DATA;

#define GRAPHICS_GRAPH_TEMP_MAX    40
#define GRAPHICS_GRAPH_TEMP_MIN   -12


EXTERN RomFont const graphics_font_misc_fixed;

/* can only be called once ... */
EXTERN RastPort* graphics_init(char* framebuffer, int width, int height, int type);

EXTERN void graphics_set_color(RastPort* rp, unsigned int rgb);
EXTERN void graphics_move_to(RastPort* rp, int x, int y);
EXTERN void graphics_draw_pixel(RastPort* rp);
EXTERN void graphics_draw_ellipse(RastPort* rp, int xm, int ym, int a, int b);
EXTERN void graphics_draw_rect(RastPort* rp, int w, int h);
EXTERN void graphics_draw_line(RastPort* rp, int x_start, int y_start, int x_end, int y_end, int width);
EXTERN void graphics_draw_char(RastPort* rp, unsigned int c, int scale);
EXTERN void graphics_draw_string(RastPort* rp, const char* s, int scale);
EXTERN void graphics_draw_picture(int x, int y, BITMAP_HEADER* bmp_header, RGBA *data);
EXTERN void graphics_redraw(RastPort* rp);
EXTERN void graphics_draw_graph(RastPort *rp, GRAPH_DATA* data, int length, int timespan, int height, int width,
                           unsigned int color_line, unsigned int color_background);

#endif /* GRAPHICS_H_ */
