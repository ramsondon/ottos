/* ow_ui.h
 *
 * Copyright (c) 2011 The ottos_ui project.
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
 *  Created on: 25.01.2012
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#ifndef OW_UI_H_
#define OW_UI_H_

#define RANGE_TEMPERATURE_MIN     -20
#define RANGE_TEMPERATURE_MAX     60
#define RANGE_SOLAR_MIN           0
#define RANGE_SOLAR_MAX           10000
#define RANGE_PRESSURE_MIN        800
#define RANGE_PRESSURE_MAX        1200

#define TREND_UP      1
#define TREND_STABLE  2
#define TREND_DOWN    3

void video_test();


#endif /* OW_UI_H_ */
