/* dispc.h
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
 *  Created on: Dec 15, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef HAL_OMAP353X_DISPC_H_
#define HAL_OMAP353X_DISPC_H_

/* Clock register addresses */
#define CM_CLOCK_BASE     0x48004D00
#define CM_CLKEN_PLL      (CM_CLOCK_BASE + 0x0)
#define CM_CLKSEL2_PLL    (CM_CLOCK_BASE + 0x44)

#define CM_DSS_BASE       0x48004E00
#define CM_CLKSEL_DSS     (CM_DSS_BASE + 0x40)


/* Display controller base address */
#define DISPC_BASE              0x48050400

#define DISPC_SYSCONFIG         (DISPC_BASE + 0x10)
#define DISPC_SYSSTATUS         (DISPC_BASE + 0x14)
#define DISPC_IRQSTATUS         (DISPC_BASE + 0x18)
#define DISPC_IRQENABLE         (DISPC_BASE + 0x1C)
#define DISPC_CONTROL           (DISPC_BASE + 0x40)
#define DISPC_CONFIG            (DISPC_BASE + 0x44)
#define DISPC_DEFAULT_COLOR0    (DISPC_BASE + 0x4c)
#define DISPC_DEFAULT_COLOR1    (DISPC_BASE + 0x50)
#define DISPC_TRANS_COLOR0      (DISPC_BASE + 0x54)
#define DISPC_TRANS_COLOR1      (DISPC_BASE + 0x58)
#define DISPC_LINE_STATUS       (DISPC_BASE + 0x5C)
#define DISPC_LINE_NUMBER       (DISPC_BASE + 0x60)
#define DISPC_TIMING_H          (DISPC_BASE + 0x64)
#define DISPC_TIMING_V          (DISPC_BASE + 0x68)
#define DISPC_POL_FREQ          (DISPC_BASE + 0x6C)
#define DISPC_DIVISOR           (DISPC_BASE + 0x70)
#define DISPC_GLOBAL_ALPHA      (DISPC_BASE + 0x74)
#define DISPC_SIZE_DIG          (DISPC_BASE + 0x78)
#define DISPC_SIZE_LCD          (DISPC_BASE + 0x7C)

#define DISPC_GFX_BA0               (DISPC_BASE + 0x80)
#define DISPC_GFX_BA1               (DISPC_BASE + 0x84)
#define DISPC_GFX_POSITION          (DISPC_BASE + 0x88)
#define DISPC_GFX_SIZE              (DISPC_BASE + 0x8C)
#define DISPC_GFX_ATTRIBUTES        (DISPC_BASE + 0xA0)
#define DISPC_GFX_FIFO_THRESHOLD    (DISPC_BASE + 0xA4)
#define DISPC_GFX_FIFO_SIZE_STATUS  (DISPC_BASE + 0xA8)
#define DISPC_GFX_ROW_INC           (DISPC_BASE + 0xAC)
#define DISPC_GFX_PIXEL_INC         (DISPC_BASE + 0xB0)
#define DISPC_GFX_WINDOW_SKIP       (DISPC_BASE + 0xB4)
#define DISPC_GFX_TABLE_BA          (DISPC_BASE + 0xB8)

#define DISPC_VID1_BA0       (DISPC_BASE + 0x0BC)
#define DISPC_VID1_BA1       (DISPC_BASE + 0x0C0)
#define DISPC_VID2_BA0       (DISPC_BASE + 0x14C)
#define DISPC_VID2_BA1       (DISPC_BASE + 0x150)

#define DISPC_VID1_POSITION       (DISPC_BASE + 0xC4)
#define DISPC_VID2_POSITION       (DISPC_VID1_POSITION + 0x90)
#define DISPC_VID1_SIZE           (DISPC_BASE + 0xC8)
#define DISPC_VID2_SIZE           (DISPC_VID1_SIZE + 0x90)
#define DISPC_VID1_ATTRIBUTES     (DISPC_BASE + 0xCC)
#define DISPC_VID2_ATTRIBUTES     (DISPC_VID1_ATTRIBUTES + 0x90)

