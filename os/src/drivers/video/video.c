/* video.c
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
 *  Created on: Dec 29, 2011
 *      Author: Florian Gopp <go.goflo@gmail.com>
 */

#include "../../hal/platform.h"
#include "video.h"
#include "drawer.h"
#include "../ottos/kernel/mmu/mmu.h"

#include <ottos/error.h>
#include <ottos/memory.h>

#include <stdlib.h>
#include <string.h>


static RastPort* video_rast_port = NULL;
static BitMap* video_bit_map = NULL;

// Master clock is 864 Mhz, and changing it is a pita since it cascades to other
// devices.
// Pixel clock is  864 / cm_clksel_dss.dss1_alwan_fclk / dispc_divisor.divisor.pcd
// So most of these modes are just approximate.
// List must be in ascending order.
static VIDEO_DISP_MODE modes[] = {
      // 640x480@72
       { 640, 480, "640x480-71", ((480 - 1) << 16) | (640 - 1),
         (128 << DISPCB_HBP) | (24 << DISPCB_HFP) | (40 << DISPCB_HSW),
         (28 << DISPCB_VBP) | (9 << DISPCB_VFP) | (3 << DISPCB_VSW), 2, 14 },

      // 800x480@60
       { 800, 480, "800x480-60", ((480 - 1) << 16) | (800 - 1),
         (88 << DISPCB_HBP) | (40 << DISPCB_HFP) | (128 << DISPCB_HSW),
         (28 << DISPCB_VBP) | (9 << DISPCB_VFP) | (3 << DISPCB_VSW), 2, 14 },

        //    800x600, 60Hz   40.000    800 40  128 88    600 1 4 23
       { 800, 600, "800x600-59", ((600 - 1) << 16) | (800 - 1),
         (88 << DISPCB_HBP) | (40 << DISPCB_HFP) | (128 << DISPCB_HSW),
          (23 << DISPCB_VBP) | (1 << DISPCB_VFP) | (4 << DISPCB_VSW), 2, 11 },

          // 1024x768, 60Hz   65.000    1024  24  136 160   768 3 6 29
       { 1024, 768, "1024x768-61", ((768 - 1) << 16) | (1024 - 1),
          (160 << DISPCB_HBP) | (24 << DISPCB_HFP) | (136 << DISPCB_HSW),
          (29 << DISPCB_VBP) | (3 << DISPCB_VFP) | (6 << DISPCB_VSW), 1, 13 },

      { 1280, 1024, "1280x1024-60", ((1024 - 1) << 16) | (1280 - 1),
         (248 << DISPCB_HBP) | (48 << DISPCB_HFP) | (112 << DISPCB_HSW),
         (38 << DISPCB_VBP) | (1 << DISPCB_VFP) | (3 << DISPCB_VSW), 1, 8 }
};


static void video_clock_init(void) {
  // sets pixel clock to 72MHz

  // sys_clk = 26.0 Mhz
  // DPLL4 = sys_clk * 432 / 13 = 864
  // DSS1_ALWON_FCLK = 864 / 6 = 144
  // Pixel clock (DISPC_DIVISOR) = 144 / 2 = 72Mhz
  // and also VENC clock = 864 / 16 = 54MHz

  // The clock multiplier/divider cannot be changed
  // without affecting other system clocks - do don't.

  // pll4 clock multiplier/divider
  MMIO_WRITE32(CM_CLKSEL2_PLL, (432 << 8) | 12);

  // tv clock divider, dss1 alwon fclk divider (S. 487)
  MMIO_WRITE32(CM_CLKSEL_DSS, (16 << 8) | 6);

  // core/peripheral PLL to 1MHz
  MMIO_WRITE32(CM_CLKEN_PLL, 0x00370037);
}


static void video_dss_init(void) {
  MMIO_WRITE32(DSS_SYSCONFIG, DSS_AUTOIDLE);

  // Select DSS1 ALWON as clock source
  MMIO_WRITE32(DSS_CONTROL, DSS_VENC_OUT_SEL | DSS_DAC_POWERDN_BGZ
      | DSS_DAC_DEMEN | DSS_VENC_CLOCK_4X_ENABLE);
}


