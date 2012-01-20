/* bitmap.c
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

#include "bitmap.h"
#include "../fs/vfat/fat_filelib.h"
#include <ottos/types.h>
#include <ottos/const.h>

/* !!!!!! NEEDED FOR MEMORY ALLOCATION !!!!!! */
#include <stdlib.h>
#include <stdio.h>

typedef int bool;

//#pragma pack(pop)

//BITMAP_FILEHEADER bmpFileHeader;
//BITMAP_HEADER bmpHeader;
static RGBA* m_BitmapData;
static unsigned int m_BitmapSize;

// Masks and bit counts shouldn't exceed 32 Bits
static inline unsigned int BitCountByMask(unsigned int Mask) {
  unsigned int BitCount = 0;
  while (Mask) {
    Mask &= Mask - 1;
    BitCount++;
  }
  return BitCount;
}

static inline unsigned int BitPositionByMask(unsigned int Mask) {
  return BitCountByMask((Mask & (~Mask + 1)) - 1);
}

static inline unsigned int ComponentByMask(unsigned int Color,
                                           unsigned int Mask) {
  unsigned int Component = Color & Mask;
  return Component >> BitPositionByMask(Mask);
}

static inline unsigned int BitCountToMask(unsigned int BitCount) {
  return (BitCount == 32) ? 0xFFFFFFFF : (1 << BitCount) - 1;
}

static unsigned int Convert(unsigned int Color, unsigned int FromBitCount,
                            unsigned int ToBitCount) {
  if (ToBitCount < FromBitCount) {
    Color >>= (FromBitCount - ToBitCount);
  } else {
    Color <<= (ToBitCount - FromBitCount);
    if (Color > 0) {
      Color |= BitCountToMask(ToBitCount - FromBitCount);
    }
  }
  return Color;
}



