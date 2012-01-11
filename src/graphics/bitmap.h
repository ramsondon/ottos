/* bithmap.h
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
 *  Created on: 05.01.2012
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#ifndef BITMAP_H
#define BITMAP_H

#include <ottos/types.h>


#ifndef __LITTLE_ENDIAN__
  #ifndef __BIG_ENDIAN__
    #define __LITTLE_ENDIAN__
  #endif
#endif

#ifdef __LITTLE_ENDIAN__
  #define BITMAP_SIGNATURE 0x4d42
#else
  #define BITMAP_SIGNATURE 0x424d
#endif

/*
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
  typedef unsigned __int32 uint32_t;
  typedef unsigned __int16 uint16_t;
  typedef unsigned __int8 uint8_t;
  typedef __int32 int32_t;
#elif defined(__GNUC__) || defined(__CYGWIN__) || defined(__MWERKS__) || defined(__WATCOMC__) || defined(__PGI) || defined(__LCC__)
  #include <stdint.h>
#else
  typedef unsigned int uint32_t;
  typedef unsigned short int uint16_t;
  typedef unsigned char uint8_t;
  typedef int int32_t;
#endif

#pragma pack(push, 1)
*/

typedef struct _BITMAP_FILEHEADER {
  uint16_t Signature;
  uint32_t Size;
  uint32_t Reserved;
  uint32_t BitsOffset;
} BITMAP_FILEHEADER;

#define BITMAP_FILEHEADER_SIZE 14

typedef struct _BITMAP_HEADER {
  uint32_t HeaderSize;
  int32_t width;
  int32_t height;
  uint16_t Planes;
  uint16_t BitCount;
  uint32_t Compression;
  uint32_t SizeImage;
  int32_t PelsPerMeterX;
  int32_t PelsPerMeterY;
  uint32_t ClrUsed;
  uint32_t ClrImportant;
  uint32_t RedMask;
  uint32_t GreenMask;
  uint32_t BlueMask;
  uint32_t AlphaMask;
  uint32_t CsType;
  uint32_t Endpoints[9]; // see http://msdn2.microsoft.com/en-us/library/ms536569.aspx
  uint32_t GammaRed;
  uint32_t GammaGreen;
  uint32_t GammaBlue;
} BITMAP_HEADER;

typedef struct _RGBA {
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  uint8_t Alpha;
} RGBA;

typedef struct _BGRA {
  uint8_t Blue;
  uint8_t Green;
  uint8_t Red;
  uint8_t Alpha;
} BGRA;

EXTERN RGBA* graphics_parse_bmp_picture(void* filecontent, int size, BITMAP_FILEHEADER* bmpFileHeader, BITMAP_HEADER* bmpHeader);

#endif /* BITHMAP_H_ */
