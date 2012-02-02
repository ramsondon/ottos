/* rtc.h
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
 *  Created on: 1 Feb 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#ifndef HAL_OMAP353X_RTC_H_
#define HAL_OMAP353X_RTC_H_


#define RTC_BASE            0x00000000    //0xfffb4800??
#define OMAP_RTC_BASE        0xfffb4800

//Registers for Secure Active Mode

#define SECONDS_REG            0x0000001C        //    R
#define MINUTES_REG            0x0000001D        //    R
#define HOURS_REG            0x0000001E        //    R
#define DAYS_REG            0x0000001F        //    R
#define MONTHS_REG            0x00000020        //    R
#define YEARS_REG            0x00000021        //    R
#define WEEKS_REG            0x00000022        //    R
#define ALARM_SECONDS_REG    0x00000023        //    RW
#define ALARM_MINUTES_REG    0x00000024        //    RW
#define ALARM_HOURS_REG        0x00000025        //    RW
#define ALARM_DAYS_REG        0x00000026        //    RW
#define ALARM_MONTHS_REG    0x00000027        //    RW
#define ALARM_YEARS_REG        0x00000028        //    RW
#define RTC_CTRL_REG        0x00000029        //    R
#define RTC_STATUS_REG        0x0000002A        //    RW
#define RTC_INTERRUPTS_REG    0x0000002B        //    RW
#define RTC_COMP_LSB_REG    0x0000002C        //    R
#define RTC_COMP_MSB_REG    0x0000002D        //    R

#endif /* HAL_OMAP353X_RTC_H_ */
