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
#include "../kernel/ram_manager/ram_manager.h"


asm("\t .bss _processMasterTableAddress, 4\n" \
    "\t .bss _tempVariableForAsmAndC, 4\n" \
    "\t .global _processMasterTableAddress\n" \
    "\t .global _tempVariableForAsmAndC\n" \
    "processMasterTableAddress .field _processMasterTableAddress, 32\n" \
    "tempVariableForAsmAndC .field _tempVariableForAsmAndC, 32\n");

extern address processMasterTableAddress;
extern unsigned int tempVariableForAsmAndC;
extern volatile unsigned int kernelMasterTable;
extern volatile unsigned int intRamStart;
extern volatile unsigned int extDDRStart;
extern volatile unsigned int intvecsStart;


address m_firstFreeInIntRam;
address m_firstFreeInExtDDR;
BOOLEAN m_occupiedPagesIntRam[MAX_PAGES_IN_INT_RAM];
BOOLEAN m_occupiedPagesExtDDR[MAX_PAGES_IN_EXT_DDR];


process_t m_kernelProcess;
process_t* m_process[MAX_TASKS];
process_t* m_currentProcess;

void MMU_init() {
   int i = NULL;
    m_currentProcess = NULL;
    m_firstFreeInIntRam = &intRamStart;
    m_firstFreeInExtDDR = &extDDRStart;
    for (i = 0; i < MAX_TASKS; i++) {
        m_process[i] = NULL;
    }

    //Initialize MMU
    initKernelMMU();
}

void initKernelMMU() {
    // reserve kernel memory
  unsigned int i = NULL;
  address tableAddress = NULL;
    unsigned int nrOfKernelPages = (((unsigned int)m_firstFreeInIntRam - INT_RAM_START) / 0x1000);
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
    m_kernelProcess.masterTableAddress = tableAddress;

    setMasterTablePointerTo(tableAddress);
    // Initialize Master Table
    for (i = 0x00000000; i < 0xFFF00000; i += 0x00100000) {
        *tableAddress = i | 0xC12;
        tableAddress++;
    }
    *tableAddress = 0xFFF00C12;

    enableMMU();
}