/* Load specified Bitmap and stores it as RGBA in an internal buffer */
RGBA* graphics_parse_bmp_picture_array(void* filecontent, int size, BITMAP_FILEHEADER* bmpFileHeader,
                                 BITMAP_HEADER* bmpHeader) {
  int k, Index;
  unsigned int i, j, LineWidth;
  unsigned int ColorTableSize = 0;
  char* Line;
  char* file = filecontent;
  char* eof = file + size;  // marks the end of the file in the memory
  BGRA* ColorTable;

  bmpFileHeader = (BITMAP_FILEHEADER*)file;
  if (bmpFileHeader->Signature != BITMAP_SIGNATURE) {
    return 0;
  }

  // jump to begin of bitmap header
  file += BITMAP_FILEHEADER_SIZE;
  bmpHeader = (BITMAP_HEADER*)file;

  /* Load Color Table */
  if (bmpHeader->BitCount == 1) {
    ColorTableSize = 2;
  } else if (bmpHeader->BitCount == 4) {
    ColorTableSize = 16;
  } else if (bmpHeader->BitCount == 8) {
    ColorTableSize = 256;
  }

  /* Only full color tables are supported */
 /* if (bmpHeader->ClrUsed != 0 && ColorTableSize != bmpHeader->ClrUsed) {
    return 0;
  }
*/
  // jump to begin of color table
  file += bmpHeader->HeaderSize;
  ColorTable = (BGRA*)file;

  /* ... Color Table for 16 bits images are not supported yet */
  m_BitmapSize = bmpHeader->width * bmpHeader->height;
  m_BitmapData = malloc(m_BitmapSize * sizeof(RGBA));

  LineWidth = ((bmpHeader->width * bmpHeader->BitCount / 8) + 3) & ~3;
  file = ((char*)filecontent) + bmpFileHeader->BitsOffset;
  Index = 0;

  if (bmpHeader->Compression == 0) {
    uint8_t *LinePtr;

    for (i = 0; i < bmpHeader->height; i++) {
      //file.read((char*) Line, LineWidth);
      Line = file;
      LinePtr = Line;

      for (j = 0; j < bmpHeader->width; j++) {
        if (bmpHeader->BitCount == 1) {
          uint32_t Color = *((uint8_t*) LinePtr);
          for (k = 0; k < 8; k++) {
            m_BitmapData[Index].Red = ColorTable[Color & 0x80 ? 1 : 0].Red;
            m_BitmapData[Index].Green = ColorTable[Color & 0x80 ? 1 : 0].Green;
            m_BitmapData[Index].Blue = ColorTable[Color & 0x80 ? 1 : 0].Blue;
            m_BitmapData[Index].Alpha = ColorTable[Color & 0x80 ? 1 : 0].Alpha;
            Index++;
            Color <<= 1;
          }
          LinePtr++;
          j += 7;
        } else if (bmpHeader->BitCount == 4) {
          uint32_t Color = *((uint8_t*) LinePtr);
          m_BitmapData[Index].Red = ColorTable[(Color >> 4) & 0x0f].Red;
          m_BitmapData[Index].Green = ColorTable[(Color >> 4) & 0x0f].Green;
          m_BitmapData[Index].Blue = ColorTable[(Color >> 4) & 0x0f].Blue;
          m_BitmapData[Index].Alpha = ColorTable[(Color >> 4) & 0x0f].Alpha;
          Index++;
          m_BitmapData[Index].Red = ColorTable[Color & 0x0f].Red;
          m_BitmapData[Index].Green = ColorTable[Color & 0x0f].Green;
          m_BitmapData[Index].Blue = ColorTable[Color & 0x0f].Blue;
          m_BitmapData[Index].Alpha = ColorTable[Color & 0x0f].Alpha;
          Index++;
          LinePtr++;
          j++;
        } else if (bmpHeader->BitCount == 8) {
          uint32_t Color = *((uint8_t*) LinePtr);
          m_BitmapData[Index].Red = ColorTable[Color].Red;
          m_BitmapData[Index].Green = ColorTable[Color].Green;
          m_BitmapData[Index].Blue = ColorTable[Color].Blue;
          m_BitmapData[Index].Alpha = ColorTable[Color].Alpha;
          Index++;
          LinePtr++;
        } else if (bmpHeader->BitCount == 16) {
          uint32_t Color = *((uint16_t*) LinePtr);
          m_BitmapData[Index].Red = ((Color >> 10) & 0x1f) << 3;
          m_BitmapData[Index].Green = ((Color >> 5) & 0x1f) << 3;
          m_BitmapData[Index].Blue = (Color & 0x1f) << 3;
          m_BitmapData[Index].Alpha = 255;
          Index++;
          LinePtr += 2;
        } else if (bmpHeader->BitCount == 24) {
          uint32_t Color = *((uint32_t*) LinePtr);
          m_BitmapData[Index].Blue = Color & 0xff;
          m_BitmapData[Index].Green = (Color >> 8) & 0xff;
          m_BitmapData[Index].Red = (Color >> 16) & 0xff;
          m_BitmapData[Index].Alpha = 255;
          Index++;
          LinePtr += 3;
        } else if (bmpHeader->BitCount == 32) {
          uint32_t Color = *((uint32_t*) LinePtr);
          m_BitmapData[Index].Blue = Color & 0xff;
          m_BitmapData[Index].Green = (Color >> 8) & 0xff;
          m_BitmapData[Index].Red = (Color >> 16) & 0xff;
          m_BitmapData[Index].Alpha = Color >> 24;
          Index++;
          LinePtr += 4;
        }
      }

      // go to next line
      file += LineWidth;
    }
  } else {
    // no comparison is supported
    return 0;
  }

  return m_BitmapData;
}


