/* drawer.h
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

#ifndef DRAWER_H_
#define DRAWER_H_

#include <ottos/types.h>
#include <ottos/const.h>
#include "video.h"

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

EXTERN RomFont const drawer_font_misc_fixed;

EXTERN void drawer_draw_pixel(RastPort *rp, unsigned int color, int x, int y);
EXTERN void drawer_draw_line(RastPort *rp, unsigned int color, int x, int y, int x_end, int y_end);
EXTERN void drawer_draw_rect(RastPort *rp, unsigned int color, int x, int y, int w, int h);
EXTERN void drawer_draw_ellipse(RastPort *rp, unsigned int color, int x, int y, int a, int b);
EXTERN void drawer_draw_string(RastPort *rp, unsigned int color, int x, int y, const char *s, int scale);

#endif /* DRAWER_H_ */
