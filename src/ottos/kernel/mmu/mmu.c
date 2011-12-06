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
 *  Created on: 6 Dec 2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <stdlib.h>

#include <ottos/const.h>
#include <ottos/types.h>

#include "../pm/process.h"
#include "mmu.h"

#define MAX_MMU_TABLES 256
#define MAX_PAGES_IN_INT_RAM 16
#define MAX_PAGES_IN_EXT_DDR 65536

#define ROM_INTERRUPT_ENTRIES 0x14000
#define INT_RAM_START 0x40200000
#define EXT_DDR_START 0x82000000

#define PROCESS_MEMORY_START 0x1000
#define PROCESS_MEMORY_END 0x1000000

int occupied_pages_int_ram[MAX_PAGES_IN_INT_RAM];
int occupied_pages_ext_ddr[MAX_PAGES_IN_EXT_DDR];

unsigned int* mmu_process_master_table_address[MAX_MMU_TABLES];
extern unsigned int* process_master_table_address;

unsigned int* first_free_in_int_ram;
unsigned int* first_free_in_ext_ddr;

extern unsigned int kernel_master_table;
extern unsigned int int_ram_start;
extern unsigned int ext_ddr_start;
extern unsigned int intvecs_start;

pid_t active_process;

void mmu_init() {
  int i;
  int max = 0;

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
      occupied_pages_ext_ddr[i] = FALSE;
    }
    if (i < MAX_PAGES_IN_INT_RAM) {
      occupied_pages_int_ram[i] = FALSE;
    }
    if (i < MAX_MMU_TABLES) {
      mmu_process_master_table_address[i] = 0x0;
    }
  }
}

void mmu_init_domain_access() {
  // Set Domain Access control register to 0101 0101 0101 0101 0101 0101 0101 0111
  asm(" MOV     R0, #0x5557");
  asm(" MOVT    R0, #0x5555");
  asm(" MCR     P15, #0, r0, c3, c0, #0");
}

void mmu_clear_tlb() {
  asm(" MOV     R0, #0x0");
  asm(" MCR     P15, #0, r0, c8, c7, #0");
}

void mmu_set_master_table_pointer_to(unsigned int* table_address) {
  unsigned int temp_address;

  process_master_table_address = table_address;
  temp_address = (unsigned int) process_master_table_address & 0xFFFFC000;
  process_master_table_address = (unsigned int*) temp_address;

  asm(" LDR     R1, process_master_table_address");
  asm(" LDR     R1, [R1]");
  asm(" MCR     P15, #0, r1, c2, c0, #0");

  mmu_clear_tlb();
}

void mmu_enable() {
  asm(" MRC     P15, #0, r0, c1, c0, #0");
  asm(" ORR     R0, R0, #0x1");
  asm(" MCR     P15, #0, r0, c1, c0, #0");
}

void mmu_reserve_pages(enum MemoryType type, int first_page_number,
                       int number_of_pages) {
  int i;
  for (i = first_page_number; i < (first_page_number + number_of_pages); i++) {
    switch (type) {
      case INT_RAM:
        occupied_pages_int_ram[i] = TRUE;
        break;
      case EXT_DDR:
        occupied_pages_ext_ddr[i] = TRUE;
        break;
    }
  }
}

unsigned int* mmu_find_free_memory(int number_of_pages, int align, int reserve) {
  return NULL;
}

unsigned int* mmu_create_master_table(){
  unsigned int* master_table_address = mmu_find_free_memory(4, TRUE, TRUE);
  memset((void*)master_table_address, 0x0000, 4096 * 4);
  return master_table_address;
}

void mmu_init_memory_for_process(pid_t pid) {
  unsigned int number_of_kernel_pages;
  unsigned int* table_address;
  unsigned int* saved_table_address = mmu_process_master_table_address[pid];

  if (saved_table_address == (address_t) 0x0) {
    // TODO (thomas.bargetz@gmail.com) pid = 0 == kernel?
    if (pid == 0) {
      unsigned int i;
      first_free_in_int_ram = &int_ram_start;
      first_free_in_ext_ddr = &ext_ddr_start;

      // reserve kernel memory
      number_of_kernel_pages =
          (((unsigned int) first_free_in_int_ram - INT_RAM_START) / 0x1000);
      if (((unsigned int) first_free_in_int_ram - INT_RAM_START) % 0x1000 > 0) {
        number_of_kernel_pages++;
      }
      mmu_reserve_pages(INT_RAM, 0, number_of_kernel_pages);
      mmu_reserve_pages(INT_RAM, 15, 1);

      number_of_kernel_pages = (((unsigned int) first_free_in_ext_ddr
          - EXT_DDR_START) / 0x1000);
      if (((unsigned int) first_free_in_ext_ddr - EXT_DDR_START) % 0x1000 > 0) {
        number_of_kernel_pages++;
      }
      mmu_reserve_pages(EXT_DDR, 0, number_of_kernel_pages);

      mmu_init_domain_access();

      table_address = &kernel_master_table;
      mmu_set_master_table_pointer_to(table_address);
      // intialize master table
      for (i = 0x00000000; i < 0xFFF00000; i += 0x00100000) {
        *table_address = i | 0xC12;
        table_address++;
      }
      *table_address = 0xFFF00C12;

      mmu_enable();
      //lock_first_tlb_entry();
    } else {
      process_master_table_address = mmu_create_master_table();

      mmu_map_one_to_one(process_master_table_address,
                         (unsigned int*) ROM_INTERRUPT_ENTRIES, 0x1c);
      mmu_map_one_to_one(process_master_table_address,
                         (unsigned int*) INT_RAM_START,
                         (unsigned int) first_free_in_int_ram - INT_RAM_START);
      mmu_map_one_to_one(process_master_table_address, &intvecs_start, 0x3B);
      mmu_map_one_to_one(process_master_table_address,
                         (unsigned int*) EXT_DDR_START,
                         (unsigned int) first_free_in_ext_ddr - EXT_DDR_START);

      mmu_set_master_table_pointer_to(process_master_table_address);
    }
    mmu_process_master_table_address[pid] = process_master_table_address;
  } else {
    mmu_set_master_table_pointer_to(saved_table_address);
  }
  active_process = pid;
}

void mmu_load_page(int page_nr) {

}

void mmu_handle_prefetch_abort() {

}

void mmu_handle_data_abort() {

}
