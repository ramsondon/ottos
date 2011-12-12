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
#include "string.h"
#include "mmu.h"
#include "../kernel/pm/process.h"


asm("\t .bss _taskMasterTableAddress, 4\n" \
    "\t .bss _tempVariableForAsmAndC, 4\n" \
    "\t .global _taskMasterTableAddress\n" \
    "\t .global _tempVariableForAsmAndC\n" \
    "taskMasterTableAddress .field _taskMasterTableAddress, 32\n" \
    "tempVariableForAsmAndC .field _tempVariableForAsmAndC, 32\n");

extern address taskMasterTableAddress;
extern unsigned int tempVariableForAsmAndC;
extern volatile unsigned int kernelMasterTable;
extern volatile unsigned int intRamStart;
extern volatile unsigned int extDDRStart;
extern volatile unsigned int intvecsStart;

address m_taskMasterTableAddresses[MAX_MMU_TABLES];


address m_firstFreeInIntRam;
address m_firstFreeInExtDDR;
BOOLEAN m_occupiedPagesIntRam[MAX_PAGES_IN_INT_RAM];
BOOLEAN m_occupiedPagesExtDDR[MAX_PAGES_IN_EXT_DDR];


process_t m_kernelTask;
process_t* m_tasks[MAX_MMU_TABLES];
process_t* m_currentTask;

void MMU_init(){
  int max = NULL;
  int i = NULL;
  m_currentTask = NULL;
  m_firstFreeInIntRam = &intRamStart;
  m_firstFreeInExtDDR = &extDDRStart;

  // Initialize fields

  if (MAX_PAGES_IN_EXT_DDR > MAX_PAGES_IN_INT_RAM) {
         max = MAX_PAGES_IN_EXT_DDR;
     } else {
         max = MAX_PAGES_IN_INT_RAM;
     }
     if (MAX_MMU_TABLES > max) {
         max = MAX_MMU_TABLES;
     }
     for (i = 0; i < max; i++) {
         if (i < MAX_PAGES_IN_EXT_DDR) {
             m_occupiedPagesExtDDR[i] = FALSE;
         }
         if (i < MAX_PAGES_IN_INT_RAM) {
             m_occupiedPagesIntRam[i] = FALSE;
         }
         if (i < MAX_MMU_TABLES) {
             m_tasks[i] = NULL;
         }

    }
    //Initialize Kernel MMU
    initKernelMMU();
}

void initKernelMMU() {

unsigned int nrOfKernelPages = NULL;
address tableAddress = NULL;
unsigned int i = NULL;

// reserve kernel memory
    nrOfKernelPages = (((unsigned int)m_firstFreeInIntRam - INT_RAM_START) / 0x1000);
    if (((unsigned int)m_firstFreeInIntRam - INT_RAM_START) % 0x1000 > 0) {
        nrOfKernelPages++;
    }
    reservePages(INT_RAM, 0, nrOfKernelPages);
    reservePages(INT_RAM, 15, 1);

    nrOfKernelPages = (((unsigned int)m_firstFreeInExtDDR - EXT_DDR_START) / 0x1000);
    if (((unsigned int)m_firstFreeInExtDDR - EXT_DDR_START) % 0x1000 > 0) {
        nrOfKernelPages++;
    }
    reservePages(EXT_DDR, 0, nrOfKernelPages);

    initDomainAccess();

    tableAddress = &kernelMasterTable;
    m_kernelTask.masterTableAddress = tableAddress;

    setMasterTablePointerTo(tableAddress);
    // Initialize Master Table
    for (i = 0x00000000; i < 0xFFF00000; i += 0x00100000) {
        *tableAddress = i | 0xC12;
        tableAddress++;
    }
    *tableAddress = 0xFFF00C12;

    enableMMU();
}



static void enableMMU() {
  //Enable MMU with Co-prozessor Register
    asm("\t MRC p15, #0, r0, c1, c0, #0\n");
    asm("\t ORR r0, r0, #0x1\n");
    asm("\t MCR p15, #0, r0, c1, c0, #0\n");
}

