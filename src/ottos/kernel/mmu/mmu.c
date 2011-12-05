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
#include "../intc/irq.h"


asm("\t .bss _taskMasterTableAddress, 4\n" \
    "\t .bss _globalVariable, 4\n" \
    "\t .global _taskMasterTableAddress\n" \
    "\t .global _globalVariable\n" \
    "taskMasterTableAddress .field _taskMasterTableAddress, 32\n" \
    "globalVariable .field _globalVariable, 32\n");

extern address taskMasterTableAddress;
extern unsigned int globalVariable;
extern volatile unsigned int kernelMasterTable;
extern volatile unsigned int intRamStart;
extern volatile unsigned int extDDRStart;
extern volatile unsigned int intvecsStart;

address m_taskMasterTableAddresses[MAX_MMU_TABLES];
int m_currentTask;


address m_firstFreeInIntRam;
address m_firstFreeInExtDDR;
BOOLEAN m_occupiedPagesIntRam[MAX_PAGES_IN_INT_RAM];
BOOLEAN m_occupiedPagesExtDDR[MAX_PAGES_IN_EXT_DDR];


/*
mem_address_t *taskMasterTableAddresses[MAX_TASKS] = {0x0};

 mem_address_t *firstFreeInIntRam;
 mem_address_t *firstFreeInExtDDR;

mem_address_t *tableAddress = NULL;
mem_address_t masterTableAddress =NULL;

BOOLEAN occupiedPagesIntRam[MAX_PAGES_IN_MEMORY] = {FALSE};
BOOLEAN occupiedPagesExtDDR[MAX_PAGES_IN_MEMORY] = {FALSE};
*/

void MMU_init(){
    int max = 0;
    int i = 0;
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
            m_taskMasterTableAddresses[i] = 0x0;
        }
    }

}



void enableMMU() {
    asm("\t MRC p15, #0, r0, c1, c0, #0\n");
    asm("\t ORR r0, r0, #0x1\n");
    asm("\t MCR p15, #0, r0, c1, c0, #0\n");
}

void initDomainAccess() {
    // Set Domain Access control register to 0101 0101 0101 0101 0101 0101 0101 0111
    asm("\t MOV  r0, #0x5557\n");
    asm("\t MOVT r0, #0x5555\n");
    asm("\t MCR  p15, #0, r0, c3, c0, #0\n");
}

void clearTLB() {
    asm("\t MOV r0, #0x0\n");
    asm("\t MCR p15, #0, r0, c8, c7, #0\n");
}

void lockFirstTLBEntry() {
    globalVariable = EXT_DDR_START;
    asm("\t LDR  r1, globalVariable\n");
    asm("\t LDR r1, [r1]\n");
    asm("\t MOV  r0, #0x1\n"); //base=victim=0 (protect bit=1 [lock])
    asm("\t MOV  r2, #0x0000\n");//base=victim=1 (protect bit=0 [unlock])
    asm("\t MOVT r2, #0x0840\n");
    asm("\t MCR  p15, #0, r0, c10, c0, #1\n"); //Write I-TLB Lockdown Register
    asm("\t MCR  p15, #0, r1, c10, c1, #1\n"); //Prefetch I-TLB
    asm("\t MCR  p15, #0, r2, c10, c0, #1\n"); //Write I-TLB Lockdown Register
}

void setMasterTablePointerTo(address tableAddress) {
     unsigned int tempAddress = NULL;
    taskMasterTableAddress = tableAddress;
    tempAddress = (unsigned int)taskMasterTableAddress & 0xFFFFC000;
    taskMasterTableAddress = (address)tempAddress;

    asm("\t LDR r1, taskMasterTableAddress\n");
    asm("\t LDR r1, [r1]\n");
    asm("\t MCR p15, #0, r1, c2, c0, #0\n");

    clearTLB();
}

address createMasterTable() {
    address masterTableAddress = findFreeMemory(4, TRUE, TRUE);
    memset((void*)masterTableAddress, 0x0000, 4096 * 4);
    return masterTableAddress;
}
address createOrGetL2Table(address masterTableAddress, int masterTableEntryNumber) {
    address result = 0x0;
    unsigned int tableEntry = NULL;
    if (masterTableEntryNumber < 4096) {
        if (*(masterTableAddress + masterTableEntryNumber) == 0x0) {
            result = findFreeMemory(1, TRUE, TRUE);

            tableEntry = (unsigned int)result | 0x00000011;
            *(masterTableAddress + masterTableEntryNumber) = tableEntry;

            memset((void*)result, 0x0, 1024); // 256 entries * 4 bytes per entry = 1024 bytes
        } else {
            result = (address)((*(masterTableAddress + masterTableEntryNumber) >> 10) << 10);
        }
    }
    return result;
}
void createMappedPage(address masterTableAddress, address virtualAddress) {
  unsigned int l2TableEntryNumber= NULL;
  unsigned int tableEntry = NULL;
    unsigned int masterTableEntryNumber = (unsigned int)virtualAddress >> 20;
    address l2TableAddress = createOrGetL2Table(masterTableAddress, masterTableEntryNumber);

    address newPage = findFreeMemory(1, TRUE, TRUE);
    memset((void*)newPage, 0x0, 4096);

    l2TableEntryNumber = ((unsigned int)virtualAddress >> 12) - (((unsigned int)virtualAddress >> 12) & 0xFFF00);
    tableEntry = (unsigned int)newPage | 0x2;
    *(l2TableAddress + l2TableEntryNumber) = tableEntry;
}

