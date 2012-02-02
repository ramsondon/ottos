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

#include <ottos/kernel.h>
#include <ottos/io.h>
#include <ottos/memory.h>

#include "../mmu/mmu.h"
#include "ram_manager.h"

#define ENTRY_SIZE 8 // 8 bit
BOOLEAN occupied_pages_int_RAM[RAM_MANAGER_MAX_PAGES_IN_INT_RAM / ENTRY_SIZE];
BOOLEAN occupied_pages_ext_DDR[RAM_MANAGER_MAX_PAGES_IN_EXT_DDR / ENTRY_SIZE];

// debug
static int allocated_pages_int = 0;
static int allocated_pages_ext = 0;

void ram_manager_init() {
	int i;
	int max = 0;
	int entry_size = sizeof(BOOLEAN) * ENTRY_SIZE; // size in bit

	if (RAM_MANAGER_MAX_PAGES_IN_EXT_DDR > RAM_MANAGER_MAX_PAGES_IN_INT_RAM) {
		max = RAM_MANAGER_MAX_PAGES_IN_EXT_DDR / entry_size;
	} else {
		max = RAM_MANAGER_MAX_PAGES_IN_INT_RAM / entry_size;
	}

	// initialize fields
	for (i = 0; i < max; i++) {
		if (i < (RAM_MANAGER_MAX_PAGES_IN_EXT_DDR / entry_size)) {
			occupied_pages_ext_DDR[i] = 0;
		}
		if (i < (RAM_MANAGER_MAX_PAGES_IN_INT_RAM / entry_size)) {
			occupied_pages_int_RAM[i] = 0;
		}
	}
}

void ram_manager_reserve_page(enum ram_manager_memory_type mem, int page_number) {
	int entry_number = (page_number / (sizeof(address) * ENTRY_SIZE));
	int bit_number = (page_number % (sizeof(address) * ENTRY_SIZE));

	if ((mem == INT_RAM) && (page_number < RAM_MANAGER_MAX_PAGES_IN_INT_RAM)) {
		SET_BIT((((address)occupied_pages_int_RAM) + entry_number), bit_number);
		allocated_pages_int++;
	} else if ((mem == EXT_DDR) && (page_number < RAM_MANAGER_MAX_PAGES_IN_EXT_DDR)) {
		SET_BIT((((address)occupied_pages_ext_DDR) + entry_number), bit_number);
		allocated_pages_ext++;
	}
}

BOOLEAN ram_manager_is_occupied(enum ram_manager_memory_type mem, int page_number) {
	int entry_number = (page_number / (sizeof(address) * ENTRY_SIZE));
	int bit_number = (page_number % (sizeof(address) * ENTRY_SIZE));

	if ((mem == INT_RAM) && (page_number < RAM_MANAGER_MAX_PAGES_IN_INT_RAM)) {
		return READ_SINGLE_BIT((((address) occupied_pages_int_RAM) + entry_number), bit_number);
	} else if ((mem == EXT_DDR) && (page_number < RAM_MANAGER_MAX_PAGES_IN_EXT_DDR)) {
		return READ_SINGLE_BIT((((address) occupied_pages_ext_DDR) + entry_number), bit_number);
	}

	return FALSE;
}

void ram_manager_release_page(enum ram_manager_memory_type mem, int pageNumber) {
	int entry_number = (pageNumber / (sizeof(address) * ENTRY_SIZE));
	int bit_number = (pageNumber % (sizeof(address) * ENTRY_SIZE));

	if ((mem == INT_RAM) && (entry_number < RAM_MANAGER_MAX_PAGES_IN_INT_RAM)) {
		CLEAR_BIT((address)(((address)occupied_pages_int_RAM) + entry_number), bit_number);
		allocated_pages_int--;
	} else if ((mem == EXT_DDR) && (entry_number < RAM_MANAGER_MAX_PAGES_IN_EXT_DDR)) {
		CLEAR_BIT((address)(((address)occupied_pages_ext_DDR) + entry_number), bit_number);
		allocated_pages_ext--;
	}
}

int ram_manager_page_for_address(enum ram_manager_memory_type* type, unsigned int mem_address) {
	if ((mem_address >= RAM_MANAGER_INT_RAM_START) && (mem_address < RAM_MANAGER_INT_RAM_START + RAM_MANAGER_INT_RAM_SIZE)) {
		*type = INT_RAM;
		return (mem_address - RAM_MANAGER_INT_RAM_START) / MMU_PAGE_SIZE;
	} else if ((mem_address >= RAM_MANAGER_EXT_DDR_START) && (mem_address < RAM_MANAGER_EXT_DDR_START + RAM_MANAGER_EXT_DDR_SIZE)) {
		*type = EXT_DDR;
		return (mem_address - RAM_MANAGER_EXT_DDR_START) / MMU_PAGE_SIZE;
	}

	return -1;
}

