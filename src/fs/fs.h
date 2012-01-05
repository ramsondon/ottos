/* fs.h
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

#ifndef FS_H_
#define FS_H_

#include <ottos/const.h>
#include <ottos/types.h>

#define FS_SECTOR_SIZE 512
#define FATFS_NO_DEF_TYPES

typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t  uint8;

EXTERN int fs_read(uint32 sector, uint8 *buffer, uint32 sector_count);

EXTERN int fs_write(uint32 sector, uint8 *buffer, uint32 sector_count);

EXTERN void fs_test();

#endif /* FS_H_ */
