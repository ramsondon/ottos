/* disp.h
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

#ifndef DISP_H_
#define DISP_H_

#include <ottos/types.h>
#include <ottos/const.h>
#include "../../graphics/graphics.h"

#define VENC_MODE_NTSC (0)
#define VENC_MODE_PAL (1)

typedef struct {
  short width, height;
  char* name;
  uint32_t dispc_size;
  uint32_t dispc_timing_h;
  uint32_t dispc_timing_v;
  uint32_t dispc_divisor;
  uint32_t dss_divisor;
} DISP_VIDEO_MODE;

EXTERN void video_init(int width, int height);
EXTERN void omap_attach_framebuffer(int id, BitMap* bm);

#endif /* DISP_H_ */
