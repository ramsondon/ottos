/* omap353x_led.h
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
 *  Created on: Dec 2, 2011
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#ifndef HAL_OMAP353X_LED_H_
#define HAL_OMAP353X_LED_H_

/*
 * Here are all Omap 353x specific addresses relevant for leds on the
 * beagleboard C4
 */
#define GPIO5_OE      0x49054034  /* GPIO5 output enable address */
#define GPIO5_DATAOUT 0x4905603C  /* GPIO5 data out register address*/


#endif /* HAL_OMAP353X_LED_H_ */
