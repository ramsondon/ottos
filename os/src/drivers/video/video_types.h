/* video_types.h
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

#ifndef VIDEO_TYPES_H_
#define VIDEO_TYPES_H_

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
    short type;
    short x;
    short y;
    unsigned int rgb_color;
    short p1;
    short p2;
    char* text;
} GRAPHIC_ELEMENT;


#define GRAPHIC_ELEMENT_PIXEL      0
#define GRAPHIC_ELEMENT_LINE       1
#define GRAPHIC_ELEMENT_RECTANGLE  2
#define GRAPHIC_ELEMENT_ELLIPSE    3
#define GRAPHIC_ELEMENT_STRING     4


#endif /* VIDEO_TYPES_H_ */