static void video_dispc_init(void) {
  MMIO_WRITE32(DISPC_SYSCONFIG, DISPC_MIDLEMODE_SMART | DISPC_SIDLEMODE_SMART
      | DISPC_ENWAKEUP | DISPC_AUTOIDLE);

  MMIO_WRITE32(DISPC_CONFIG, DISPC_LOADMODE_FRAME);

  // LCD default = CBM blue
  MMIO_WRITE32(DISPC_DEFAULT_COLOR0, 0x7c70da);
  // digital default = black
  MMIO_WRITE32(DISPC_DEFAULT_COLOR1, 0x000000);

  MMIO_WRITE32(DISPC_TRANS_COLOR0, 0x000000);
  MMIO_WRITE32(DISPC_TRANS_COLOR1, 0x000000);

  MMIO_WRITE32(DISPC_POL_FREQ, DISPC_POL_IPC | DISPC_POL_IHS | DISPC_POL_IVS
      | (2 << DISPCB_POL_ACBI) | (8 << DISPCB_POL_ACB));

  // Set pixel clock divisor = 2
  MMIO_WRITE32(DISPC_DIVISOR, (1 << DISPCB_DIVISOR_LCD)
               | (2 << DISPCB_DIVISOR_PCD));

  // Disable graphical output
  MMIO_WRITE32(DISPC_GFX_ATTRIBUTES, 0);

  // Set palette / gamma table address
  //MMIO_WRITE32(DISPC_GFX_TABLE_BA, xx);

  // Set colour conversion coefficients for YUV modes
  // this is BT.601-5, see Table 15-47  S 15.5.3.3
  MMIO_WRITE32(DISPC_VID1_CONV_COEF0, 296 | (409 << 16));
  MMIO_WRITE32(DISPC_VID1_CONV_COEF1, 0 | (298 << 16));
  MMIO_WRITE32(DISPC_VID1_CONV_COEF2, ((-208) & 0xffff) | (-100 << 16));
  MMIO_WRITE32(DISPC_VID1_CONV_COEF3, 298 | (0 << 16));
  MMIO_WRITE32(DISPC_VID1_CONV_COEF4, 517);

  MMIO_WRITE32(DISPC_VID2_CONV_COEF0, 296 | (409 << 16));
  MMIO_WRITE32(DISPC_VID2_CONV_COEF1, 0 | (298 << 16));
  MMIO_WRITE32(DISPC_VID2_CONV_COEF2, ((-208) & 0xffff) | (-100 << 16));
  MMIO_WRITE32(DISPC_VID2_CONV_COEF3, 298 | (0 << 16));
  MMIO_WRITE32(DISPC_VID2_CONV_COEF4, 517);

  // Turn on the LCD data
  MMIO_WRITE32(DISPC_CONTROL, DISPC_GPOUT1 | DISPC_GPOUT0
      | DISPC_TFTDATALINES_24 | DISPC_STDITHERENABLE | DISPC_GOLCD
      | DISPC_STNTFT | DISPC_LCDENABLE);

  // apply current settings
  while ((MMIO_READ32(DISPC_CONTROL) & DISPC_GOLCD))
    ;
}


static void video_set_lcd_mode(int w, int h) {
  int i;
  VIDEO_DISP_MODE *m;

  for (i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
    if (w <= modes[i].width && h <= modes[i].height) {
      goto found;
    }
  }

  i -= 1;

found:
  m = &modes[i];

  MMIO_WRITE32(DISPC_SIZE_LCD, m->dispc_size);
  MMIO_WRITE32(DISPC_TIMING_H, m->dispc_timing_h);
  MMIO_WRITE32(DISPC_TIMING_V, m->dispc_timing_v);

  MMIO_AND_THEN_OR32(DISPC_DIVISOR, ~0xffff, m->dispc_divisor);
  MMIO_AND_THEN_OR32(CM_CLKSEL_DSS, ~0xffff, m->dss_divisor);

  // Tell hardware to update, and wait for it
  MMIO_AND_THEN_OR32(DISPC_CONTROL, ~DISPC_GOLCD, DISPC_GOLCD);

  while ((MMIO_READ32(DISPC_CONTROL) & DISPC_GOLCD))
    ;
}