void mapOneToOne(address masterTableAddress, address startAddress, unsigned int length) {
  int i ;
  int j ;
    int nrOfMasterTableEntries = (length / 1048576) + 1;

    int firstEntryNumber = ((unsigned int)startAddress >> 12) - (((unsigned int)startAddress >> 12) & 0xFFF00);
    int lastEntryNumber = firstEntryNumber + (length / 4096);

    unsigned int firstL2Entry = ((unsigned int)startAddress >> 20) << 20;
    for (i = 0; i < nrOfMasterTableEntries; i++) {

        unsigned int masterTableEntryNumber = (unsigned int)startAddress >> 20;
        address freeForL2Table = createOrGetL2Table(masterTableAddress, masterTableEntryNumber);

        if (freeForL2Table > 0x0) {
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

address addressOfPage(enum MemoryType mem, int pageNumberInMemory) {
    address result = 0x0;
    if (mem == INT_RAM) {
        result = (address)(INT_RAM_START + (pageNumberInMemory * 4096));
    } else if (mem == EXT_DDR) {
        result = (address)(EXT_DDR_START + (pageNumberInMemory * 4096));
    }
    return result;
}
void reservePages(enum MemoryType mem, int firstPageNumber, int nrOfPages) {
     int i = 0;
    for (i = firstPageNumber; i < (firstPageNumber + nrOfPages); i++) {
        if (mem == INT_RAM) {
            m_occupiedPagesIntRam[i] = TRUE;
        } else if (mem == EXT_DDR) {
            m_occupiedPagesExtDDR[i] = TRUE;
        }
    }
}
void releasePages(enum MemoryType mem, int firstPageNumber, int nrOfPages) {
  int i = 0;
    for (i = firstPageNumber; i < (firstPageNumber + nrOfPages); ++i) {
        if (mem == INT_RAM) {
            m_occupiedPagesIntRam[i] = FALSE;
        } else if (mem == EXT_DDR) {
            m_occupiedPagesExtDDR[i] = FALSE;
        }
        memset((void*)addressOfPage(mem, i), 0x0, 4096);
    }
}

address findFreeMemory(int nrOfPages, BOOLEAN align, BOOLEAN reserve) {
    address result = 0x0;
    int freePages;
    int i =0;
    for ( i = 0; (i < MAX_PAGES_IN_INT_RAM) && (result == 0x0); i++) {
        if ((!m_occupiedPagesIntRam[i]) && ((align) && ((freePages > 0) || ((i % nrOfPages) == 0)))) {
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
        if (!m_occupiedPagesExtDDR[i]) {
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

void initMemoryForTask(int taskId) {
  unsigned int nrOfKernelPages = NULL;
  address tableAddress = NULL;
    address savedTableAddress = m_taskMasterTableAddresses[taskId];
    if (savedTableAddress == (address)0x0) {
        if (taskId == 0) {
            unsigned int i;
            m_firstFreeInIntRam = &intRamStart;
            m_firstFreeInExtDDR = &extDDRStart;

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
            setMasterTablePointerTo(tableAddress);
            // Initialize Master Table
            for (i = 0x00000000; i < 0xFFF00000; i += 0x00100000) {
                *tableAddress = i | 0xC12;
                tableAddress++;
            }
            *tableAddress = 0xFFF00C12;

            enableMMU();
            lockFirstTLBEntry();

        } else {
            taskMasterTableAddress = createMasterTable();

            mapOneToOne(taskMasterTableAddress, (address)ROM_INTERRUPT_ENTRIES, 0x1C);
            mapOneToOne(taskMasterTableAddress, (address)INT_RAM_START, (unsigned int)m_firstFreeInIntRam - INT_RAM_START);
            mapOneToOne(taskMasterTableAddress, &intvecsStart, 0x3B);
            mapOneToOne(taskMasterTableAddress, (address)EXT_DDR_START, (unsigned int)m_firstFreeInExtDDR - EXT_DDR_START);

            setMasterTablePointerTo(taskMasterTableAddress);
        }
        m_taskMasterTableAddresses[taskId] = taskMasterTableAddress;
    } else {
        setMasterTablePointerTo(savedTableAddress);
    }
    m_currentTask = taskId;
}
void loadPage(int pageNumber) {
    //TODO
}

void prepagePagesFor(int serviceId)  {
  //TODO: to be implemented
}

address parameterAddressFor(int serviceId)  {
  //TODO: to be implemented
  return (address)0x820F0000;
}

void handlePrefetchAbort() {
    asm("\t MRC p15, #0, r0, c6, c0, #2\n"); // Read instruction fault address register
    asm("\t LDR r1, globalVariable\n");
    asm("\t STR r0, [r1]\n");

    // TODO check for execute permissions
    if ((globalVariable % 0x4 == 0x0) && (globalVariable >= PROCESS_MEMORY_START) && (globalVariable < PROCESS_MEMORY_END)) {
        int currentTask = m_currentTask;
        initMemoryForTask(0);
        createMappedPage(m_taskMasterTableAddresses[currentTask], (address)globalVariable);
        // TODO load needed instructions into new page
        initMemoryForTask(currentTask);
    } else {
        // TODO invalid access
    }
}

void handleDataAbort() {
    asm("\t MRC p15, #0, r0, c6, c0, #0\n"); // Read data foult address register
    asm("\t LDR r1, globalVariable\n");
    asm("\t STR r0, [r1]\n");
    // TODO check for read / write permissions
    if ((globalVariable % 0x4 == 0x0) && (globalVariable >= PROCESS_MEMORY_START) && (globalVariable < PROCESS_MEMORY_END)) {
        int currentTask = m_currentTask;
        initMemoryForTask(0);
        createMappedPage(m_taskMasterTableAddresses[currentTask], (address)globalVariable);
        initMemoryForTask(currentTask);
    } else {
        // TODO invalid access
    }
}