static void initDomainAccess() {
    // Set Domain Access control register to 0101 0101 0101 0101 0101 0101 0101 0111
    asm("\t MOV  r0, #0x5557\n");
    asm("\t MOVT r0, #0x5555\n");
    asm("\t MCR  p15, #0, r0, c3, c0, #0\n");
}

static void clearTLB() {

    asm("\t MOV r0, #0x0\n");
    asm("\t MCR p15, #0, r0, c8, c7, #0\n");


}


static void setMasterTablePointerTo(address tableAddress) {
     unsigned int tempAddress = NULL;
     taskMasterTableAddress = tableAddress;
     tempAddress = (unsigned int)taskMasterTableAddress & 0xFFFFC000;
     taskMasterTableAddress = (address)tempAddress;

        asm("\t LDR r1, taskMasterTableAddress\n");
        asm("\t LDR r1, [r1]\n");
        asm("\t MCR p15, #0, r1, c2, c0, #0\n");

        clearTLB();

}

static address createMasterTable() {
    address masterTableAddress = findFreeMemory(4, TRUE, TRUE);
    memset((void*)masterTableAddress, 0x0000, 4096 * 4);
    return masterTableAddress;
}

static address createOrGetL2Table(address masterTableAddress, int masterTableEntryNumber) {
    address result = 0x0;
    unsigned int tableEntry = NULL;
       if (masterTableEntryNumber < 4096) {
           if (*(masterTableAddress + masterTableEntryNumber) == 0x0) {
               result = findFreeMemory(1, TRUE,TRUE);

               tableEntry = (unsigned int)result | 0x00000011;
               *(masterTableAddress + masterTableEntryNumber) = tableEntry;

               memset((void*)result, 0x0, 1024); // 256 entries * 4 bytes per entry = 1024 bytes
           } else {
               result = (address)((*(masterTableAddress + masterTableEntryNumber) >> 10) << 10);
           }
       }
       return result;
}


static address createMappedPage(address masterTableAddress, address virtualAddress) {
  address newPage = NULL;
  newPage = findFreeMemory(1, TRUE, TRUE);
   memset((void*)newPage, 0x0, 4096);

     mapDirectly(masterTableAddress, virtualAddress, newPage);
     return newPage;

}


void mapDirectly(address masterTableAddress, address virtualAddress, address physicalAddress) {
  unsigned int masterTableEntryNumber = (unsigned int)virtualAddress >> 20;
  address l2TableAddress = createOrGetL2Table(masterTableAddress, masterTableEntryNumber);

  address pageAddress = (address)(((unsigned int)physicalAddress >> 12) << 12);

  unsigned int l2TableEntryNumber = ((unsigned int)virtualAddress >> 12) - (((unsigned int)virtualAddress >> 12) & 0xFFF00);
  unsigned int tableEntry = (unsigned int)pageAddress | 0x2;
  *(l2TableAddress + l2TableEntryNumber) = tableEntry;
}


static void mapOneToOne(address masterTableAddress, address startAddress, unsigned int length) {
  int i = NULL;
  int j = NULL ;
  int nrOfMasterTableEntries = (length / 1048576) + 1;

      int firstEntryNumber = ((unsigned int)startAddress >> 12) - (((unsigned int)startAddress >> 12) & 0xFFF00);
      int lastEntryNumber = firstEntryNumber + (length / 4096);

      unsigned int firstL2Entry = ((unsigned int)startAddress >> 20) << 20;
      for (i = 0; i < nrOfMasterTableEntries; i++) {

          unsigned int masterTableEntryNumber = (unsigned int)startAddress >> 20;
          address freeForL2Table = createOrGetL2Table(masterTableAddress, masterTableEntryNumber);

          if (freeForL2Table > (address)0x0) {
              for (j = firstEntryNumber; (j < 256) && (j <= lastEntryNumber); ++j) {
                  unsigned int tableEntry = firstL2Entry + ((i * 256) + j) * 4096;
                  tableEntry |= 0x2;
                  *(freeForL2Table + j) = tableEntry;
                  firstEntryNumber = j;
              }
          } else {
              // TODO Handle full memory
          }
      }
}