address ram_manager_address_of_page(enum ram_manager_memory_type mem, int page_number_in_memory) {
	switch (mem) {
	case INT_RAM:
		return (address) (RAM_MANAGER_INT_RAM_START + (page_number_in_memory * MMU_PAGE_SIZE));
	case EXT_DDR:
		return (address) (RAM_MANAGER_EXT_DDR_START + (page_number_in_memory * MMU_PAGE_SIZE));
	default:
		return NULL;
	}
}

void ram_manager_reserve_pages(enum ram_manager_memory_type mem, int first_page_number, int nr_of_pages) {
	int i;

	for (i = first_page_number; i < (first_page_number + nr_of_pages); i++) {
		ram_manager_reserve_page(mem, i);
	}
}
void ram_manager_release_pages(enum ram_manager_memory_type mem, int first_page_number, int nr_of_pages) {
	int i;

	for (i = first_page_number; i < (first_page_number + nr_of_pages); ++i) {
		ram_manager_release_page(mem, i);
		memset((void*) ram_manager_address_of_page(mem, i), 0, MMU_PAGE_SIZE);
	}
}

int ram_manager_max_pages_in(enum ram_manager_memory_type mem) {
	switch (mem) {
	case INT_RAM:
		return RAM_MANAGER_MAX_PAGES_IN_INT_RAM;
	case EXT_DDR:
		return RAM_MANAGER_MAX_PAGES_IN_EXT_DDR;
	default:
		return -1;
	}
}

double ram_manager_mem_alloc_intram() {
  return allocated_pages_int * MMU_PAGE_SIZE;
}

double ram_manager_mem_alloc_extddr() {
  return allocated_pages_ext * MMU_PAGE_SIZE;
}

double ram_manager_mem_total_intram() {
  return ram_manager_max_pages_in(INT_RAM) * MMU_PAGE_SIZE;
}

double ram_manager_mem_total_extddr() {
  return ram_manager_max_pages_in(EXT_DDR) * MMU_PAGE_SIZE;
}

void ram_manager_print_mem_usage() {
  char message[256];
  char buffer0[10];
  char buffer1[10];
  char buffer2[10];
  char buffer3[10];

  memory_bytes_readable(allocated_pages_int * MMU_PAGE_SIZE, buffer0);
  memory_bytes_readable(ram_manager_max_pages_in(INT_RAM) * MMU_PAGE_SIZE, buffer1);
  memory_bytes_readable(allocated_pages_ext * MMU_PAGE_SIZE, buffer2);
  memory_bytes_readable(ram_manager_max_pages_in(EXT_DDR) * MMU_PAGE_SIZE, buffer3);

  sprintf(message, "Memory usage: %s (INT max: %s), %s (EXT max: %s)",
          buffer0, buffer1, buffer2, buffer3);
  kernel_debug(17, message);
}

address ram_manager_find_free_memory_in(enum ram_manager_memory_type mem, int nr_of_pages, BOOLEAN align, BOOLEAN reserve) {
	int i = 0;
	address result = NULL;
	int free_pages = 0;

	for (i = 0; (i < ram_manager_max_pages_in(mem)) && (result == 0); i++) {
		if ((!ram_manager_is_occupied(mem, i)) && ((!align) || ((free_pages > 0) || ((i % nr_of_pages) == 0)))) {
			free_pages++;
			if (free_pages == nr_of_pages) {
				result = ram_manager_address_of_page(mem, (i - nr_of_pages) + 1);
				if (reserve == TRUE) {
					ram_manager_reserve_pages(mem, (i - nr_of_pages) + 1, nr_of_pages);
				}
			}
		} else {
			free_pages = 0;
		}
	}

	// debug
	ram_manager_print_mem_usage();

	return result;
}

address ram_manager_find_free_memory(int nr_of_pages, BOOLEAN align, BOOLEAN reserve) {
	address result = ram_manager_find_free_memory_in(INT_RAM, nr_of_pages, align, reserve);

	if (result == NULL) {
		result = ram_manager_find_free_memory_in(EXT_DDR, nr_of_pages, align, reserve);
	}

	return result;
}

