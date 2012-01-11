/* mmu.h
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

#ifndef MMU_H_
#define MMU_H_

#include <ottos/types.h>
#include <ottos/const.h>
#include "../pm/process.h"

#define MMU_PAGE_SIZE 0x1000 // 4 kB page
// Initialises the mmu and the ram_manager.
void mmu_init();

// Tells the mmu to switch to the kernel's master page
void mmu_switch_to_kernel();

// Tells the mmu to switch to the process' master page. If no master page exists, one will be created
void mmu_init_memory_for_process(process_t* process);

// Deletes the pages of the process
void mmu_delete_process_memory(process_t* process);

// Returns the physical address for the given process and virtual address
unsigned int mmu_get_physical_address(process_t* process, unsigned int  virtual_address);

// Handles the prefetch abort. Deletes the current active process and induces a context switch by returning TRUE
BOOLEAN mmu_handle_prefetch_abort();

// Handles the data abort. Creates mapped pages if necessary.
BOOLEAN mmu_handle_data_abort();

#endif /* MMU_H_ */