BOOLEAN isTaskPage(address pageAddress) {
  address intVecsPageStart = (address)((((unsigned int)&intvecsStart) >> 12) << 12);
     return !(((pageAddress >= (address)INT_RAM_START) && (pageAddress < m_firstFreeInIntRam))
         || ((pageAddress >= (address)EXT_DDR_START) && (pageAddress < m_firstFreeInExtDDR))
         || ((pageAddress >= (address)ROM_INTERRUPT_ENTRIES) && (pageAddress < (address)ROM_INTERRUPT_ENTRIES + ROM_INTERRUPT_LENGTH))
         || (pageAddress >= intVecsPageStart));
}

int pageForAddress(enum MemoryType* type, unsigned int memAddress) {
  int result = -1;
      if ((memAddress - INT_RAM_START) < 0x10000) {
          result = (memAddress - INT_RAM_START) / 4096;
          *type = INT_RAM;
      } else if ((memAddress - EXT_DDR_START) < 0x10000000) {
          result = (memAddress - EXT_DDR_START) / 4096;
          *type = EXT_DDR;
      }
      return result;
}




static address addressOfPage(enum MemoryType mem, int pageNumberInMemory) {
  address result = 0x0;
    if (mem == INT_RAM) {
        result = (address)(INT_RAM_START + (pageNumberInMemory * 4096));
    } else if (mem == EXT_DDR) {
        result = (address)(EXT_DDR_START + (pageNumberInMemory * 4096));
    }
    return result;
}
static void reservePages(enum MemoryType mem, int firstPageNumber, int nrOfPages) {
     int i = NULL;
     for (i = firstPageNumber; i < (firstPageNumber + nrOfPages); i++) {
            if (mem == INT_RAM) {
                m_occupiedPagesIntRam[i] = TRUE;
            } else if (mem == EXT_DDR) {
                m_occupiedPagesExtDDR[i] = TRUE;
            }
        }
}
static void releasePages(enum MemoryType mem, int firstPageNumber, int nrOfPages) {
  int i = NULL;
  for (i = firstPageNumber; i < (firstPageNumber + nrOfPages); ++i) {
          if (mem == INT_RAM) {
              m_occupiedPagesIntRam[i] = FALSE;
          } else if (mem == EXT_DDR) {
              m_occupiedPagesExtDDR[i] = FALSE;
          }
          memset((void*)addressOfPage(mem, i), 0x0, 4096);
      }
}

static address findFreeMemory(int nrOfPages, BOOLEAN align, BOOLEAN reserve) {
  address result = 0x0;
     int freePages = 0;
     int i = NULL;
     for (i = 0; (i < MAX_PAGES_IN_INT_RAM) && (result == 0x0); i++) {
         if ((!m_occupiedPagesIntRam[i]) && ((!align) || ((freePages > 0) || ((i % nrOfPages) == 0)))) {
             freePages++;
             if (freePages == nrOfPages) {
                 result = addressOfPage(INT_RAM, (i - nrOfPages) + 1);
                 if (reserve) {
                     reservePages(INT_RAM, (i - nrOfPages) + 1, nrOfPages);
                 }
             }
         } else {
             freePages = 0;
         }
     }
     freePages = 0;
     for (i = 0; (i < MAX_PAGES_IN_EXT_DDR) && (result == 0x0); i++) {
         if ((!m_occupiedPagesExtDDR[i]) && ((!align) || ((freePages > 0) || ((i % nrOfPages) == 0)))) {
             freePages++;
             if (freePages == nrOfPages) {
                 result = addressOfPage(EXT_DDR, (i - nrOfPages) + 1);
                 if (reserve) {
                     reservePages(EXT_DDR, (i - nrOfPages) + 1, nrOfPages);
                 }

             }
         } else {
             freePages = 0;
         }
     }
     return result;
}

void switchToKernelMMU() {
    setMasterTablePointerTo(m_kernelTask.masterTableAddress);
}