/* Load specified Bitmap and stores it as RGBA in an internal buffer */
RGBA* graphics_parse_bmp_picture(void* file, BITMAP_FILEHEADER* bmpFileHeader, BITMAP_HEADER* bmpHeader) {
//  std::ifstream file(Filename, std::ios::binary | std::ios::in);

//  if (file.bad()) {
//    return null;
//  }
//
//  if (file.is_open() == FALSE) {
//    return null;
//  }

  int k, Index;
  unsigned int i, j, LineWidth;
  unsigned int ColorTableSize = 0;
  char* Line;
  BGRA* ColorTable;

  // read the bmp file header
  //file.read((char*) &bmpFileHeader, BITMAP_FILEHEADER_SIZE); // read file Bitmap File Header into bmpFileHeader
  fl_fread((char*)bmpFileHeader, BITMAP_FILEHEADER_SIZE, 1, file);
  //bmpFileHeader = (BITMAP_FILEHEADER*)file;
  if (bmpFileHeader->Signature != BITMAP_SIGNATURE) {
    return 0;
  }

  // read the bmp header
  //file.read((char*) &bmpHeader, sizeof(BITMAP_HEADER));
  fl_fread((char*)bmpHeader, sizeof(BITMAP_HEADER), 1, file);
  // jump to begin of bitmap header
  //file += BITMAP_FILEHEADER_SIZE;
  //bmpHeader = (BITMAP_HEADER*)file;

  /* Load Color Table */
  if (bmpHeader->BitCount == 1) {
    ColorTableSize = 2;
  } else if (bmpHeader->BitCount == 4) {
    ColorTableSize = 16;
  } else if (bmpHeader->BitCount == 8) {
    ColorTableSize = 256;
  }

  /* Only full color tables are supported */
  if (bmpHeader->ClrUsed != 0 && (ColorTableSize != bmpHeader->ClrUsed)) {
    return 0;
  }

  // jump to Color Table
  //file.seekg(BITMAP_FILEHEADER_SIZE + bmpHeader->HeaderSize, std::ios::beg); // jump to character (BITMAP_FILEHEADER_SIZE + bmpHeader->HeaderSize)
  fl_fseek(file, BITMAP_FILEHEADER_SIZE + bmpHeader->HeaderSize, SEEK_SET);
  //file += bmpHeader->HeaderSize;

  //BGRA* ColorTable = new BGRA[ColorTableSize]; // std::bad_alloc exception should be thrown if memory is not available
  printf("malloc: %i", sizeof(BGRA) * ColorTableSize);
  ColorTable = malloc(sizeof(BGRA) * ColorTableSize);

  // read the color table
  //file.read((char*) ColorTable, sizeof(BGRA) * ColorTableSize);
  fl_fread((char*) ColorTable, sizeof(BGRA), ColorTableSize, file);
  //ColorTable = (BGRA*)file;

  /* ... Color Table for 16 bits images are not supported yet */
  m_BitmapSize = bmpHeader->width * bmpHeader->height;
  //m_BitmapData = new RGBA[m_BitmapSize];
  printf("malloc: %i", sizeof(RGBA) * m_BitmapSize);
  m_BitmapData = malloc(m_BitmapSize * sizeof(RGBA));

  LineWidth = ((bmpHeader->width * bmpHeader->BitCount / 8) + 3) & ~3;
  printf("malloc: %i", LineWidth * sizeof(uint8_t));
  Line = malloc(LineWidth * sizeof(uint8_t));

  //file.seekg(bmpFileHeader->BitsOffset, std::ios::beg); // jump to position bmpFileHeader->BitsOffset
  fl_fseek(file, bmpFileHeader->BitsOffset, SEEK_SET);
  //file = ((char*)filecontent) + bmpFileHeader->BitsOffset;
  Index = 0;

  if (bmpHeader->Compression == 0) {
    uint8_t *LinePtr;

    for (i = 0; i < bmpHeader->height; i++) {
      //file.read((char*) Line, LineWidth);
      fl_fread((char*) Line, sizeof(char), LineWidth, file);
      Line = file;
      LinePtr = (uint8_t*)Line;

      for (j = 0; j < bmpHeader->width; j++) {
        if (bmpHeader->BitCount == 1) {
          uint32_t Color = *((uint8_t*) LinePtr);
          for (k = 0; k < 8; k++) {
            m_BitmapData[Index].Red = ColorTable[Color & 0x80 ? 1 : 0].Red;
            m_BitmapData[Index].Green = ColorTable[Color & 0x80 ? 1 : 0].Green;
            m_BitmapData[Index].Blue = ColorTable[Color & 0x80 ? 1 : 0].Blue;
            m_BitmapData[Index].Alpha = ColorTable[Color & 0x80 ? 1 : 0].Alpha;
            Index++;
            Color <<= 1;
          }
          LinePtr++;
          j += 7;
        } else if (bmpHeader->BitCount == 4) {
          uint32_t Color = *((uint8_t*) LinePtr);
          m_BitmapData[Index].Red = ColorTable[(Color >> 4) & 0x0f].Red;
          m_BitmapData[Index].Green = ColorTable[(Color >> 4) & 0x0f].Green;
          m_BitmapData[Index].Blue = ColorTable[(Color >> 4) & 0x0f].Blue;
          m_BitmapData[Index].Alpha = ColorTable[(Color >> 4) & 0x0f].Alpha;
          Index++;
          m_BitmapData[Index].Red = ColorTable[Color & 0x0f].Red;
          m_BitmapData[Index].Green = ColorTable[Color & 0x0f].Green;
          m_BitmapData[Index].Blue = ColorTable[Color & 0x0f].Blue;
          m_BitmapData[Index].Alpha = ColorTable[Color & 0x0f].Alpha;
          Index++;
          LinePtr++;
          j++;
        } else if (bmpHeader->BitCount == 8) {
          uint32_t Color = *((uint8_t*) LinePtr);
          m_BitmapData[Index].Red = ColorTable[Color].Red;
          m_BitmapData[Index].Green = ColorTable[Color].Green;
          m_BitmapData[Index].Blue = ColorTable[Color].Blue;
          m_BitmapData[Index].Alpha = ColorTable[Color].Alpha;
          Index++;
          LinePtr++;
        } else if (bmpHeader->BitCount == 16) {
          uint32_t Color = *((uint16_t*) LinePtr);
          m_BitmapData[Index].Red = ((Color >> 10) & 0x1f) << 3;
          m_BitmapData[Index].Green = ((Color >> 5) & 0x1f) << 3;
          m_BitmapData[Index].Blue = (Color & 0x1f) << 3;
          m_BitmapData[Index].Alpha = 255;
          Index++;
          LinePtr += 2;
        } else if (bmpHeader->BitCount == 24) {
          uint32_t Color = *((uint32_t*) LinePtr);
          m_BitmapData[Index].Blue = Color & 0xff;
          m_BitmapData[Index].Green = (Color >> 8) & 0xff;
          m_BitmapData[Index].Red = (Color >> 16) & 0xff;
          m_BitmapData[Index].Alpha = 255;
          Index++;
          LinePtr += 3;
        } else if (bmpHeader->BitCount == 32) {
          uint32_t Color = *((uint32_t*) LinePtr);
          m_BitmapData[Index].Blue = Color & 0xff;
          m_BitmapData[Index].Green = (Color >> 8) & 0xff;
          m_BitmapData[Index].Red = (Color >> 16) & 0xff;
          m_BitmapData[Index].Alpha = Color >> 24;
          Index++;
          LinePtr += 4;
        }
      }

      // go to next line
      //file += LineWidth;
    }
  } else {
    // no compression supported!!!
    return 0;
  }

  /*else if (bmpHeader->Compression == 1) { // RLE 8
    uint8_t Count = 0;
    uint8_t ColorIndex = 0;
    int x = 0, y = 0, k;

    while (fl_feof(file) == FALSE) {
    //while (file < eof) {
      //file.read((char*) &Count, sizeof(uint8_t));
      fl_fread((char*) &Count, sizeof(uint8_t), 1, file);
      //Count = *file;
      //file++;

      //file.read((char*) &ColorIndex, sizeof(uint8_t));
      fl_fread((char*) &ColorIndex, sizeof(uint8_t), 1, file);
      //ColorIndex = *file;
      //file++;

      if (Count > 0) {
        Index = x + y * bmpHeader->width;
        for (k = 0; k < Count; k++) {
          m_BitmapData[Index + k].Red = ColorTable[ColorIndex].Red;
          m_BitmapData[Index + k].Green = ColorTable[ColorIndex].Green;
          m_BitmapData[Index + k].Blue = ColorTable[ColorIndex].Blue;
          m_BitmapData[Index + k].Alpha = ColorTable[ColorIndex].Alpha;
        }
        x += Count;
      } else if (Count == 0) {
        int Flag = ColorIndex;
        if (Flag == 0) {
          x = 0;
          y++;
        } else if (Flag == 1) {
          break;
        } else if (Flag == 2) {
          char rx = 0;
          char ry = 0;
          //file.read((char*) &rx, sizeof(char));
          fl_fread((char*) &rx, sizeof(char), 1, file);
          //rx = *file;
          //file++;
          //file.read((char*) &ry, sizeof(char));
          fl_fread((char*) &ry, sizeof(char), 1, file);
          //ry = *file;
          //file++;

          x += rx;
          y += ry;
        } else {
          Count = Flag;
          Index = x + y * bmpHeader->width;
          for (k = 0; k < Count; k++) {
            //file.read((char*) &ColorIndex, sizeof(uint8_t));
            fl_fread((char*) &ColorIndex, sizeof(uint8_t), 1, file);
            //ColorIndex = *file;
            //file++;

            m_BitmapData[Index + k].Red = ColorTable[ColorIndex].Red;
            m_BitmapData[Index + k].Green = ColorTable[ColorIndex].Green;
            m_BitmapData[Index + k].Blue = ColorTable[ColorIndex].Blue;
            m_BitmapData[Index + k].Alpha = ColorTable[ColorIndex].Alpha;
          }
          x += Count;
          // Attention: Current Microsoft STL implementation seems to be buggy, tellg() always returns 0.
          //if (file.tellg() & 1) {
          if (FALSE) {
            // What's that??? only true when file.tellg() returns 1 (tellg returns the position of the indexer of the file)
            //file.seekg(1, std::ios::cur); // jump to the next character
            fl_fseek(file, 1, SEEK_CUR);
            //file++;
          }
        }
      }
    }
  } else if (bmpHeader->Compression == 2) { // RLE 4
    // RLE 4 is not supported
    return 0;
  } else if (bmpHeader->Compression == 3) { // BITFIELDS
    // We assumes that mask of each color component can be in any order
    unsigned int i, j;
    uint8_t* LinePtr;
    uint32_t BitCountRed = BitCountByMask(bmpHeader->RedMask);
    uint32_t BitCountGreen = BitCountByMask(bmpHeader->GreenMask);
    uint32_t BitCountBlue = BitCountByMask(bmpHeader->BlueMask);
    uint32_t BitCountAlpha = BitCountByMask(bmpHeader->AlphaMask);

    for (i = 0; i < bmpHeader->height; i++) {
      //file.read((char*) Line, LineWidth);
      fl_fread((char*) Line, sizeof(char), LineWidth, file);

      Line = file;
      LinePtr = Line;

      for (j = 0; j < bmpHeader->width; j++) {

        uint32_t Color = 0;

        if (bmpHeader->BitCount == 16) {
          Color = *((uint16_t*) LinePtr);
          LinePtr += 2;
        } else if (bmpHeader->BitCount == 32) {
          Color = *((uint32_t*) LinePtr);
          LinePtr += 4;
        } else {
          // Other formats are not valid
        }
        m_BitmapData[Index].Red = Convert(ComponentByMask(Color, bmpHeader->RedMask), BitCountRed, 8);
        m_BitmapData[Index].Green = Convert(ComponentByMask(Color, bmpHeader->GreenMask), BitCountGreen, 8);
        m_BitmapData[Index].Blue = Convert(ComponentByMask(Color, bmpHeader->BlueMask), BitCountBlue, 8);
        m_BitmapData[Index].Alpha = Convert(ComponentByMask(Color, bmpHeader->AlphaMask), BitCountAlpha, 8);

        Index++;
      }

      //file += LineWidth;
    }
  }
  */

  //delete [] ColorTable;
  //delete [] Line;
  //file.close();

  return m_BitmapData;
}