#define DISPC_VID1_FIFO_THRESHOLD     (DISPC_BASE + 0xD0)
#define DISPC_VID2_FIFO_THRESHOLD     (DISPC_VID1_FIFO_THRESHOLD + 0x90)
#define DISPC_VID1_FIFO_SIZE_STATUS   (DISPC_BASE + 0xD4)
#define DISPC_VID2_FIFO_SIZE_STATUS   (DISPC_VID1_FIFO_SIZE_STATUS + 0x90)
#define DISPC_VID1_ROW_INC            (DISPC_BASE + 0xD8)
#define DISPC_VID2_ROW_INC            (DISPC_VID1_ROW_INC + 0x90)
#define DISPC_VID1_PIXEL_INC          (DISPC_BASE + 0xDC)
#define DISPC_VID2_PIXEL_INC          (DISPC_VID1_PIXEL_INC + 0x90)
/*
#define DISPC_VIDn_FIR 0x0E0+((n-1)*
*/
#define DISPC_VID1_PICTURE_SIZE (DISPC_BASE + 0x0E4)
#define DISPC_VID2_PICTURE_SIZE (0x0E4+0x90)
/*
#define DISPC_VIDn_ACCUl 0x0E8
#define DISPC_VIDn_FIR_COEF_Hi 0x0F0+
#define DISPC_VIDn_FIR_COEF_HVi 0x0F4+
*/
#define DISPC_VID1_CONV_COEF0 (DISPC_BASE + 0x130)
#define DISPC_VID2_CONV_COEF0 (DISPC_VID1_CONV_COEF0 + 0x90)
#define DISPC_VID1_CONV_COEF1 (DISPC_BASE + 0x134)
#define DISPC_VID2_CONV_COEF1 (DISPC_VID1_CONV_COEF1 + 0x90)
#define DISPC_VID1_CONV_COEF2 (DISPC_BASE + 0x138)
#define DISPC_VID2_CONV_COEF2 (DISPC_VID1_CONV_COEF2 + 0x90)
#define DISPC_VID1_CONV_COEF3 (DISPC_BASE + 0x13C)
#define DISPC_VID2_CONV_COEF3 (DISPC_VID1_CONV_COEF3 + 0x90)
#define DISPC_VID1_CONV_COEF4 (DISPC_BASE + 0x140)
#define DISPC_VID2_CONV_COEF4 (DISPC_VID1_CONV_COEF4 + 0x90)
/*
#define DISPC_DATA_CYCLEk 0x1D4+((k-1)*
#define DISPC_VIDn_FIR_COEF_Vi 0x1E0+
*/
#define DISPC_CPR_COEF_R (DISPC_BASE + 0x220)
#define DISPC_CPR_COEF_G (DISPC_BASE + 0x224)
#define DISPC_CPR_COEF_B (DISPC_BASE + 0x228)
#define DISPC_GFX_PRELOAD (DISPC_BASE + 0x22C)
#define DISPC_VID1_PRELOAD (DISPC_BASE + 0x230)
#define DISPC_VID2_PRELOAD (DISPC_BASE + 0x234)

/* DISPC_SYSCONFIG bits */
#define DISPC_MIDLEMODE_FORCE (0<<12)
#define DISPC_MIDLEMODE_NEVER BIT12
#define DISPC_MIDLEMODE_SMART BIT13

#define DISPC_SIDLEMODE_FORCE (0<<3)
#define DISPC_SIDLEMODE_NEVER BIT3
#define DISPC_SIDLEMODE_SMART BIT4

#define DISPC_ENWAKEUP        BIT2
#define DISPC_SOFTRESET       BIT1
#define DISPC_AUTOIDLE        BIT0

/* DISPC_SYSSTATUS bits */
#define DISPC_RESETDONE 1

/* TODO: rest of IRQ bits */
#define DISPC_VSYNC 2
#define DISPC_FRAMEDONE 1

