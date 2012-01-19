/* ihex2one.c
*
* Copyright (c) 2011 The ottos project.
*
* this work is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 2.1 of the License, or (at your option)
* any later version.
*
* this work is distributed in the hope that it will be useful, but without
* any warranty; without even the implied warranty of merchantability or
* fitness for a particular purpose. See the GNU Lesser General Public License
* for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation,
* Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*
* Created on: 19.01.2012
* Author: Franziskus Domig <fdomig@gmail.com>
*/

#include <stdio.h>

/**
 * Print the usage of this tool.
 */
void print_usage(const char* bin) {
    printf("Usage: %s inputfile [outputfile]\n", bin);
    printf("  inputfile:  The inputfile may not add the file extension (i0..i3).\n");
    printf("  outputfile: The outputfile parameter is optional.\n");
}

/**
 * Write one file to a specified output file.
 */
void write_file_to_output(const char* filename, FILE* output, FILE* define_file) {
    int c;
    static i = 0;
    
    FILE* input = fopen(filename, "r");
    if (!input) {
        printf("Could not open %s for reading input.\n", filename);
        return;
    }

    if (define_file) {
        fprintf(define_file, "#define byte%d \"", i++);
    }

    while ((c = fgetc(input)) != EOF) {
        fputc(c, output);
        
        if (define_file && c != '\n') {
            fputc(c, define_file);
        }
    }
    fputc('\n', output);
    
    if (define_file) {
        fputc('"', define_file);
        fputc('\n', define_file);
    }

    fclose(input);
}

/**
 * Generate the output/define file.
 */
void generate_file(const char* input_filename, const char* output_filename, char use_define) {
    char file0[64];
    char file1[64];
    char file2[64];
    char file3[64];
    char define_filename[128];
    
    FILE *output, *define_file;

    sprintf(file0, "%s.i0", input_filename);
    sprintf(file1, "%s.i1", input_filename);
    sprintf(file2, "%s.i2", input_filename);
    sprintf(file3, "%s.i3", input_filename);
    sprintf(define_filename, "%s-defines.h", output_filename);

    output = fopen(output_filename, "w+");
    if (!output) {
        printf("Could not create %s for output.\n", output_filename);
        return;
    }
    
    if (use_define) {
        define_file = fopen(define_filename, "w+");
    }
    
    write_file_to_output(file0, output, define_file);
    write_file_to_output(file1, output, define_file);
    write_file_to_output(file2, output, define_file);
    write_file_to_output(file3, output, define_file);

    fclose(output);
    
    if (use_define && define_file) {
        fclose(define_file);
    }
}

/**
 * Main entry point.
 */
int main(int argc, char** argv) {
    char* output = argv[1];
    char use_define = 1;
    
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    
    } else if (argc == 3) {
        output = argv[2];
    }
    
    generate_file(argv[1], output, use_define);
    
    return 0;
}