void initMemoryForTask(process_t* task) {
  process_t* savedTaskPointer = m_tasks[task->pid];

    if (savedTaskPointer == NULL) {
      task->masterTableAddress = createMasterTable();

        mapOneToOne(task->masterTableAddress, (address)ROM_INTERRUPT_ENTRIES, ROM_INTERRUPT_LENGTH);
        mapOneToOne(task->masterTableAddress, (address)INT_RAM_START, (unsigned int)m_firstFreeInIntRam - INT_RAM_START);
        mapOneToOne(task->masterTableAddress, &intvecsStart, 0x3B);
        mapOneToOne(task->masterTableAddress, (address)EXT_DDR_START, (unsigned int)m_firstFreeInExtDDR - EXT_DDR_START);

        //task->messageQueueAddress = createMappedPage((*task)->masterTableAddress, (address)MESSAGE_QUEUE_VIRTUAL_ADDRESS);

        setMasterTablePointerTo(task->masterTableAddress);

        m_tasks[task->pid] = task;
    } else {
        setMasterTablePointerTo(savedTaskPointer->masterTableAddress);
    }
    m_currentTask = task;
}

void deleteTaskMemory(process_t* task) {
  int pageNumber = NULL;
  unsigned int masterTableEntry  = NULL;
  address masterTableAddress = NULL;

   masterTableAddress = task->masterTableAddress;
   if (masterTableAddress != 0x0) {

    for (masterTableEntry = 0; masterTableEntry < 4096; masterTableEntry++) {

     unsigned int l2TableEntry = NULL;
      address l2TableAddress = NULL;
      address pageAddress = NULL;

        l2TableAddress = (address)(((*(masterTableAddress + masterTableEntry)) >> 10) << 10);
        if (l2TableAddress != 0x0) {
            for (l2TableEntry = 0; l2TableEntry < 256; l2TableEntry++) {
                pageAddress = (address)(((*(l2TableAddress + l2TableEntry)) >> 12) << 12);
                if ((pageAddress != 0x0) && (isTaskPage(pageAddress))) {
                  enum MemoryType* type;
                    pageNumber = pageForAddress(type, (unsigned int)pageAddress);
                    releasePages(*type, pageNumber, 1);
                }
                *(l2TableAddress + l2TableEntry) = 0x0;
            }
            *(masterTableAddress + masterTableEntry) = 0x0;
        }
    }
}
m_tasks[task->pid] = NULL;
}


void handlePrefetchAbort() {
  address newPage = NULL;
unsigned int accessedAddress = NULL;

    asm("\t MRC p15, #0, r0, c6, c0, #2\n"); // Read instruction fault address register
    asm("\t LDR r1, tempVariableForAsmAndC\n");
    asm("\t STR r0, [r1]\n");

   accessedAddress = tempVariableForAsmAndC;
    // TODO check for execute permissions
    if ((accessedAddress % 0x4 == 0x0) && (accessedAddress >= PROCESS_MEMORY_START) && (accessedAddress < PROCESS_MEMORY_END)) {
        process_t* currentTask = m_currentTask;
        switchToKernelMMU();
        // Create new Page
        newPage = createMappedPage(currentTask->masterTableAddress,(address) accessedAddress);

        // load needed instructions into new page
        memcpy((void*)newPage, (void*)(currentTask->codeLocation + ((accessedAddress - PROCESS_MEMORY_START) / 4)), 4096);

        initMemoryForTask(currentTask);
    } else {
        // TODO invalid access
    }
}

void handleDataAbort() {
    asm("\t MRC p15, #0, r0, c6, c0, #0\n"); // Read data foult address register
    asm("\t LDR r1, tempVariableForAsmAndC\n");
    asm("\t STR r0, [r1]\n");
    // TODO check for read / write permissions
    if ((tempVariableForAsmAndC % 0x4 == 0x0) && (tempVariableForAsmAndC >= PROCESS_MEMORY_START) && (tempVariableForAsmAndC < PROCESS_MEMORY_END)) {
        process_t* currentTask = m_currentTask;
        switchToKernelMMU();
        createMappedPage(currentTask->masterTableAddress, (address)tempVariableForAsmAndC);
        initMemoryForTask(currentTask);
    } else {
        // TODO invalid access
    }
}
