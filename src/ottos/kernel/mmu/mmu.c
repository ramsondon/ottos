/* mmu.c
 *
 * Copyright (c) 2011 The ottos project. Inspired by the BOSS project.
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
#include <cstring>
#include <ottos/memory.h>
#include "../ram_manager/ram_manager.h"

#include "fault_status_flags.h"
#include "mmu.h"

asm(" .bss _process_table_master_address, 4");
asm(" .bss _accessed_address, 4");
asm(" .bss _fault_state, 4");

asm(" .global _process_table_master_address");
asm(" .global _accessed_address");
asm(" .global _fault_state");

asm("process_table_master_address .field _process_table_master_address, 32");
asm("accessed_address .field _accessed_address, 32");
asm("fault_state .field _fault_state, 32");

extern address process_table_master_address;
extern unsigned int accessed_address;
extern unsigned int fault_state;
extern volatile unsigned int kernel_master_table;
extern volatile unsigned int int_RAM_start;
extern volatile unsigned int ext_DDR_start;
extern volatile unsigned int intvecs_start;

address first_free_in_int_RAM;
address first_free_in_ext_DDR;

#define MMU_FIRST_PAGE_NUMBER_FOR_INTVECS 15 // the first page number for the intvecs
#define MMU_MAX_PROCESS_SPACE 0xFFF00000 // 4 GB space for each process
#define MMU_SECTION_ENTRY_SIZE 0x00100000 // 1 MB for each section entry
// SECTION_ENTRY: SECTION_BASE_ADDRESS SBZ AP SBZ DOMAIN IMP CACHE(C) BUFFER(B)  1 0
// SECTION_BASE_ADDRESS = 000000000000
// SBZ = 00000000
// AP = 11 ... read write access for user and privileged mode
// SBZ = 0
// DOMAIN = 0000 ... kernel has the first domain
// IMP = 1 ... ? (ask armin simma)
// C = 0
// B = 0
#define MMU_SECTION_ENTRY_INITIAL_VALUE_FOR_KERNEL 0xC12
#define MMU_ALIGN_ADDRESS 0xFFFFC000
#define MMU_INTVECTS_SIZE 0x3B
#define MMU_MASTER_TABLE_SIZE MMU_PAGE_SIZE * 4
#define MMU_L2_ENTRY_COUNT 256
#define MMU_L2_ENTRY_SIZE 4 // 4 byte
address mmu_create_master_table();
void mmu_map_one_to_one(address master_table_address, address start_address,
                        unsigned int length, unsigned char domain_number);
address mmu_create_or_get_l2_table(address master_table_address,
                                   int master_table_entry_number,
                                   unsigned char domain_number);
address mmu_create_mapped_page(address master_table_address,
                               address virtual_address,
                               unsigned char domain_number);
void mmu_map_directly(address master_table_address, address virtual_address,
                      address physical_address, unsigned char domain_numer);
BOOLEAN mmu_is_process_page(address page_address);
void mmu_set_master_table_pointer_to(address table_address);

void mmu_init() {
  unsigned int i;
  unsigned int nrOfKernelPages;
  address table_address;
  process_table_master_address = 0;

  // intialise ram manager
  ram_manager_init();

  first_free_in_int_RAM = &int_RAM_start;
  first_free_in_ext_DDR = &ext_DDR_start;

  // *** initialise MMU ***
  // reserve kernel memory
  // determine the number of kernel pages in the internal ram
  nrOfKernelPages = (((unsigned int) first_free_in_int_RAM - INT_RAM_START)
      / MMU_PAGE_SIZE);

  // if there is some space left, we space for one more kernel page
  if (((unsigned int) first_free_in_int_RAM - INT_RAM_START) % MMU_PAGE_SIZE
      > 0) {
    nrOfKernelPages++;
  }
  // reserve the space for the determined number of kernel pages
  ram_manager_reserve_pages(INT_RAM, 0, nrOfKernelPages);
  ram_manager_reserve_pages(INT_RAM, MMU_FIRST_PAGE_NUMBER_FOR_INTVECS, 1);

  // determine the number of kernel pages in the external ram
  nrOfKernelPages = (((unsigned int) first_free_in_ext_DDR - EXT_DDR_START)
      / MMU_PAGE_SIZE);
  // if there is some space left, we space for one more kernel page
  if (((unsigned int) first_free_in_ext_DDR - EXT_DDR_START) % MMU_PAGE_SIZE
      > 0) {
    nrOfKernelPages++;
  }
  // reserve the space for the determined number of kernel pages
  ram_manager_reserve_pages(EXT_DDR, 0, nrOfKernelPages);

  // *** initialise the domain access ***
  // Set Domain Access control register to 0101 0101 0101 0101 0101 0101 0101 0111
  asm(" MOV   R0, #0x5557");
  asm(" MOVT  R0, #0x5555");
  asm(" MCR   P15, #0, R0, C3, C0, #0");

  table_address = &kernel_master_table;
  // i dont think we need this
  //m_kernelProcess.masterTableAddress = tableAddress;

  // *** initialise master table ***
  mmu_set_master_table_pointer_to(table_address);
  for (i = 0x00000000; i < MMU_MAX_PROCESS_SPACE; i += MMU_SECTION_ENTRY_SIZE) {
    *table_address = i | MMU_SECTION_ENTRY_INITIAL_VALUE_FOR_KERNEL;
    table_address++;
  }
  *table_address = 0xFFF00C12; //MMU_SECTION_ENTRY_INITIAL_VALUE_FOR_KERNEL;

  // *** enable the MMU ***
  // set internal MMU from OMAP 3530 on
  asm(" MRC   P15, #0, R0, C1, C0, #0");
  asm(" ORR   R0, R0, #0x1");
  asm(" MCR   P15, #0, R0, C1, C0, #0");
}

void mmu_set_master_table_pointer_to(address table_address) {
  // unsigned int temp_address;

  //process_table_master_address = table_address;
  //// align the given table_address (set the last 12 bits to zero)
  //temp_address = (unsigned int) process_table_master_address & MMU_ALIGN_ADDRESS;
  //process_table_master_address = (address) temp_address;

  // align the given table_address (set the last 12 bits to zero)
  process_table_master_address = (address) ((unsigned int) table_address
      & MMU_ALIGN_ADDRESS);

  asm(" LDR   R1, process_table_master_address");
  asm(" LDR   R1, [R1]");
  asm(" MCR   P15, #0, R1, C2, C0, #0");

  // clear the TLB
  asm(" MOV   R0, #0");
  asm(" MCR   P15, #0, R0, C8, C7, #0");
}

void mmu_init_memory_for_process(process_t* process) {
  int i = 0;

  if (process->master_table_address != NULL) {
    mmu_set_master_table_pointer_to(process->master_table_address);
    return;
  }
  //if (savedTaskPointer == NULL) {

  process->master_table_address = mmu_create_master_table();

  mmu_map_one_to_one(process->master_table_address,
                     (address) ROM_INTERRUPT_ENTRIES, ROM_INTERRUPT_LENGTH, 0);
  mmu_map_one_to_one(process->master_table_address, (address) INT_RAM_START,
                     (unsigned int) first_free_in_int_RAM - INT_RAM_START, 0);
  mmu_map_one_to_one(process->master_table_address, &intvecs_start,
                     MMU_INTVECTS_SIZE, 0);
  mmu_map_one_to_one(process->master_table_address, (address) EXT_DDR_START,
                     (unsigned int) first_free_in_ext_DDR - EXT_DDR_START, 0);

  //task->memoryManager = (MemoryManager*)createMappedPage(task->masterTableAddress, (address)MESSAGE_QUEUE_VIRTUAL_ADDRESS, true, true);
  //MemoryManager::getInstanceAt((address)task->memoryManager);

  //mapHardwareRegisters(task);

  // map process code
  for (i = 0; i < process->page_count; i++) {
    // The next virtual address is the first address plus 4096 bytes (4KB)
    address virtual_address = (address) (process->pcb.restart_address + (i
        * MMU_PAGE_SIZE));
    // The next physical address is the first address plus 4096 bytes (4KB)
    address physical_address =
        (address) (((unsigned int) (process->code_location)) + (i
            * MMU_PAGE_SIZE));
    mmu_map_directly(process->master_table_address, virtual_address,
                     physical_address, 0);
  }

  mmu_set_master_table_pointer_to(process->master_table_address);

  //m_process[process->pid] = process;
  /*
   } else {
   mmu_set_master_table_pointer_to(savedTaskPointer->masterTableAddress);
   }*/
  //m_currentProcess = process;
}