/* Copies internal RGBA buffer to user specified buffer and converts it into destination
 * bit format specified by component masks.
 *
 * Typical Bitmap color formats (BGR/BGRA):
 *
 * Masks for 16 bit (5-5-5): ALPHA = 0x00000000, RED = 0x00007C00, GREEN = 0x000003E0, BLUE = 0x0000001F
 * Masks for 16 bit (5-6-5): ALPHA = 0x00000000, RED = 0x0000F800, GREEN = 0x000007E0, BLUE = 0x0000001F
 * Masks for 24 bit: ALPHA = 0x00000000, RED = 0x00FF0000, GREEN = 0x0000FF00, BLUE = 0x000000FF
 * Masks for 32 bit: ALPHA = 0xFF000000, RED = 0x00FF0000, GREEN = 0x0000FF00, BLUE = 0x000000FF
 *
 * Other color formats (RGB/RGBA):
 *
 * Masks for 32 bit (RGBA): ALPHA = 0xFF000000, RED = 0x000000FF, GREEN = 0x0000FF00, BLUE = 0x00FF0000
 *
 * Bit count will be rounded to next 8 bit boundary. If IncludePadding is TRUE, it will be ensured
 * that line width is a multiple of 4. padding bytes are included if necessary.
 *
 * NOTE: systems with big endian byte order may require masks in inversion order.
 */