// set a bitmap on the given video source
// id: 0 = gfx, 1 = vid1, 2 = vid2
// id: (1<<8) = set digital (tv) out
void video_attach_framebuffer(int id, BitMap* bm) {
  uint32_t vheight, vwidth, dheight, dwidth, pos, gsize;
  uint32_t vsize, attr = 0;
  uint32_t rowinc = 1;
  uint32_t data0 = (uint32_t) bm->data;
  uint32_t data1 = data0;

  // FIXME: other formats than RGB16 taken from bitmap
  // FIXME: add a scale-to-fit bit for video overlays?

  vsize = MMIO_READ32(DISPC_SIZE_LCD);

  // Centre graphics on display for now
  vheight = (vsize >> 16) + 1;
  vwidth = (vsize & 0xffff) + 1;
  dheight = bm->height;
  dwidth = bm->width;

  if (dheight > vheight) dheight = vheight;
  if (dwidth > vwidth) {
    // FIXME: adjust for data format width
    rowinc += bm->stride - vwidth * 2;
    dwidth = vwidth;
  }

  pos = (((vheight - dheight) / 2) << 16) | (vwidth - dwidth) / 2;
  gsize = ((dheight - 1) << 16) | (dwidth - 1);

  switch (id & 0x0f) {
    case 0:
      MMIO_WRITE32(DISPC_GFX_BA0, data0);
      MMIO_WRITE32(DISPC_GFX_BA1, data1);
      MMIO_WRITE32(DISPC_GFX_POSITION, pos);
      MMIO_WRITE32(DISPC_GFX_SIZE, gsize);
      MMIO_WRITE32(DISPC_GFX_FIFO_THRESHOLD, (0x3ff << 16) | 0x3c0);
      MMIO_WRITE32(DISPC_GFX_ROW_INC, rowinc);
      MMIO_WRITE32(DISPC_GFX_PIXEL_INC, 1);
      MMIO_WRITE32(DISPC_GFX_WINDOW_SKIP, 0);
      MMIO_WRITE32(DISPC_GFX_ATTRIBUTES, DISPC_GFXFORMAT_RGB16 | attr
          | DISPC_GFXBURSTSIZE_16x32 | DISPC_GFXENABLE);
      break;
    case 1:
      MMIO_WRITE32(DISPC_VID1_BA0, data0);
      MMIO_WRITE32(DISPC_VID1_BA1, data1);
      MMIO_WRITE32(DISPC_VID1_POSITION, pos);
      MMIO_WRITE32(DISPC_VID1_SIZE, gsize);
      MMIO_WRITE32(DISPC_VID1_PICTURE_SIZE, gsize);
      MMIO_WRITE32(DISPC_VID1_FIFO_THRESHOLD, (0x3ff << 16) | 0x3c0);
      MMIO_WRITE32(DISPC_VID1_ROW_INC, rowinc);
      MMIO_WRITE32(DISPC_VID1_PIXEL_INC, 1);
      MMIO_WRITE32(DISPC_VID1_ATTRIBUTES, DISPC_VIDFORMAT_UYVY | attr
                   | DISPC_VIDBURSTSIZE_16x32 | DISPC_VIDENABLE);
      break;
    case 2:
      MMIO_WRITE32(DISPC_VID2_BA0, data0);
      MMIO_WRITE32(DISPC_VID2_BA1, data1);
      MMIO_WRITE32(DISPC_VID2_POSITION, pos);
      MMIO_WRITE32(DISPC_VID2_SIZE, gsize);
      MMIO_WRITE32(DISPC_VID2_PICTURE_SIZE, gsize);
      MMIO_WRITE32(DISPC_VID2_FIFO_THRESHOLD, (0x3ff << 16) | 0x3c0);
      MMIO_WRITE32(DISPC_VID2_ROW_INC, rowinc);
      MMIO_WRITE32(DISPC_VID2_PIXEL_INC, 1);
      MMIO_WRITE32(DISPC_VID2_ATTRIBUTES, DISPC_VIDFORMAT_RGB16 | attr
          | DISPC_VIDBURSTSIZE_16x32 | DISPC_VIDENABLE);
      break;
  }

  // Tell hardware to update, and wait for it
  MMIO_AND_THEN_OR32(DISPC_CONTROL, ~DISPC_GOLCD, DISPC_GOLCD);

  while ((MMIO_READ32(DISPC_CONTROL) & DISPC_GOLCD))
    ;
}