address mmu_create_master_table() {
  address masterTableAddress;

  masterTableAddress = ram_manager_find_free_memory(4, TRUE, TRUE);
  memset((void*) masterTableAddress, 0, MMU_MASTER_TABLE_SIZE);

  return masterTableAddress;
}

void mmu_map_one_to_one(address master_table_address, address start_address,
                        unsigned int length, unsigned char domain_number) {
  int i;
  // Each entry maps 4096 bytes = 0x1000
  for (i = 0; i < length; i += 0x1000) {
    // Map the next page. because startAddress is of type address, it adds 4 * i automatically => take i / 4
    mmu_map_directly(master_table_address, start_address + (i / 4),
                     start_address + (i / 4), domain_number);
  }
}

// returns the l2 table address
address mmu_create_or_get_l2_table(address master_table_address,
                                   int master_table_entry_number,
                                   unsigned char domain_number) {

  address result;
  unsigned int table_entry;

  if (master_table_entry_number < MMU_PAGE_SIZE) {
    // check if a l2 table exists
    if (*(master_table_address + master_table_entry_number) == 0) {
      // initialise l2 table
      result = ram_manager_find_free_memory(1, TRUE, TRUE);

      // create a cross page table
      table_entry = (unsigned int) result | 0x00000011;
      // Set the domain
      table_entry |= (domain_number << 5);

      *(master_table_address + master_table_entry_number) = table_entry;

      memset((void*) result, 0, MMU_L2_ENTRY_COUNT * MMU_L2_ENTRY_SIZE); // 256 entries * 4 bytes per entry = 1024 bytes
    } else {
      // get the l2 table
      // delete the access flags
      result = (address) ((*(master_table_address + master_table_entry_number)
          >> 10) << 10);
    }
  }

  return result;
}

