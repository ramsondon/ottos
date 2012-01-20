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

#include <ottos/memory.h>

// Master clock is 864 Mhz, and changing it is a pita since it cascades to other
// devices.
// Pixel clock is  864 / cm_clksel_dss.dss1_alwan_fclk / dispc_divisor.divisor.pcd
// So most of these modes are just approximate.
// List must be in ascending order.
DISP_VIDEO_MODE modes[] = {
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
  DISP_VIDEO_MODE *m;

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
void video_attach_framebuffer(int id, BitMap *bm) {
  uint32_t vheight, vwidth, dheight, dwidth, pos, gsize;
  uint32_t vsize, attr = 0;
  uint32_t rowinc = 1;
  uint32_t data0 = (uint32_t) bm->data;
  uint32_t data1 = data0;

  // FIXME: other formats than RGB16 taken from bitmap
  // FIXME: add a scale-to-fit bit for video overlays?

  /*
  if (id & VID_TVOUT) {
    vsize = Read32(DISPC_SIZE_DIG);
    gobit = DISPC_GODIGITAL | DISPC_DIGITALENABLE;
    attr = id > VID_TVOUT ? DISPC_VIDCHANNELOUT : DISPC_GFXCHANNELOUT;
    // turn off test pattern
    Set32(VENC_BASE, VENC_F_CONTROL, VENC_SVDS_MASK, VENC_SVDS_EXTERNAL);
    // data is interlaced
    rowinc += bm->stride;
    data0 += bm->stride;
  } else {
  */
    vsize = MMIO_READ32(DISPC_SIZE_LCD);
    //gobit = DISPC_GOLCD;
    attr = 0;
  //}

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


void video_init(int width, int height) {
  MMIO_WRITE32(DISPC_IRQENABLE, 0x00000);
  MMIO_WRITE32(DISPC_IRQSTATUS, 0x1ffff);

  // initialize the clocks
  video_clock_init();
  // initialize the display subsystem
  video_dss_init();

  // initialize the display controller
  video_dispc_init();

  video_set_lcd_mode(width, height);
}

