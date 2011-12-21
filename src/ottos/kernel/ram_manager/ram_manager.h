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



enum MemoryType {
    INT_RAM, EXT_DDR
};


        void reservePage(enum MemoryType mem, int pageNumber);
        void releasePage(enum MemoryType mem, int pageNumber);
        BOOLEAN  isOccupied(enum MemoryType mem, int pageNumber);

        address findFreeMemoryin(enum MemoryType mem, int nrOfPages, BOOLEAN align, BOOLEAN  reserve);
        address findFreeMemory(int nrOfPages, BOOLEAN  align, BOOLEAN  reserve);
        int pageForAddress(enum MemoryType* type, unsigned int memAddress);
        address addressOfPage(enum MemoryType mem, int pageNumberInMemory);

        void reservePages(enum MemoryType mem, int firstPageNumber, int nrOfPages);
        void releasePages(enum MemoryType mem, int firstPageNumber, int nrOfPages);

        int maxPagesIn(enum MemoryType mem);

#endif /* RAM_MANAGER_H_ */
