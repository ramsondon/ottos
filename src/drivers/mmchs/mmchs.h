/* mmchs.h
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
 *  Created on: 24.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef MMCHS_H_
#define MMCHS_H_

#include <ottos/types.h>
#include <ottos/const.h>

#define MMCHS_MAX_RETRY_COUNT   (100*5)

#define MMCHS_HCS               BIT30 //Host capacity support/1 = Supporting high capacity
#define MMCHS_CCS               BIT30 //Card capacity status/1 = High capacity card
typedef struct {
  uint32_t  Reserved0:   7; // 0
  uint32_t  V170_V195:   1; // 1.70V - 1.95V
  uint32_t  V200_V260:   7; // 2.00V - 2.60V
  uint32_t  V270_V360:   9; // 2.70V - 3.60V
  uint32_t  RESERVED_1:  5; // Reserved
  uint32_t  AccessMode:  2; // 00b (byte mode), 10b (sector mode)
  uint32_t  Busy:        1; // This bit is set to LOW if the card has not finished the power up routine
} MMCHS_OCR;

typedef struct {
  uint32_t  NOT_USED;   // 1 [0:0]
  uint32_t  CRC;        // CRC7 checksum [7:1]
  uint32_t  MDT;        // Manufacturing date [19:8]
  uint32_t  RESERVED_1; // Reserved [23:20]
  uint32_t  PSN;        // Product serial number [55:24]
  uint8_t   PRV;        // Product revision [63:56]
  uint8_t   PNM[5];     // Product name [64:103]
  uint16_t  OID;        // OEM/Application ID [119:104]
  uint8_t   MID;        // Manufacturer ID [127:120]
} MMCHS_CID;

typedef struct {
  uint32_t  NOT_USED:           1; // Not used, always 1 [0:0]
  uint32_t  CRC:                7; // CRC [7:1]

  uint32_t  RESERVED_1:         2; // Reserved [9:8]
  uint32_t  FILE_FORMAT:        2; // File format [11:10]
  uint32_t  TMP_WRITE_PROTECT:  1; // Temporary write protection [12:12]
  uint32_t  PERM_WRITE_PROTECT: 1; // Permanent write protection [13:13]
  uint32_t  COPY:               1; // Copy flag (OTP) [14:14]
  uint32_t  FILE_FORMAT_GRP:    1; // File format group [15:15]

  uint32_t  RESERVED_2:         5; // Reserved [20:16]
  uint32_t  WRITE_BL_PARTIAL:   1; // Partial blocks for write allowed [21:21]
  uint32_t  WRITE_BL_LEN:       4; // Max. write data block length [25:22]
  uint32_t  R2W_FACTOR:         3; // Write speed factor [28:26]
  uint32_t  RESERVED_3:         2; // Reserved [30:29]
  uint32_t  WP_GRP_ENABLE:      1; // Write protect group enable [31:31]

  uint32_t  WP_GRP_SIZE:        7; // Write protect group size [38:32]
  uint32_t  SECTOR_SIZE:        7; // Erase sector size [45:39]
  uint32_t  ERASE_BLK_EN:       1; // Erase single block enable [46:46]
  uint32_t  C_SIZE_MULT:        3; // Device size multiplier [49:47]
  uint32_t  VDD_W_CURR_MAX:     3; // Max. write current @ VDD max [52:50]
  uint32_t  VDD_W_CURR_MIN:     3; // Max. write current @ VDD min [55:53]
  uint32_t  VDD_R_CURR_MAX:     3; // Max. read current @ VDD max [58:56]
  uint32_t  VDD_R_CURR_MIN:     3; // Max. read current @ VDD min [61:59]
  uint32_t  C_SIZELow2:         2; // Device size [63:62]

  uint32_t  C_SIZEHigh10:       10;// Device size [73:64]
  uint32_t  RESERVED_4:         2; // Reserved [75:74]
  uint32_t  DSR_IMP:            1; // DSR implemented [76:76]
  uint32_t  READ_BLK_MISALIGN:  1; // Read block misalignment [77:77]
  uint32_t  WRITE_BLK_MISALIGN: 1; // Write block misalignment [78:78]
  uint32_t  READ_BL_PARTIAL:    1; // Partial blocks for read allowed [79:79]
  uint32_t  READ_BL_LEN:        4; // Max. read data block length [83:80]
  uint32_t  CCC:                12;// Card command classes [95:84]

  uint8_t   TRAN_SPEED          ;  // Max. bus clock frequency [103:96]
  uint8_t   NSAC                ;  // Data read access-time 2 in CLK cycles (NSAC*100) [111:104]
  uint8_t   TAAC                ;  // Data read access-time 1 [119:112]

  uint32_t  RESERVED_5:         6; // Reserved [125:120]
  uint32_t  CSD_STRUCTURE:      2; // CSD structure [127:126]
} MMCHS_CSD;

typedef struct {
  uint32_t  NOT_USED:           1; // Not used, always 1 [0:0]
  uint32_t  CRC:                7; // CRC [7:1]
  uint32_t  RESERVED_1:         2; // Reserved [9:8]
  uint32_t  FILE_FORMAT:        2; // File format [11:10]
  uint32_t  TMP_WRITE_PROTECT:  1; // Temporary write protection [12:12]
  uint32_t  PERM_WRITE_PROTECT: 1; // Permanent write protection [13:13]
  uint32_t  COPY:               1; // Copy flag (OTP) [14:14]
  uint32_t  FILE_FORMAT_GRP:    1; // File format group [15:15]
  uint32_t  RESERVED_2:         5; // Reserved [20:16]
  uint32_t  WRITE_BL_PARTIAL:   1; // Partial blocks for write allowed [21:21]
  uint32_t  WRITE_BL_LEN:       4; // Max. write data block length [25:22]
  uint32_t  R2W_FACTOR:         3; // Write speed factor [28:26]
  uint32_t  RESERVED_3:         2; // Reserved [30:29]
  uint32_t  WP_GRP_ENABLE:      1; // Write protect group enable [31:31]
  uint32_t  WP_GRP_SIZE:        7; // Write protect group size [38:32]
  uint32_t  SECTOR_SIZE:        7; // Erase sector size [45:39]
  uint32_t  ERASE_BLK_EN:       1; // Erase single block enable [46:46]
  uint32_t  RESERVED_4:         1; // Reserved [47:47]
  uint32_t  C_SIZELow16:        16;// Device size [69:48]
  uint32_t  C_SIZEHigh6:        6; // Device size [69:48]
  uint32_t  RESERVED_5:         6; // Reserved [75:70]
  uint32_t  DSR_IMP:            1; // DSR implemented [76:76]
  uint32_t  READ_BLK_MISALIGN:  1; // Read block misalignment [77:77]
  uint32_t  WRITE_BLK_MISALIGN: 1; // Write block misalignment [78:78]
  uint32_t  READ_BL_PARTIAL:    1; // Partial blocks for read allowed [79:79]
  uint32_t  READ_BL_LEN:        4; // Max. read data block length [83:80]
  uint32_t  CCC:                12;// Card command classes [95:84]
  uint8_t   TRAN_SPEED          ;  // Max. bus clock frequency [103:96]
  uint8_t   NSAC                ;  // Data read access-time 2 in CLK cycles (NSAC*100) [111:104]
  uint8_t   TAAC                ;  // Data read access-time 1 [119:112]
  uint32_t  RESERVED_6:         6; // 0 [125:120]
  uint32_t  CSD_STRUCTURE:      2; // CSD structure [127:126]
} MMCHS_CSD_SDV2;

typedef enum {
  UNKNOWN_CARD,
  MMC_CARD,              //MMC card
  SD_CARD,               //SD 1.1 card
  SD_CARD_2,             //SD 2.0 or above standard card
  SD_CARD_2_HIGH         //SD 2.0 or above high capacity card
} MMCHS_CARD_TYPE;

typedef enum {
  READ,
  WRITE
} MMCHS_OPERATION_TYPE;

typedef struct  {
  uint16_t        RCA;
  uint32_t        BlockSize;
  uint32_t        NumBlocks;
  uint32_t        ClockFrequencySelect;
  MMCHS_CARD_TYPE CardType;
  MMCHS_OCR       OCRData;
  MMCHS_CID       CIDData;
  MMCHS_CSD       CSDData;
} MMCHS_CARD_INFO;

#define MMCHS_STATUS  unsigned int

#define MMCHS_STATUS_SUCCESS     0
#define MMCHS_STATUS_LOAD_ERROR  1
#define MMCHS_STATUS_TIMEOUT     2

#define MMCHS_DEBUG_INFO         50

#define MMCHS_ERROR_DEVICE       100
#define MMCHS_ERROR_UNSUPPORTED  101
#define MMCHS_ERROR_INVALID      102

EXTERN void mmchs_test();
EXTERN void mmchs_init();

#endif /* MMCHS_H_ */
