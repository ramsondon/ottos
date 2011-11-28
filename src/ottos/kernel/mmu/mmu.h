/*
 * mmu.h
 *
 *  Created on: 10.11.2011
 *      Author: Thomas
 */

#ifndef MMU_H_
#define MMU_H_

#include <stdint.h>

#define MAX_TASKS 10
#define MAX_PAGES_IN_MEMORY 8192 // 32MB mapped to 4KB pages

void mmu_init();
void mmu_initMemoryForTask(int taskId);

void mmu_loadPage(int pageNumber);


#endif /* MMU_H_ */
