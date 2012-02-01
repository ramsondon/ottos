/* video.h
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

#ifndef VIDEO_H_
#define VIDEO_H_

#include <ottos/types.h>
#include <ottos/const.h>
#include <ottos/drivers/driver.h>
#include <ottos/video_types.h>

// default resolution
#define VIDEO_RESOLUTION_HEIGHT   768
#define VIDEO_RESOLUTION_WIDTH    1024

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

#define VENC_MODE_NTSC (0)
#define VENC_MODE_PAL (1)


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


typedef struct {
  short width, height;
  char* name;
  uint32_t dispc_size;
  uint32_t dispc_timing_h;
  uint32_t dispc_timing_v;
  uint32_t dispc_divisor;
  uint32_t dss_divisor;
} VIDEO_DISP_MODE;

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


EXTERN RomFont const drawer_font_misc_fixed;


int video_open(device_t dev);
int video_close(device_t dev);
int video_read(device_t dev, int count, char* buffer);
int video_write(device_t dev, int count, char* buffer);
int video_ioctl(device_t dev, ioctl_t msg);
int video_create(device_t dev);


static driver_t omap_video_driver = {
  video_open,
  video_close,
  video_read,
  video_write,
  video_ioctl,
  video_create
};


#endif /* VIDEO_H_ */
