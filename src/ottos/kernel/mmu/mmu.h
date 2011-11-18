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


void mmu_init();
void mmu_initMemoryForTask(int taskId);

#endif /* MMU_H_ */
