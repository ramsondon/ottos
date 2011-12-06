/* fs.c
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

#include "fs.h"
#include "vfat/fat_filelib.h"
#include "../drivers/mmchs/mmchs.h"

int fs_read(uint32 sector, uint8 *buffer, uint32 sector_count) {

  mmchs_io_device->read(mmchs_io_device,
                         sector * FS_SECTOR_SIZE,
                         sector_count * FS_SECTOR_SIZE,
                         buffer);

  return 1;
}

int fs_write(uint32 sector, uint8 *buffer, uint32 sector_count) {

  mmchs_io_device->write(mmchs_io_device,
                           sector * FS_SECTOR_SIZE,
                           sector_count * FS_SECTOR_SIZE,
                           buffer);

  return 1;
}

void fs_init() {
  fl_init();
  fl_attach_media(fs_read, fs_write);
}

