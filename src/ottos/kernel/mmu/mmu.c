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
#include <ottos/types.h>

asm("\t .bss _taskMasterTableAddress, 4\n" \
    "\t .global _taskMasterTableAddress\n" \
    "taskMasterTableAddress .field _taskMasterTableAddress, 32");

asm("\t .bss _taskL2TableAddress, 4\n" \
    "\t .global _taskL2TableAddress\n" \
    "taskL2TableAddress .field _taskL2TableAddress, 32");

/*
asm("\t .bss _firstFreeInIntRam, 4\n" \
    "\t .global _firstFreeInIntRam\n" \
    "firstFreeInIntRam .field _firstFreeInIntRam, 32");

asm("\t .bss _firstFreeInExtDDR, 4\n" \
    "\t .global _firstFreeInExtDDR\n" \
    "firstFreeInExtDDR .field _firstFreeInExtDDR, 32");
*/
extern mem_address_t *taskMasterTableAddress;
extern mem_address_t *taskL2TableAddress;
extern mem_address_t kernelMasterTable;

extern mem_address_t intRamStart;
extern mem_address_t extDDRStart;

mem_address_t *taskMasterTableAddresses[MAX_TASKS] = {0x0};
mem_address_t *taskL2TableAddresses[MAX_PAGES_L2] = {0x0};

 mem_address_t *firstFreeInIntRam;
 mem_address_t *firstFreeInExtDDR;

mem_address_t *tableAddress = NULL;
mem_address_t *tableAddress2 = NULL;

//bool occupiedAddresses[MAX_PAGES_IN_MEMORY] = {false};


void mmu_initMemoryForTask(int taskId) {

    taskMasterTableAddress = taskMasterTableAddresses[taskId];
    if (taskMasterTableAddress == (mem_address_t)0x0) {
        if (taskId == 0)

        {
          unsigned int i = NULL;
          firstFreeInIntRam = &intRamStart;
          firstFreeInExtDDR = &extDDRStart;

          taskMasterTableAddress = &kernelMasterTable;




              // Set Domain Access control register to 0101 0101 0101 0101 0101 0101 0101 0111
              asm("\t MOV r1, #0x5557\n");
              asm("\t MOVT r1, #0x5555\n");
              asm("\t MCR p15, #0, r1, c3, c0, #0\n");



               // Set the Master Table Pointer to the internal ram
              asm("\t LDR r1, taskMasterTableAddress\n");
              asm("\t LDR r1, [r1]\n");
              asm("\t MCR p15, #0, r1, c2, c0, #0\n");

              tableAddress = taskMasterTableAddress;

              // Initialize Master Table
              for (i = 0x00000000; i < 0xFFF00000; i += 0x00100000)
                  {
                    *tableAddress = i | 0xC12;
                    tableAddress++;
                  }
              *tableAddress = 0xFFF00C12;

              // Enable MMU
              asm("\t MRC p15, #0, r1, c1, c0, #0\n");
              asm("\t ORR r1, r1, #0x1\n");
              asm("\t MCR p15, #0, r1, c1, c0, #0\n");

        }

        else


        {

          // TODO init process memory

          unsigned int i = NULL;
          firstFreeInIntRam = &intRamStart;
          firstFreeInExtDDR = &extDDRStart;
          tableAddress = NULL;
        // TO DOabfrage platz in internal ram sonst ddr ram

          taskMasterTableAddress = firstFreeInIntRam;
                   tableAddress = taskMasterTableAddress;

                    // Initialize Master Table für Prozess
                   for (i = 0; i<4096;i++)
                   {
                     *tableAddress =(mem_address_t) 0x0;
                     tableAddress++;
                   }

                   //Erhöhe int ram start um Mastertable (16kb)
                   //firstFreeInIntRam+=1000000;


          /*


         taskMasterTableAddress = firstFreeInIntRam;
         tableAddress = taskMasterTableAddress;

          // Initialize Master Table für Prozess
         for (i = 0; i<4096;i++)
         {
           *tableAddress =(mem_address_t) firstFreeInExtDDR;
           firstFreeInExtDDR+=1024;
           tableAddress++;
         }

         //Erhöhe int ram start um Mastertable (16kb)
         firstFreeInIntRam+=1000000;


         //warte auf pagefault
         //TODO page fault abhandeln





         taskL2TableAddress = taskL2TableAddresses[MAX_PAGES_L2];
         taskL2TableAddress = &extDDRStart;

         tableAddress2 = taskL2TableAddress;

         for(i =0 ; i<256 ; i++)
         {
           *tableAddress2 = 0x0;
           tableAddress2++;
         }

         //warte auf pagefault
                //TODO page fault abhandeln

         //Erhöhe startadresse DDR um L2 page table (1kb)

          firstFreeInExtDDR+=1024;


/*
         tableAddress2 = taskL2TableAddress;

                       for (i = 0x00000000; i < 0xFF000000; i += 0x01000000)
                           {
                             *tableAddress2 = i | 0x31;
                             tableAddress2++;
                           }
                       *tableAddress2 = 0xFF000031;
  */







                       // Set the Master Table Pointer to the internal ram
                         asm("\t LDR r1, taskMasterTableAddress\n");
                         //asm("\t LDR r2, firstFreeInIntRam\n");
                         //asm("\t LDR r3, firstFreeInExtDDR\n");
                         asm("\t LDR r1, [r1]\n");
                         asm("\t MCR p15, #0, r1, c2, c0, #0\n");





        }
    }
    else
       {
           asm("\t LDR r1, taskMasterTableAddress\n");
           asm("\t LDR r1, [r1]\n");
           asm("\t MCR p15, #0, r1, c2, c0, #0\n");
       }

    taskMasterTableAddresses[taskId] = taskMasterTableAddress;
}




void mmu_loadPage(int pageNumber) {
    //TODO
}



void mmu_init(){
  unsigned int i = NULL;
  mem_address_t tableAddress = (mem_address_t)0x40200000;
     // Set Domain Access control register to 0101 0101 0101 0101 0101 0101 0101 0101
     asm("\t MOV r1, #0x5557\n");
     asm("\t MOVT r1, #0x5555\n");
     asm("\t MCR p15, #0, r1, c3, c0, #0\n");

     // Set the Master Table Pointer to the internal ram
     asm("\t MOV r1, #0x0000\n");
     asm("\t MOVT r1, #0x4020\n");
     asm("\t MCR p15, #0, r1, c2, c0, #0\n");

     // Initialize Master Table
     for (i = 0x00000000; i < 0xFFF00000; i += 0x00100000) {
         tableAddress = i | 0xC12;
         tableAddress++;
     }
     tableAddress = 0xFFF00C12;

     // Enable MMU
     asm("\t MRC p15, #0, r1, c1, c0, #0\n");
     asm("\t ORR r1, r1, #0x1\n");
     asm("\t MCR p15, #0, r1, c1, c0, #0\n");

}


