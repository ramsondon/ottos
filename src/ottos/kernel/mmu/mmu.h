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



#define MAX_MMU_TABLES 256
#define MAX_PAGES_IN_INT_RAM 16
#define MAX_PAGES_IN_EXT_DDR 65536

#define ROM_INTERRUPT_ENTRIES 0x14000
#define ROM_INTERRUPT_LENGTH 0x1C
#define INT_RAM_START 0x40200000
#define EXT_DDR_START 0x82000000

#define PROCESS_MEMORY_START 0x1000
#define PROCESS_MEMORY_END 0x1000000

enum MemoryType{INT_RAM, EXT_DDR};


void MMU_init();

static void enableMMU();
static void initDomainAccess();
static void setMasterTablePointerTo(address tableAddress);
static address createMasterTable();
static address createOrGetL2Table(address masterTableAddress, int masterTableEntryNumber);
static void createMappedPage(address masterTableAddress, address virtualAddress);
static void mapOneToOne(address masterTableAddress, address startAddress, unsigned int length);
static void clearTLB();
static address addressOfPage(enum MemoryType mem, int pageNumberInMemory);
static void reservePages(enum MemoryType mem, int firstPageNumber, int nrOfPages);
static  void releasePages(enum MemoryType mem, int firstPageNumber, int nrOfPages);
static  address findFreeMemory(int nrOfPages, BOOLEAN align, BOOLEAN reserve);


void initMemoryForTask(int taskId);
void loadPage(int pageNumber);
void prepagePagesFor(int serviceId);
address parameterAddressFor(int serviceId);
extern void handlePrefetchAbort();
extern void handleDataAbort();



#endif /* MMU_H_ */