/* DISPC_CONTROL bits (incomplete) */
#define DISPC_DITHER_FRAMES_0     (0<<30)
#define DISPC_DITHER_FRAMES_2     BIT30
#define DISPC_DITHER_FRAMES_4     BIT31
#define DISPC_LCDENABLEPOL        BIT29
#define DISPC_LCDENABLESIGNAL     BIT28
#define DISPC_PCKFREEENABLE       BIT27
#define DISPC_GPOUT1              BIT16
#define DISPC_GPOUT0              BIT15
#define DISPC_GPIN1               BIT14
#define DISPC_GPIN0               BIT13
#define DISPC_OVERLAYOPTIMISATION BIT12
#define DISPC_STALLMODE           BIT11
#define DISPC_TFTDATALINES_12     (0<<8)  // NOT BIT8
#define DISPC_TFTDATALINES_16     (1<<8)  // BIT8
#define DISPC_TFTDATALINES_18     (2<<8)  // BIT9
#define DISPC_TFTDATALINES_24     (3<<8)  // (BIT9 | BIT8)
#define DISPC_STDITHERENABLE      BIT7
#define DISPC_GODIGITAL           BIT6
#define DISPC_GOLCD               BIT5
#define DISPC_M8B                 BIT4
#define DISPC_STNTFT              BIT3
#define DISPC_MONOCOLOR           BIT2
#define DISPC_DIGITALENABLE       BIT1
#define DISPC_LCDENABLE           BIT0

/* DISPC_CONFIG bits */
#define DISPC_TVALPHABLENDERENABLE  BIT19
#define DISPC_LCDALPHABLENDERENABLE BIT18
#define DISPC_FIFOFILLING           BIT17
#define DISPC_FIFOHANDCHECK         BIT16
#define DISPC_CPR                   BIT15
#define DISPC_FIFOMERGE             BIT14
#define DISPC_TCKDIGSELECTION       BIT13
#define DISPC_TCKDIGENABLE          BIT12
#define DISPC_TCKLCDSELECTION       BIT11
#define DISPC_TCKLCDENABLE          BIT10
#define DISPC_FUNCGATED             BIT9
#define DISPC_ACBIASGATED           BIT8
#define DISPC_VSYNCGATED            BIT7
#define DISPC_HSYNCGATED            BIT6
#define DISPC_PIXELCLOCKGATED       BIT5
#define DISPC_PIXELDATAGATED        BIT4
#define DISPC_PALETTEGAMMATABLE     BIT3
#define DISPC_LOADMODE_EVERY        (0<<1)
#define DISPC_LOADMODE_MANUAL       BIT1
#define DISPC_LOADMODE_FRAME        BIT2
#define DISPC_LOADMODE_FIRST        (3<<1) //(BIT2 | BIT1)
#define DISPC_PIXELGATED            1

/* DISPC_TIMING_H bits and masks */
#define DISPCB_HBP 20
#define DISPCB_HFP 8
#define DISPCB_HSW 0
#define DISPCM_HBP 0xfff00000
#define DISPCM_HFP 0x000fff00
#define DISPCM_HSW 0x000000ff

/* DISPC_TIMING_V bits and masks */
#define DISPCB_VBP 20
#define DISPCB_VFP 8
#define DISPCB_VSW 0
#define DISPCM_VBP 0x0ff00000
#define DISPCM_VFP 0x0000ff00
#define DISPCM_VSW 0x0000003f

/* DISPC_POL_FREQ bits */
#define DISPC_POL_ONOFF     BIT17
#define DISPC_POL_RF        BIT16
#define DISPC_POL_IEO       BIT15
#define DISPC_POL_IPC       BIT14
#define DISPC_POL_IHS       BIT13
#define DISPC_POL_IVS       BIT12
#define DISPCB_POL_ACBI 8
#define DISPCM_POL_ACBI 0xf00
#define DISPCB_POL_ACB 0
#define DISPCM_POL_ACB 0xff

/* DISPC_DIVISOR bits */
#define DISPCB_DIVISOR_LCD 16
#define DISPCB_DIVISOR_PCD 0

