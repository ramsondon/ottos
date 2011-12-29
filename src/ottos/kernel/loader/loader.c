/* loader.c
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
 *  Created on: 21.10.2011
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <ottos/parser.h>
#include <ottos/const.h>
#include <ottos/types.h>
#include <ottos/code.h>
#include "../mmu/mmu.h"
#include "../ram_manager/ram_manager.h"
#include "loader.h"



void loader_load(process_t* process, code_bytes_t* code_bytes) {
	code_t* code_begin = parser_parse(code_bytes);

	// reserve memory for the code
	code_t* code_current = code_begin;

	// initialize some extreme values for the start and end page values
	int page_start = 255;
	int page_end = 0;
	int page_count = 0;
	address memory_start = NULL;

	while(code_current != NULL) {
		// check the record type data to find the start and end page
		if(code_current->record_type == CODE_RECORD_TYPE_DATA) {
			// the first hex number of the address of the current code
			// defines the page number

			// 1ABC = 1 page
			// 4000 = 4 page
			int page_number = code_current->address / MMU_PAGE_SIZE;
			if(page_number < page_start) {
				page_start = page_number;
			}
			if(page_number > page_end) {
				page_end = page_number;
			}
		}

		code_current = code_current->next;
	}

	// reserve the needed pages
	page_count = page_end - page_start + 1;
	memory_start = ram_manager_find_free_memory(page_count, FALSE, TRUE);

	// load the code in the reserved memory
	code_current = code_begin;
	while(code_current != NULL) {
		if(code_current->record_type == CODE_RECORD_TYPE_DATA) {
			// handle data record
			// get the start address for this record
			address address_start = (address)((int)memory_start + code_current->address - PROCESS_MEMORY_START);
			// copy the code data into the reserved memory
			memcpy((int*)address_start, code_current->bytes, code_current->byte_count);
		}

		code_current = code_current->next;
	}

	// set the code location for the process
	process->code_location = memory_start;
	// set the page count
	process->page_count = page_count;

	// free allocated memory for code parsing
	parser_free_parsed_code(code_begin);
}
