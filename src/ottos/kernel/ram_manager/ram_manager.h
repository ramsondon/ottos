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
#include <../kernel/pm/process.h>

#define MAX_PAGES_IN_INT_RAM 16
#define MAX_PAGES_IN_EXT_DDR 8192

#define INT_RAM_START 0x40200000
#define INT_RAM_SIZE  0x00010000

#define EXT_DDR_START 0x82000000
#define EXT_DDR_SIZE  0x02000000

#define ROM_INTERRUPT_ENTRIES 0x14000
#define ROM_INTERRUPT_LENGTH 0x1C

enum memory_type {
  INT_RAM, EXT_DDR
};

// TODO (thomas.bargetz@gmail.com) move this function!
BOOLEAN readBit(address number, int bitOffset);

void ram_manager_init();

void ram_manager_reserve_page(enum memory_type mem, int pageNumber);
void ram_manager_release_page(enum memory_type mem, int pageNumber);

address ram_manager_find_free_memory_in(enum memory_type mem, int nrOfPages,
                                        BOOLEAN align, BOOLEAN reserve);
address ram_manager_find_free_memory(int nrOfPages, BOOLEAN align,
                                     BOOLEAN reserve);

int ram_manager_page_for_address(enum memory_type* type,
                                 unsigned int memAddress);
address ram_manager_address_of_page(enum memory_type mem,
                                    int pageNumberInMemory);

void ram_manager_reserve_pages(enum memory_type mem, int firstPageNumber,
                               int nrOfPages);
void ram_manager_release_pages(enum memory_type mem, int firstPageNumber,
                               int nrOfPages);

int ram_manager_max_pages_in(enum memory_type mem);

#endif /* RAM_MANAGER_H_ */
