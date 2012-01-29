/*
 * code.h
 *
 *  Created on: 29.12.2011
 *      Author: Thomas
 */

#ifndef CODE_H_
#define CODE_H_

#include <ottos/types.h>

// record types defines
#define CODE_RECORD_TYPE_DATA 						0x0
#define CODE_RECORD_TYPE_EOF 							0x1
#define CODE_RECORD_TYPE_EXTENDED_SEGMENT_ADDRESS 	0x2
#define CODE_RECORD_TYPE_START_SEGMENT_ADDRESS		0x3
#define CODE_RECORD_TYPE_EXTENDED_LINEAR_ADDRESS 		0x4
#define CODE_RECORD_TYPE_START_LINEAR_ADDRESS 		0x5

struct code;
typedef struct code code_t;

struct code {
	unsigned char byte_count;
	unsigned int address;
	unsigned char record_type;
	byte_t* bytes;

	code_t* next;
};

struct code_bytes;
typedef struct code_bytes code_bytes_t;

struct code_bytes {
	char* byte_0;
	char* byte_1;
	char* byte_2;
	char* byte_3;
};

EXTERN code_bytes_t* code_get_single_file(const char* executable_file);

EXTERN code_bytes_t* code_get(const char* executable_file);

#endif /* CODE_H_ */