void enableMMU() {
  // Set internal MMU from OMAP 3530 on
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



void setMasterTablePointerTo(address tableAddress) {
  unsigned int tempAddress = NULL;
    processMasterTableAddress = tableAddress;
    tempAddress = (unsigned int)processMasterTableAddress & 0xFFFFC000;
    processMasterTableAddress = (address)tempAddress;

    asm("\t LDR r1, processMasterTableAddress\n");
    asm("\t LDR r1, [r1]\n");
    asm("\t MCR p15, #0, r1, c2, c0, #0\n");

    clearTLB();
}

address createMasterTable() {
  address masterTableAddress = NULL;
  masterTableAddress =findFreeMemory(4,TRUE, TRUE);
    memset((void*)masterTableAddress, 0x0000, 4096 * 4);
    return masterTableAddress;
}
address createOrGetL2Table(address masterTableAddress, int masterTableEntryNumber) {
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

address createMappedPage(address masterTableAddress, address virtualAddress) {
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

void mapOneToOne(address masterTableAddress, address startAddress, unsigned int length) {
  int i = NULL;
  int j = NULL;
    int nrOfMasterTableEntries = (length / 1048576) + 1;

    int firstEntryNumber = ((unsigned int)startAddress >> 12) - (((unsigned int)startAddress >> 12) & 0xFFF00);
    int lastEntryNumber = firstEntryNumber + (length / 4096);

    unsigned int firstL2Entry = ((unsigned int)startAddress >> 20) << 20;
    for (i = 0; i < nrOfMasterTableEntries; i++) {

        unsigned int masterTableEntryNumber = (unsigned int)startAddress >> 20;
        address freeForL2Table = createOrGetL2Table(masterTableAddress, masterTableEntryNumber);

        if (freeForL2Table >(address) 0x0) {
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

BOOLEAN isProcessPage(address pageAddress) {
    address intVecsPageStart = (address)((((unsigned int)&intvecsStart) >> 12) << 12);
    return !(((pageAddress >= (address)INT_RAM_START) && (pageAddress < m_firstFreeInIntRam))
        || ((pageAddress >= (address)EXT_DDR_START) && (pageAddress < m_firstFreeInExtDDR))
        || ((pageAddress >= (address)ROM_INTERRUPT_ENTRIES) && (pageAddress < (address)(ROM_INTERRUPT_ENTRIES + ROM_INTERRUPT_LENGTH)))
        || ((pageAddress >= intVecsPageStart) && (pageAddress < (address)(INT_RAM_START + INT_RAM_SIZE))));
}

void switchToKernelMMU() {
    setMasterTablePointerTo(m_kernelProcess.masterTableAddress);
}

void initMemoryForProcess(process_t* process) {
  unsigned int startAddress  = NULL;
  address newPage = NULL;
    process_t* savedTaskPointer = m_process[process->pid];

    if (savedTaskPointer == NULL) {
        process->masterTableAddress = createMasterTable();

        mapOneToOne(process->masterTableAddress, (address)ROM_INTERRUPT_ENTRIES, ROM_INTERRUPT_LENGTH);
        mapOneToOne(process->masterTableAddress, (address)INT_RAM_START, (unsigned int)m_firstFreeInIntRam - INT_RAM_START);
        mapOneToOne(process->masterTableAddress, &intvecsStart, 0x3B);
        mapOneToOne(process->masterTableAddress, (address)EXT_DDR_START, (unsigned int)m_firstFreeInExtDDR - EXT_DDR_START);

        //task->messageQueueAddress = createMappedPage(task->masterTableAddress, (address)MESSAGE_QUEUE_VIRTUAL_ADDRESS);

        // Fake loader
        startAddress = process->pcb.restart_address;
        newPage = createMappedPage(process->masterTableAddress, (address)startAddress);

        process->codeLocation = process->codeLocation + ((startAddress - TASK_MEMORY_START) / 4);
        // load needed instructions into new page
       memcpy((void*)newPage, (void*)(process->codeLocation), 4096);

        setMasterTablePointerTo(process->masterTableAddress);

        m_process[process->pid] = process;
    } else {
        setMasterTablePointerTo(savedTaskPointer->masterTableAddress);
    }
    m_currentProcess = process;
}

void deleteProcessMemory(process_t* process) {
  unsigned int masterTableEntry = NULL;
  unsigned int l2TableEntry = NULL;
  int pageNumber = NULL;
    address masterTableAddress = process->masterTableAddress;
    if (masterTableAddress != 0x0) {
        enum MemoryType type;

        for (masterTableEntry = 0; masterTableEntry < 4096; masterTableEntry++) {
            address l2TableAddress = (address)(((*(masterTableAddress + masterTableEntry)) >> 10) << 10);
            if (l2TableAddress != 0x0) {
                for (l2TableEntry = 0; l2TableEntry < 256; l2TableEntry++) {
                    address pageAddress = (address)(((*(l2TableAddress + l2TableEntry)) >> 12) << 12);
                    if ((pageAddress != 0x0) && (isProcessPage(pageAddress))) {

                        pageNumber = pageForAddress(&type, (unsigned int)pageAddress);
                        releasePages(type, pageNumber, 1);
                    }
                    *(l2TableAddress + l2TableEntry) = 0x0;
                }
                 pageNumber =pageForAddress(&type, (unsigned int)l2TableAddress);
                releasePages(type, pageNumber, 1);
                *(masterTableAddress + masterTableEntry) = 0x0;
            }
        }
        pageNumber = pageForAddress(&type, (unsigned int)masterTableAddress);
        releasePages(type, pageNumber, 4);
    }
    m_process[process->pid] = NULL;
}


BOOLEAN handlePrefetchAbort() {

    switchToKernelMMU();
    process_delete();
    return TRUE;
}

BOOLEAN handleDataAbort() {
  BOOLEAN doContextSwitch = FALSE;
  unsigned int accessedAddress = NULL;
    asm("\t MRC p15, #0, r0, c6, c0, #0\n"); // Read data fault address register
    asm("\t LDR r1, tempVariableForAsmAndC\n");
    asm("\t STR r0, [r1]\n");
    // TODO check for read / write permissions


    accessedAddress = tempVariableForAsmAndC;

    if ((accessedAddress % 0x4 == 0x0) && (accessedAddress >= TASK_MEMORY_START) && (accessedAddress < TASK_MEMORY_END)) {
        process_t* currentProcess = m_currentProcess;
        switchToKernelMMU();
        createMappedPage(currentProcess->masterTableAddress, (address)accessedAddress);
        initMemoryForProcess(currentProcess);
        doContextSwitch = FALSE;
    } else {

        switchToKernelMMU();
        process_delete();
        doContextSwitch = TRUE;
    }
    return doContextSwitch;
}
