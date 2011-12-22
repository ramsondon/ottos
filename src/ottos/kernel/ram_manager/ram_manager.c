/* ram_manager.c
 * 
 * Copyright (c) 2011 The ottos_ project.
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
 *  Created on: 16.12.2011
 *      Author: Thomas Wiedemann <thomas.wiedemann@students.fhv.at>
 */
#include <cstring>
#include <bits.h>

#include "../mmu/mmu.h"

#include "ram_manager.h"

#define ENTRY_SIZE 8 // 8 bit

BOOLEAN occupied_pages_int_RAM[MAX_PAGES_IN_INT_RAM / 8];
BOOLEAN occupied_pages_ext_DDR[MAX_PAGES_IN_EXT_DDR / 8];

void ram_manager_init() {
  int i;

  // Initialize fields
  int entry_size = sizeof(BOOLEAN) * ENTRY_SIZE; // Size in bit
  int max = 0;

  if (MAX_PAGES_IN_EXT_DDR > MAX_PAGES_IN_INT_RAM) {
    max = MAX_PAGES_IN_EXT_DDR / entry_size;
  } else {
    max = MAX_PAGES_IN_INT_RAM / entry_size;
  }

  for (i = 0; i < max; i++) {
    if (i < (MAX_PAGES_IN_EXT_DDR / entry_size)) {
      occupied_pages_ext_DDR[i] = 0;
    }
    if (i < (MAX_PAGES_IN_INT_RAM / entry_size)) {
      occupied_pages_int_RAM[i] = 0;
    }
  }
}

void ram_manager_reserve_page(enum memory_type mem, int page_number) {
  int entryNumber = (page_number / (sizeof(address) * ENTRY_SIZE));
  int bitNumber = (page_number % (sizeof(address) * ENTRY_SIZE));

  if ((mem == INT_RAM) && (page_number < MAX_PAGES_IN_INT_RAM)) {
    SET_BIT((((address)occupied_pages_int_RAM) + entryNumber), bitNumber);
  } else if ((mem == EXT_DDR) && (page_number < MAX_PAGES_IN_EXT_DDR)) {
    SET_BIT((((address)occupied_pages_ext_DDR) + entryNumber), bitNumber);
  }
}

BOOLEAN readBit(address number, int bitOffset) {
    return (*(number) & (1 << bitOffset)) >> bitOffset;
}

BOOLEAN ram_manager_is_occupied(enum memory_type mem, int page_number) {
  int entry_number = (page_number / (sizeof(address) * ENTRY_SIZE));
  int bit_number = (page_number % (sizeof(address) * ENTRY_SIZE));
  BOOLEAN result = FALSE;

  if ((mem == INT_RAM) && (page_number < MAX_PAGES_IN_INT_RAM)) {
    result
        = /*READ_BIT*/readBit((((address)occupied_pages_int_RAM) + entry_number), bit_number);
  } else if ((mem == EXT_DDR) && (page_number < MAX_PAGES_IN_EXT_DDR)) {
    result
        = /*READ_BIT*/readBit((((address)occupied_pages_ext_DDR) + entry_number), bit_number);
  }

  return result;
}

void ram_manager_release_page(enum memory_type mem, int pageNumber) {
  int entry_number = (pageNumber / (sizeof(address) * ENTRY_SIZE));
  int bit_number = (pageNumber % (sizeof(address) * ENTRY_SIZE));

  if ((mem == INT_RAM) && (entry_number < MAX_PAGES_IN_INT_RAM)) {
    CLEAR_BIT((address)(((address)occupied_pages_int_RAM) + entry_number), bit_number);
  } else if ((mem == EXT_DDR) && (entry_number < MAX_PAGES_IN_EXT_DDR)) {
    CLEAR_BIT((address)(((address)occupied_pages_ext_DDR) + entry_number), bit_number);
  }
}

int ram_manager_page_for_address(enum memory_type* type, unsigned int mem_address) {
  int result = -1;

  if ((mem_address >= INT_RAM_START) && (mem_address < INT_RAM_START
      + INT_RAM_SIZE)) {
    result = (mem_address - INT_RAM_START) / MMU_PAGE_SIZE;
    *type = INT_RAM;
  } else if ((mem_address >= EXT_DDR_START) && (mem_address < EXT_DDR_START
      + EXT_DDR_SIZE)) {
    result = (mem_address - EXT_DDR_START) / MMU_PAGE_SIZE;
    *type = EXT_DDR;
  }

  return result;
}

address ram_manager_address_of_page(enum memory_type mem, int page_number_in_memory) {
  switch (mem) {
    case INT_RAM:
      return (address) (INT_RAM_START + (page_number_in_memory * MMU_PAGE_SIZE));
    case EXT_DDR:
      return (address) (EXT_DDR_START + (page_number_in_memory * MMU_PAGE_SIZE));
  }
  return NULL;
}
// // TODO (thomas.bargetz@gmail.com) firstPageNumber necessary?
void ram_manager_reserve_pages(enum memory_type mem, int first_page_number, int nr_of_pages) {
  int i;

  for (i = first_page_number; i < (first_page_number + nr_of_pages); i++) {
    ram_manager_reserve_page(mem, i);
  }
}
void ram_manager_release_pages(enum memory_type mem, int first_page_number, int nr_of_pages) {
  int i;

  for (i = first_page_number; i < (first_page_number + nr_of_pages); ++i) {
    ram_manager_release_page(mem, i);
    memset((void*) ram_manager_address_of_page(mem, i), 0, MMU_PAGE_SIZE);
  }
}

address ram_manager_find_free_memory_in(enum memory_type mem, int nr_of_pages, BOOLEAN align,
                         BOOLEAN reserve) {
  int i = NULL;
  address result = 0;
  int freePages = 0;

  for (i = 0; (i < ram_manager_max_pages_in(mem)) && (result == 0); i++) {
    if ((!ram_manager_is_occupied(mem, i)) && ((!align) || ((freePages > 0) || ((i
        % nr_of_pages) == 0)))) {
      freePages++;
      if (freePages == nr_of_pages) {
        result = ram_manager_address_of_page(mem, (i - nr_of_pages) + 1);
        if (reserve) {
          ram_manager_reserve_pages(mem, (i - nr_of_pages) + 1, nr_of_pages);
        }
      }
    } else {
      freePages = 0;
    }
  }

  return result;
}

address ram_manager_find_free_memory(int nr_of_pages, BOOLEAN align, BOOLEAN reserve) {
  address result = ram_manager_find_free_memory_in(INT_RAM, nr_of_pages, align, reserve);

  if (result == 0) {
    result = ram_manager_find_free_memory_in(EXT_DDR, nr_of_pages, align, reserve);
  }

  return result;
}

int ram_manager_max_pages_in(enum memory_type mem) {

  switch (mem) {
    case INT_RAM:
      return MAX_PAGES_IN_INT_RAM;
    case EXT_DDR:
      return MAX_PAGES_IN_EXT_DDR;
  }

  return -1;
}

