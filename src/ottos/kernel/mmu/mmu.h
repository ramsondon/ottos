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
#include "../pm/process.h"


#define PROCESS_MEMORY_START 0x20000
#define PROCESS_MEMORY_END 0x1000000

#define MMU_PAGE_SIZE 0x1000 // 4 kB page

void mmu_init();
void mmu_switch_to_kernel();
void mmu_init_memory_for_process(process_t* process);
void mmu_delete_process_memory(process_t* process);
BOOLEAN mmu_handle_prefetch_abort();
BOOLEAN mmu_handle_data_abort();





#endif /* MMU_H_ */
