/*
 * omap35x.h
 *
 *  Created on: 15.11.2011
 *      Author: Fox
 */

#ifndef OMAP35X_H_
#define OMAP35X_H_


#define BIT0                (0x0001)
#define BIT1                (0x0002)
#define BIT2                (0x0004)
#define BIT3                (0x0008)
#define BIT4                (0x0010)
#define BIT5                (0x0020)
#define BIT6                (0x0040)
#define BIT7                (0x0080)
#define BIT8                (0x0100)
#define BIT9                (0x0200)
#define BITA                (0x0400)
#define BITB                (0x0800)
#define BITC                (0x1000)
#define BITD                (0x2000)
#define BITE                (0x4000)
#define BITF                (0x8000)

#define DEFCW(name, address) __no_init union \
{ \
  struct \
  { \
    volatile unsigned char  name##_L; \
    volatile unsigned char  name##_H; \
  }; \
  volatile unsigned short   name; \
} @ address;



//#define CM_FCLKEN1_CORE_            (0x48004A00)
//DEFC(   CM_FCLKEN1_CORE           , CM_FCLKEN1_CORE_)
//#define CM_FCLKEN1_CORE (address)0x48004A00

#endif /* OMAP35X_H_ */
