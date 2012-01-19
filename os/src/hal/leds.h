/* leds.h
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
 *  Created on: 03.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#ifndef HAL_LEDS_H_
#define HAL_LEDS_H_



#define LED_ON        1
#define LED_OFF       0

typedef enum LED_DEVICE
{
  LED_DEVICE_USR0 = 22,
  LED_DEVICE_USR1 = 21
} LED_DEVICE;


#endif /* HAL_LEDS_H_ */
