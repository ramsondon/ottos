/* fat.h
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
 *  Created on: 09.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef FAT_H_
#define FAT_H_

#include <stdint.h>

// must be 512 byte; anything else is not supported by FAT
#define FAT_SECTOR_SIZE 512

// the last two byte in the MBR has to be the following; one can ensure
// the integrity of the MBR if these two byte are set correct
#define FAT_MAGIC_BEGIN 0x55
#define FAT_MAGIC_END   0xAA

typedef struct {
  uint8_t boot_flag;    // should be ignored
  uint8_t chs_start[3]; // should be ignored
  unit8_t type_code;    // Ox0B or 0x0C represent FAT32
  uint8_t chs_end[3];   // should be ignored
  uint32_t lba_begin;   // begin of the partition on the disk
  uint32_t nr_sectors;  // redundant to FAT32 file system itself
} fat_partition_t;

typedef struct {
  uint16_t bytes_per_sector;    // has to be 512
  uint8_t sectors_per_cluster;  // has to be one of 1,2,4,8,16,32,64,128
  unit16_t nr_reserved_sectors; // usually 0x20
  uint8_t nr_fat;               // number of FATs; always 2
  uint32_t sectors_per_fat;     // depends on disk size
  uint32_t root_cluster;        // usually 0x00000002
  uint16_t signature;           // must be 0x55AA (see MAGIC)
} fat_volume_id;

typedef struct {
  uint8_t name[11];    // 11 byte short filename; 8.3 e.g. "xxxxxxxx.xxx"
  uint8_t attr;        // 0 (LSB) = read only
                       // 1       = hidden
                       // 2       = system (file is operating system)
                       // 3       = volume id; filename is volume id
                       // 4       = is a sub directory
                       // 5       = archive; has been changed since last backup
                       // 6       = unused (should be zero)
                       // 7 (MSB) = unused (should be zero)
  uint16_t cluster_hi; // starting cluster number high bits
  uint16_t cluster_lo; // starting cluster number low bits
  uint32_t size;       // actual space allocated on the disk; an integer number
                       // of clusters;
} fat_directory_t;

#endif /* FAT_H_ */