address mmu_create_mapped_page(address master_table_address,
                               address virtual_address,
                               unsigned char domain_number) {
  address new_page;

  new_page = ram_manager_find_free_memory(1, TRUE, TRUE);
  memset((void*) new_page, 0, MMU_PAGE_SIZE);
  mmu_map_directly(master_table_address, virtual_address, new_page,
                   domain_number);

  return new_page;
}

unsigned char mmu_get_access_flag_for_domain(unsigned char domain_number) {
  unsigned char result = 0;
  switch (domain_number) {
    case 0:
      result = 3;
      break;
    default:
      result = 1;
      break;
  }
  return result;
}

void mmu_map_directly(address master_table_address, address virtual_address,
                      address physical_address, unsigned char domain_number) {

  unsigned int masterTableEntryNumber = (unsigned int) virtual_address >> 20;
  address l2_table_address = mmu_create_or_get_l2_table(master_table_address,
                                                        masterTableEntryNumber,
                                                        domain_number);

  if (l2_table_address != 0) {
    address page_address = (address) (((unsigned int) physical_address >> 12)
        << 12);

    unsigned int l2_table_entry_number = ((unsigned int) virtual_address >> 12)
        - (((unsigned int) virtual_address >> 12) & 0xFFF00);

    unsigned int table_entry = (unsigned int) page_address | 0x00000002;

    unsigned char access_flag = mmu_get_access_flag_for_domain(domain_number);
    // Set the AP bits
    table_entry |= (access_flag << 4);
    table_entry |= (access_flag << 6);
    table_entry |= (access_flag << 8);
    table_entry |= (access_flag << 10);

    *(l2_table_address + l2_table_entry_number) = table_entry;
  } else {
    // TODO Handle full memory
  }
}

void mmu_delete_process_memory(process_t* process) {
  unsigned int master_table_entry;
  unsigned int l2_table_entry;
  int page_number;
  address master_table_address = process->master_table_address;

  if (master_table_address != 0) {
    enum memory_type type;

    for (master_table_entry = 0; master_table_entry < MMU_PAGE_SIZE; master_table_entry++) {
      address l2_table_address = (address) (((*(master_table_address
          + master_table_entry)) >> 10) << 10);

      if (l2_table_address != 0) {
        // delete l2 table entries
        for (l2_table_entry = 0; l2_table_entry < MMU_L2_ENTRY_COUNT; l2_table_entry++) {
          address page_address = (address) (((*(l2_table_address
              + l2_table_entry)) >> 12) << 12);

          // if the page has been initialised and it's a process page, then release the page
          if ((page_address != 0) && (mmu_is_process_page(page_address))) {
            page_number
                = ram_manager_page_for_address(&type,
                                               (unsigned int) page_address);
            ram_manager_release_pages(type, page_number, 1);
          }

          // delete l2 table entry
          *(l2_table_address + l2_table_entry) = 0;
        }

        // delete l2 table?
        page_number
            = ram_manager_page_for_address(&type,
                                           (unsigned int) l2_table_address);
        ram_manager_release_pages(type, page_number, 1);

        // delete master table entry
        *(master_table_address + master_table_entry) = 0;
      }
    }

    // delete master table?
    page_number
        = ram_manager_page_for_address(&type,
                                       (unsigned int) master_table_address);
    ram_manager_release_pages(type, page_number, 4);
  }
}