bool GetBits(void* Buffer, BITMAP_HEADER* bmpHeader, unsigned int Size, unsigned int RedMask,
             unsigned int GreenMask, unsigned int BlueMask,
             unsigned int AlphaMask, bool IncludePadding/* = TRUE*/) {
  bool Result = FALSE;
  unsigned int i, j, k, w, LineWidth;
  uint32_t BitPosRed, BitPosGreen, BitPosBlue, BitPosAlpha;
  uint8_t* BufferPtr;
  uint32_t BitCountRed = BitCountByMask(RedMask);
  uint32_t BitCountGreen = BitCountByMask(GreenMask);
  uint32_t BitCountBlue = BitCountByMask(BlueMask);
  uint32_t BitCountAlpha = BitCountByMask(AlphaMask);

  unsigned int BitCount = (BitCountRed + BitCountGreen + BitCountBlue
      + BitCountAlpha + 7) & ~7;

  if (BitCount > 32) {
    return FALSE;
  }

  w = bmpHeader->width;
  LineWidth = (w + 3) & ~3;

  if (Size == 0 || Buffer == 0) {
    Size = (LineWidth * bmpHeader->height * BitCount) / 8 + sizeof(unsigned int);
    return TRUE;
  }

  BufferPtr = (uint8_t*) Buffer;
  Result = TRUE;

  BitPosRed = BitPositionByMask(RedMask);
  BitPosGreen = BitPositionByMask(GreenMask);
  BitPosBlue = BitPositionByMask(BlueMask);
  BitPosAlpha = BitPositionByMask(AlphaMask);

  j = 0;

  for (i = 0; i < m_BitmapSize; i++) {
    *(uint32_t*) BufferPtr = (Convert(m_BitmapData[i].Blue, 8, BitCountBlue) << BitPosBlue)
        | (Convert(m_BitmapData[i].Green, 8, BitCountGreen) << BitPosGreen)
        | (Convert(m_BitmapData[i].Red, 8, BitCountRed) << BitPosRed)
        | (Convert(m_BitmapData[i].Green, 8, BitCountAlpha) << BitPosAlpha);

    if (IncludePadding) {
      j++;
      if (j >= w) {
        for (k = 0; k < LineWidth - w; k++) {
          BufferPtr += (BitCount >> 3);
        }
        j = 0;
      }
    }

    BufferPtr += (BitCount >> 3);
  }

  Size -= sizeof(unsigned int);

  return Result;
}