/* DISPC_GFX_ATTRIBUTES bits */
#define DISPC_GFXSELFREFRESH (1<<15)
#define DISPC_GFXARBITRATION (1<<14)
#define DISPC_GFXROTATION_0 (0<<12)
#define DISPC_GFXROTATION_90 (1<<12)
#define DISPC_GFXROTATION_180 (2<<12)
#define DISPC_GFXROTATION_270 (3<<12)
#define DISPC_GFXFIFOPRELOAD (1<<11)
#define DISPC_GFXENDIANNESS (1<<10)
#define DISPC_GFXNIBBLEMODE (1<<9)
#define DISPC_GFXCHANNELOUT (1<<8)
#define DISPC_GFXBURSTSIZE_4x32 (0<<6)
#define DISPC_GFXBURSTSIZE_8x32 (1<<6)
#define DISPC_GFXBURSTSIZE_16x32 (2<<6)
#define DISPC_GFXREPLICATIONENABLE (1<<5)
#define DISPC_GFXFORMAT_BITMAP1 (0<<1)
#define DISPC_GFXFORMAT_BITMAP2 (1<<1)
#define DISPC_GFXFORMAT_BITMAP4 (2<<1)
#define DISPC_GFXFORMAT_BITMAP8 (3<<1)
#define DISPC_GFXFORMAT_RGB12 (4<<1)
#define DISPC_GFXFORMAT_ARGB16 (5<<1)
#define DISPC_GFXFORMAT_RGB16 (6<<1)
#define DISPC_GFXFORMAT_RGB32 (8<<1) /* 24 bit, 32-bit un - packed */
#define DISPC_GFXFORMAT_RGB24 (9<<1)
#define DISPC_GFXFORMAT_ARGB32 (12<<1)
#define DISPC_GFXFORMAT_RGBA32 (13<<1)
#define DISPC_GFXFORMAT_RGBx (14<<1)
#define DISPC_GFXENABLE 1

/* DISPC_VIDn_ATTRIBUTES bits */
#define DISPC_VIDSELFREFRESH (1<<24)
#define DISPC_VIDARBITRATION (1<<23)
#define DISPC_VIDLINEBUFFERSPLOT (1<<22)
#define DISPC_VIDVERTICALTAPS (1<<21)
#define DISPC_VIDOPTIMISATION (1<<20)
#define DISPC_VIDFIFOPRELOAD (1<<19)
#define DISPC_VIDROWREPEATENABLE (1<<18)
#define DISPC_VIDENDIANNESS (1<<17)
#define DISPC_VIDCHANNELOUT (1<<16)
#define DISPC_VIDBURSTSIZE_4x32 (0<<14)
#define DISPC_VIDBURSTSIZE_8x32 (1<<14)
#define DISPC_VIDBURSTSIZE_16x32 (2<<14)
#define DISPC_VIDROTATION_0 (0<<12)
#define DISPC_VIDROTATION_90 (1<<12)
#define DISPC_VIDROTATION_180 (2<<12)
#define DISPC_VIDROTATION_270 (3<<12)
#define DISPC_VIDFULLRANGE (1<<11)
#define DISPC_VIDREPLICATIONENABLE (1<<10)
#define DISPC_VIDCOLORCONVENABLE (1<<9)
#define DISPC_VIDVRESIZECONF (1<<8)
#define DISPC_VIDHRESIZECONF (1<<7)
#define DISPC_VIDRESIZEENABLE_H (1<<5)
#define DISPC_VIDRESIZEENABLE_V (2<<5)
#define DISPC_VIDRESIZEENABLE_HV (3<<5)
// Only VID2 supports the alpha formats
#define DISPC_VIDFORMAT_RGB12 (4<<1)
#define DISPC_VIDFORMAT_ARGB16 (5<<1)
#define DISPC_VIDFORMAT_RGB16 (6<<1)
#define DISPC_VIDFORMAT_RGB32 (8<<1) /* 24 bit, 32-bit un - packed */
#define DISPC_VIDFORMAT_RGB24 (9<<1)
#define DISPC_VIDFORMAT_YUV2 (10<<1)
#define DISPC_VIDFORMAT_UYVY (11<<1)
#define DISPC_VIDFORMAT_ARGB32 (12<<1)
#define DISPC_VIDFORMAT_RGBA32 (13<<1)
#define DISPC_VIDFORMAT_RGBx (14<<1)
#define DISPC_VIDENABLE 1


/* Display controller register offsets */



#endif /* HAL_OMAP353X_DISPC_H_ */