BOOLEAN mmu_is_process_page(address page_address) {
  address intVecsPageStart = (address) ((((unsigned int) &intvecs_start) >> 12)
      << 12);

  // check if the page_address is in kernel space
  return !(((page_address >= (address) INT_RAM_START) && (page_address
      < first_free_in_int_RAM)) || ((page_address >= (address) EXT_DDR_START)
      && (page_address < first_free_in_ext_DDR)) || ((page_address
      >= (address) ROM_INTERRUPT_ENTRIES) && (page_address
      < (address) (ROM_INTERRUPT_ENTRIES + ROM_INTERRUPT_LENGTH)))
      || ((page_address >= intVecsPageStart) && (page_address
          < (address) (INT_RAM_START + INT_RAM_SIZE))));
}

void mmu_switch_to_kernel() {
  mmu_set_master_table_pointer_to(&kernel_master_table);
}

BOOLEAN mmu_is_legal(unsigned int accessed_address, unsigned int fault_status) {
  BOOLEAN result = FALSE;
  BOOLEAN write_access = readBit(&fault_status, 11); // 11'th bit
  unsigned int status_field = (fault_status & 0xF); // last 4 bit
  BOOLEAN sd_bit = readBit(&fault_status, 12); // 12'th bit
  BOOLEAN s_bit = readBit(&fault_status, 10); // 10'th bit
  status_field |= (sd_bit << 6);
  status_field |= (s_bit << 5);

  if (write_access == TRUE) {
    switch (status_field) {
      case PERMISSION_FAULT_SECTION:
      case PERMISSION_FAULT_PAGE:
        result = FALSE;
        break;
      case TRANSLATION_FAULT_SECTION:
      case TRANSLATION_FAULT_PAGE:
        result = (((accessed_address >= PROCESS_STACK_START)
            && (accessed_address < PROCESS_STACK_START + PROCESS_STACK_SIZE))
            || ((accessed_address >= PROCESS_SYSMEM_START) && (accessed_address
                < PROCESS_SYSMEM_START + PROCESS_SYSMEM_SIZE)));
        //|| ((accessed_address >= MESSAGE_QUEUE_VIRTUAL_ADDRESS) && (accessedAddress < MESSAGE_QUEUE_VIRTUAL_ADDRESS + MESSAGE_QUEUE_SIZE)));
        break;
      case DEBUG_EVENT:
        result = TRUE;
        break;
      default:
        result = FALSE;
        break;
    }
  } else {
    result = FALSE;
    /*
     switch (status_field) {
     case TRANSLATION_FAULT_SECTION:
     case TRANSLATION_FAULT_PAGE:
     result = FALSE;//((accessed_address >= PROCESS_MEMORY_START) && (accessed_address < PROCESS_MEMORY_END));
     break;
     case DEBUG_EVENT:
     result = TRUE;
     break;
     default:
     result = FALSE;
     break;
     }
     */
  }

  return result;
}

// TODO (thomas.bargetz@gmail.com) not part of the MMU!


BOOLEAN mmu_handle_prefetch_abort() {

  asm("\t MRC p15, #0, R0, C6, C0, #2\n");
  asm("\t MRC p15, #0, R1, C5, C0, #1\n");

  mmu_switch_to_kernel();
  process_delete();

  return TRUE;
}

// TODO (thomas.bargetz@gmail.com) not part of the MMU!
BOOLEAN mmu_handle_data_abort() {
  BOOLEAN do_context_switch = FALSE;
  accessed_address = 0;
  fault_state = 0;

  asm(" MRC   P15, #0, R0, C6, C0, #0");
  // Read data fault address register
  asm(" LDR   R1, accessed_address");
  asm(" STR   R0, [R1]\n");
  // TODO check for read / write permissions

  // Get the abort status
  asm(" MRC p15, #0, r0, c5, c0, #0");
  asm(" LDR r1, fault_state");
  asm(" STR r0, [r1]");

  // TODO (thomas.bargetz@gmail.com) 0x4 the magic number appears again!
  if (mmu_is_legal(accessed_address, fault_state) == TRUE) {

    mmu_switch_to_kernel();
    mmu_create_mapped_page(process_table[process_active]->master_table_address,
                           (address) accessed_address, 0);
    mmu_init_memory_for_process(process_table[process_active]);
    do_context_switch = FALSE;
  } else {
    if (process_active != PID_INVALID) {
      mmu_switch_to_kernel();
      process_delete();
      do_context_switch = TRUE;
    }
  }
  return do_context_switch;
}