int video_open(device_t dev) {
  if (video_rast_port != NULL) {
    // the video module is already initialized
    return DRIVER_NO_ERROR;
  }

  // initialize BitMap struct
  video_bit_map = (BitMap*)malloc(sizeof(BitMap));
  video_bit_map->width = VIDEO_RESOLUTION_WIDTH;
  video_bit_map->height = VIDEO_RESOLUTION_HEIGHT;
  video_bit_map->format = BM_RGB16;
  video_bit_map->stride = VIDEO_RESOLUTION_WIDTH * 2;
  video_bit_map->data = (void*)malloc(VIDEO_RESOLUTION_HEIGHT*VIDEO_RESOLUTION_WIDTH*4);

  // initialize RastPort struct
  video_rast_port = (RastPort*)malloc(sizeof(RastPort));
  video_rast_port->x = 0;
  video_rast_port->y = 0;
  video_rast_port->point = video_bit_map->data;
  video_rast_port->color = 0x00000000;
  video_rast_port->font.romfont = (RomFont*) &drawer_font_misc_fixed;
  video_rast_port->drawable.bitmap = video_bit_map;

  MMIO_WRITE32(DISPC_IRQENABLE, 0x00000);
  MMIO_WRITE32(DISPC_IRQSTATUS, 0x1ffff);

  // initialize the clocks
  video_clock_init();
  // initialize the display subsystem
  video_dss_init();

  // initialize the display controller
  video_dispc_init();

  video_set_lcd_mode(VIDEO_RESOLUTION_WIDTH, VIDEO_RESOLUTION_HEIGHT);


  // TODO (go.goflo@gmail.com) --> video_attach_framebuffer(0, bitmap)
  video_attach_framebuffer(0, video_bit_map);

  return DRIVER_NO_ERROR;
}


int video_close(device_t dev) {
  // free memory
  free(video_bit_map->data);
  free(video_bit_map);
  free(video_rast_port);
  video_bit_map = NULL;
  video_rast_port = NULL;

  return TRUE;
}

int video_write(device_t dev, int count, char* buffer) {
  GRAPHIC_ELEMENT* g = (GRAPHIC_ELEMENT*)buffer;

  switch(g->id) {
  case GRAPHIC_ELEMENT_PIXEL:
    drawer_draw_pixel(video_rast_port, g->rgb_color, g->x, g->y);
    break;
  case GRAPHIC_ELEMENT_LINE:
    drawer_draw_line(video_rast_port, g->rgb_color, g->x, g->y, g->p1, g->p2);
    break;
  case GRAPHIC_ELEMENT_RECTANGLE:
    drawer_draw_rect(video_rast_port, g->rgb_color, g->x, g->y, g->p1, g->p2);
    break;
  case GRAPHIC_ELEMENT_ELLIPSE:
    drawer_draw_ellipse(video_rast_port, g->rgb_color, g->x, g->y, g->p1, g->p2);
    break;
  case GRAPHIC_ELEMENT_STRING:
    drawer_draw_string(video_rast_port, g->rgb_color, g->x, g->y,
                      (char*)mmu_get_physical_address(process_table[process_active], g->text),
                      g->p1);
    break;
  default:
    // do nothing
    return DRIVER_ERROR_NOT_SUPPORTED;
  }

  return count;
}

int video_read(device_t dev, int count, char* buffer) {
  return DRIVER_ERROR_NOT_SUPPORTED;
}

int video_ioctl(device_t dev, ioctl_t msg) {
  return DRIVER_ERROR_NOT_SUPPORTED;
}

int video_create(device_t dev) {
  return DRIVER_ERROR_NOT_SUPPORTED;
}

