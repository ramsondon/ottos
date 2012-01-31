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


#define RESOLUTION_HEIGHT   768
#define RESOLUTION_WIDTH    1024

// correct the height of the ellipse --> this is needed because we use a 16:9 display but a 4:3 resolution
#define GRAPHICS_RESOLUTION_CORRECTION_FACTOR   1.333

#define GRAPHICS_GRAPH_TEMP_MAX    40
#define GRAPHICS_GRAPH_TEMP_MIN   -12

EXTERN void graphics_draw_pixel(unsigned int rgb, int x, int y);
EXTERN void graphics_draw_ellipse(unsigned int rgb, int x, int y, int a, int b);
EXTERN void graphics_draw_rect(unsigned int rgb, int x, int y, int w, int h);
EXTERN void graphics_draw_line(unsigned int rgb, int x, int y, int x_end, int y_end, int width);
EXTERN void graphics_draw_string(unsigned int rgb, int x, int y, const char* s, int scale);

//EXTERN void graphics_draw_graph(GRAPH_DATA* data, int length, int timespan, int height, int width,
//                           unsigned int color_line, unsigned int color_background);

#endif /* GRAPHICS_H_ */
