/* block.h
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
 *  Created on: 25.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <ottos/types.h>

#define BLOCK_LBA uint64_t
#define BLOCK_STATUS uint32_t

typedef struct {
    uint32_t media_id;         // the current ID of this media
    BOOLEAN removable_media;   // TRUE if this media is removable
    BOOLEAN media_present;     // TRUE if there is a media present in this device
    BOOLEAN logical_partition; // TRUE if the LBA0 is the first block of a partition
                               // for media with only one partition this is TRUE
    BOOLEAN read_only;         // TRUE if this media is read only
    BOOLEAN write_caching;     // TRUE if the write method caches write data
    uint32_t block_size;       // the intrinsic block size of this device
    uint32_t io_allign;        // supplies the alignment requirements for any buffer
                               // that read/writes blocks
    BLOCK_LBA last_block;      // the logical last block on the device
} BLOCK_IO_MEDIA;

typedef struct _BLOCK_IO_PROTOCOL  BLOCK_IO_PROTOCOL;

typedef BLOCK_STATUS(*BLOCK_READ)(
    BLOCK_IO_PROTOCOL *this,
    uint32_t media_id,
    BLOCK_LBA lba,
    uint32_t buffer_size,
    void* buffer);

typedef BLOCK_STATUS(*BLOCK_WRITE)(
    BLOCK_IO_PROTOCOL *this,
    uint32_t media_id,
    BLOCK_LBA lba,
    uint32_t buffer_size,
    void* buffer);

struct _BLOCK_IO_PROTOCOL {
  BLOCK_IO_MEDIA* media;
  BLOCK_READ read_blocks;
  BLOCK_WRITE write_blocks;
  // FIXME: probably needs more?!
};


#endif /* BLOCK_H_ */
