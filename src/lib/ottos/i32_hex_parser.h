/*
 * i32_hex_parser.h
 *
 *  Created on: 14.12.2011
 *  Author: Fox
 */

#ifndef I32_HEX_PARSER_H_
#define I32_HEX_PARSER_H_



#define O_RDONLY 1

// this structure represents a contiguous area
struct i32hex
{
    // start address of the area
    uint32_t addr;
    // length of the area
    uint32_t length;
    // area content
    uint8_t *data;
    // pointer to the next contiguous area
    struct i32hex *next;
    // debug info
    int linenum;
};


struct i32hex *parse_i32hexfile(const char *fname);


#endif /* I32_HEX_PARSER_H_ */
