/* io.h
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
 *  Created on: 04.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef OTTOS_IO_H_
#define OTTOS_IO_H_

#include <ottos/types.h>
#include <ottos/const.h>

// include stdio as long as we do not have our own
#include <stdio.h>

#define STDIN    0
#define STDOUT   1
#define STDERR   2

/**
 * Open a file.
 *
 * @param filename The filename to open.
 * @param mode The mode how to open the file.
 *             "r"  read only mode
 *             "w"  write only mode, truncate file to zero,
 *                  creates file if not existing
 *             "w+" read and write mode
 *             TODO(fdomig@gmail.com) add more modes
 * @return file The file pointer.
 */
//EXTERN file_t* fopen(char* filename, char* mode);

/**
 * Close a file
 *
 * @param fp The file pointer.
 * @return The status code.
 */
//EXTERN int fclose(file_t* fp);

/**
 * Writes a buffer to a given file.
 *
 * @param fp The file to write to.
 * @param size The size of one entry in the buffer.
 * @param count The number of entries in the buffer.
 * @param buffer The buffer to write to the file.
 * @return The status code.
 */
//EXTERN int fwrite(file_t* fp, size_t size, size_t count, char* buffer);

#endif /* OTTOS_IO_H_ */
