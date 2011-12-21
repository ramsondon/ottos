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
#include "ram_manager.h"
#include "bits.h"

BOOLEAN occupiedPagesIntRam[MAX_PAGES_IN_INT_RAM / 8];
BOOLEAN occupiedPagesExtDDR[MAX_PAGES_IN_EXT_DDR / 8];

RAMManager() {
  int i = NULL;
  // Initialize fields
  int entrySize = sizeof(BOOLEAN) * 8; // Size in bit
  int max = 0;
  if (MAX_PAGES_IN_EXT_DDR > MAX_PAGES_IN_INT_RAM) {
    max = MAX_PAGES_IN_EXT_DDR / entrySize;
  } else {
    max = MAX_PAGES_IN_INT_RAM / entrySize;
  }

  for (i = 0; i < max; i++) {
    if (i < (MAX_PAGES_IN_EXT_DDR / entrySize)) {
      occupiedPagesExtDDR[i] = 0x0;
    }
    if (i < (MAX_PAGES_IN_INT_RAM / entrySize)) {
      occupiedPagesIntRam[i] = 0x0;
    }
  }
}

void reservePage(enum MemoryType mem, int pageNumber) {
  int entryNumber = (pageNumber / (sizeof(address) * 8));
  int bitNumber = (pageNumber % (sizeof(address) * 8));

  if ((mem == INT_RAM) && (pageNumber < MAX_PAGES_IN_INT_RAM)) {
    SET_BIT((((address)occupiedPagesIntRam) + entryNumber), bitNumber);
  } else if ((mem == EXT_DDR) && (pageNumber < MAX_PAGES_IN_EXT_DDR)) {
    SET_BIT((((address)occupiedPagesExtDDR) + entryNumber), bitNumber);
  }
}

BOOLEAN isOccupied(enum MemoryType mem, int pageNumber) {
  int entryNumber = (pageNumber / (sizeof(address) * 8));
  int bitNumber = (pageNumber % (sizeof(address) * 8));

  BOOLEAN result = FALSE;
  if ((mem == INT_RAM) && (pageNumber < MAX_PAGES_IN_INT_RAM)) {
    result
        = READ_BIT((((address)occupiedPagesIntRam) + entryNumber), bitNumber);
  } else if ((mem == EXT_DDR) && (pageNumber < MAX_PAGES_IN_EXT_DDR)) {
    result
        = READ_BIT((((address)occupiedPagesExtDDR) + entryNumber), bitNumber);
  }

  return result;
}

void releasePage(enum MemoryType mem, int pageNumber) {
  int entryNumber = (pageNumber / (sizeof(address) * 8));
  int bitNumber = (pageNumber % (sizeof(address) * 8));

  if ((mem == INT_RAM) && (entryNumber < MAX_PAGES_IN_INT_RAM)) {
    CLEAR_BIT((address)(((address)occupiedPagesIntRam) + entryNumber), bitNumber);
  } else if ((mem == EXT_DDR) && (entryNumber < MAX_PAGES_IN_EXT_DDR)) {
    CLEAR_BIT((address)(((address)occupiedPagesExtDDR) + entryNumber), bitNumber);
  }
}

int page_for_address(enum MemoryType* type, unsigned int memAddress) {
  int result = -1;
  if ((memAddress >= INT_RAM_START) && (memAddress < INT_RAM_START
      + INT_RAM_SIZE)) {
    result = (memAddress - INT_RAM_START) / 4096;
    *type = INT_RAM;
  } else if ((memAddress >= EXT_DDR_START) && (memAddress < EXT_DDR_START
      + EXT_DDR_SIZE)) {
    result = (memAddress - EXT_DDR_START) / 4096;
    *type = EXT_DDR;
  }
  return result;
}

address addressOfPage(enum MemoryType mem, int pageNumberInMemory) {
  address result = 0x0;
  switch (mem) {
    case INT_RAM:
      result = (address) (INT_RAM_START + (pageNumberInMemory * 4096));
      break;
    case EXT_DDR:
      result = (address) (EXT_DDR_START + (pageNumberInMemory * 4096));
      break;
  }
  return result;
}
// // TODO (thomas.bargetz@gmail.com) firstPageNumber necessary?
void reservePages(enum MemoryType mem, int firstPageNumber, int nrOfPages) {
  int i = NULL;
  for (i = firstPageNumber; i < (firstPageNumber + nrOfPages); i++) {
    reservePage(mem, i);
  }
}
void ram_manager_release_pages(enum MemoryType mem, int firstPageNumber, int nrOfPages) {
  int i = NULL;
  for (i = firstPageNumber; i < (firstPageNumber + nrOfPages); ++i) {
    releasePage(mem, i);
    memset((void*) addressOfPage(mem, i), 0x0, 4096);
  }
}

address findFreeMemoryin(enum MemoryType mem, int nrOfPages, BOOLEAN align,
                         BOOLEAN reserve) {
  int i = NULL;
  address result = 0x0;
  int freePages = 0;
  for (i = 0; (i < maxPagesIn(mem)) && (result == 0x0); i++) {
    if ((!isOccupied(mem, i)) && ((!align) || ((freePages > 0) || ((i
        % nrOfPages) == 0)))) {
      freePages++;
      if (freePages == nrOfPages) {
        result = addressOfPage(mem, (i - nrOfPages) + 1);
        if (reserve) {
          reservePages(mem, (i - nrOfPages) + 1, nrOfPages);
        }
      }
    } else {
      freePages = 0;
    }
  }
  return result;
}

address ram_manager_find_free_memory(int nrOfPages, BOOLEAN align, BOOLEAN reserve) {
  address result = findFreeMemoryin(INT_RAM, nrOfPages, align, reserve);
  if (result == 0x0) {
    result = findFreeMemoryin(EXT_DDR, nrOfPages, align, reserve);
  }
  return result;
}

int maxPagesIn(enum MemoryType mem) {
  int result = 0;
  switch (mem) {
    case INT_RAM:
      result = MAX_PAGES_IN_INT_RAM;
      break;
    case EXT_DDR:
      result = MAX_PAGES_IN_EXT_DDR;
      break;
  }
  return result;
}

