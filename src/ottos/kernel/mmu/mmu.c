/* mmu.c
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
 *  Created on: 11.11.2011
 *      Author: Thomas Wiedemann <thomas.wiedemann@students.fhv.at>
 *
*/
#include "mmu.h"

unsigned int cnt3;
int cnt,cnt2;
int i;
unsigned int *PTEptr;
unsigned int PTE;
int index;

//MasterTable mit NULL initialisieren

void mastertable_init (unsigned int *PTEptr){

PTEptr = (unsigned int *)0x83000000;   /* set base address PT */

		for ( i=0; i<256;i++)
		{
			*PTEptr = 0;PTEptr++;
		}

}


void mastertable_fill(unsigned int *PTEptr, int PTE){
	PTEptr = (unsigned int *)0x83000000;   /* get base address PT */
	PTEptr += 0x80000000 >> 20;  /* set to first PTE in   VA80000000 */
	PTEptr += 0x100-1;     /* set to last PTE in region */

	PTE = 80000000 & 0xfff00000;     /* set physical address */
	PTE |= (0x03 & 0x3) << 10;       /* set Access Permissions  (3= full RW */
	PTE |= 3    << 5;        /* set domain for section */
	PTE |=  (2 & 0x3) << 2;     /* set cache & WB attributes (2 = write through*/
	PTE |= 0x2;                             /* set as section entry */

	for (i =0x100 - 1; i >= 0; i--) /* fill PTE in region */
		{
			*PTEptr-- = PTE + (i << 20);  /* i = 1 MB section */
		}
}



