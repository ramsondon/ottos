/*
 * code.h
 *
 *  Created on: 29.12.2011
 *      Author: Thomas
 */

#ifndef CODE_H_
#define CODE_H_

#include <ottos/types.h>

struct code;
typedef struct code code_t;

struct code {
	unsigned char byte_count;
	unsigned short address;
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

#endif /* CODE_H_ */
