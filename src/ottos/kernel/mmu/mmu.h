/*
 * mmu.h
 *
 *  Created on: 10.11.2011
 *      Author: Thomas
 */

#ifndef MMU_H_
#define MMU_H_



#include <ottos/types.h>
#include <ottos/const.h>
#include <../kernel/pm/process.h>
#include <../kernel/ram_manager/ram_manager.h>


#define TASK_MEMORY_START 0x20000
#define TASK_MEMORY_END 0x1000000
#define MAX_PROCESS 256


        void initKernelMMU();
        void MMU_init();
        void enableMMU();
        void initDomainAccess();
        void setMasterTablePointerTo(address tableAddress);

        address createMasterTable();
        address createOrGetL2Table(address masterTableAddress, int masterTableEntryNumber);
        address createMappedPage(address masterTableAddress, address virtualAddress);
        void mapDirectly(address masterTableAddress, address virtualAddress, address physicalAddress);
        void mapOneToOne(address masterTableAddress, address startAddress, unsigned int length);

        void clearTLB();


        BOOLEAN isProcessPage(address pageAddress);
        int pageForAddress(enum MemoryType* type, unsigned int memAddress);
        address addressOfPage(enum MemoryType mem, int pageNumberInMemory);
        void reservePages(enum MemoryType mem, int firstPageNumber, int nrOfPages);
        void releasePages(enum MemoryType mem, int firstPageNumber, int nrOfPages);


        void switchToKernelMMU();
        void initMemoryForProcess(process_t* process);
        void deleteProcessMemory(process_t* process);



        BOOLEAN handlePrefetchAbort();
        BOOLEAN handleDataAbort();





#endif /* MMU_H_ */
