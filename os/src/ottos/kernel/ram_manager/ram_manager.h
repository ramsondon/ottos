/* ram_manager.h
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
#ifndef RAM_MANAGER_H_
#define RAM_MANAGER_H_

#include <ottos/types.h>
#include <ottos/const.h>
#include "../pm/process.h"
#include "../mmu/mmu.h"


#define RAM_MANAGER_INT_RAM_START 0x40200000
#define RAM_MANAGER_INT_RAM_SIZE  0x00010000

#define RAM_MANAGER_EXT_DDR_START 0x80000000
#define RAM_MANAGER_EXT_DDR_SIZE  0x03FFFFFF

#define RAM_MANAGER_MAX_PAGES_IN_INT_RAM 16
#define RAM_MANAGER_MAX_PAGES_IN_EXT_DDR RAM_MANAGER_EXT_DDR_SIZE / MMU_PAGE_SIZE

#define RAM_MANAGER_ROM_INTERRUPT_ENTRIES 0x14000
#define RAM_MANAGER_ROM_INTERRUPT_LENGTH 0x1C

enum ram_manager_memory_type {
	INT_RAM, EXT_DDR
};

// initializes the ram manager
void ram_manager_init();

// finds the next free memory location and returns the address of it
address ram_manager_find_free_memory(int nr_of_pages, BOOLEAN align, BOOLEAN reserve);

// returns the page number for address
int ram_manager_page_for_address(enum ram_manager_memory_type* type, unsigned int address);

// reserves pages
void ram_manager_reserve_pages(enum ram_manager_memory_type type, int first_page_number, int nr_of_pages);

// releases pages
void ram_manager_release_pages(enum ram_manager_memory_type type, int first_page_number, int nr_of_pages);

#endif /* RAM_MANAGER_H_ */
