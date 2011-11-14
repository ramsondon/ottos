/*
 * mmu.h
 *
 *  Created on: 10.11.2011
 *      Author: Thomas
 */

#ifndef MMU_H_
#define MMU_H_

#include <stdint.h>

#define ENABLEMMU       0x00000001
#define ENABLEDCACHE    0x00000004
#define ENABLEICACHE    0x00001000

#define CHANGEMMU       0x00000001
#define CHANGEDCACHE    0x00000004
#define CHANGEICACHE    0x00001000

//To do: Struct der Masterpagetable
typedef struct {
    uint8_t PTE; //Pagetableentry
    uint8_t PTn;     //pagetablenummer

}masterpagetable_t;

typedef struct {
    uint8_t PTE;  //Pagetableentry

}l2pagetable_t;




//Prototype

void mastertable_init (unsigned int *PTEptr);

void mastertable_fill(unsigned int *PTEptr, int PTE);

#endif /* MMU_H_ */
