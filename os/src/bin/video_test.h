/* video_test.h
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
 *  Created on: 11.01.2012
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#ifndef VIDEO_TEST_H_
#define VIDEO_TEST_H_

#include <ottos/const.h>

#define RES_WIDTH 1024
#define RES_HEIGHT 768
#define WIDTH  1024
#define HEIGHT 768

EXTERN void video_test();
EXTERN void video_bmp_test_file();
EXTERN void video_bmp_test_array();

#endif /* VIDEO_TEST_H_ */
