/*
 * code.c
 *
 *  Created on: 17.01.2012
 *      Author: Thomas
 */
#include <stdlib.h>
// TODO move fs to a different folder?
#include <vfat/fat_filelib.h>
#include <ottos/io.h>
#include <ottos/types.h>
#include <ottos/const.h>
#include <ottos/code.h>
#include <ottos/kernel.h>

code_bytes_t* code_get(const char* executable_file) {
	code_bytes_t* code = NULL;
	long file_length = 0;
	const char mode[] = { 'r' };

	// read executable file
	// the hex data is stored in 4 files
	// the filenames are executable_file.i0, executable_file.i1, executable_file.i2, executable_file.i3
	// where executable_file is the real filename or path
	const char* extension = ".i0";
	int extension_length = strlen(extension);
	int executable_filename_length = strlen(executable_file);
	int hex_filename_length = executable_filename_length + extension_length;

	char* executable_filename_0 = (char*) malloc(sizeof(char) * hex_filename_length);
	char* executable_filename_1 = (char*) malloc(sizeof(char) * hex_filename_length);
	char* executable_filename_2 = (char*) malloc(sizeof(char) * hex_filename_length);
	char* executable_filename_3 = (char*) malloc(sizeof(char) * hex_filename_length);

	void* executable_file_0;
	void* executable_file_1;
	void* executable_file_2;
	void* executable_file_3;

	char* byte_file_0 = NULL;
	char* byte_file_1 = NULL;
	char* byte_file_2 = NULL;
	char* byte_file_3 = NULL;

	BOOLEAN abort = FALSE;

	strcpy(executable_filename_0, executable_file);
	strcat(executable_filename_0, ".i0");

	strcpy(executable_filename_1, executable_file);
	strcat(executable_filename_1, ".i1");

	strcpy(executable_filename_2, executable_file);
	strcat(executable_filename_2, ".i2");

	strcpy(executable_filename_3, executable_file);
	strcat(executable_filename_3, ".i3");

	// check if files exist
	executable_file_0 = fl_fopen(executable_filename_0, mode);
	abort = executable_file_0 == NULL;
	fl_fclose(executable_file_0);

	if (!abort) {
		executable_file_1 = fl_fopen(executable_filename_1, mode);
		abort = executable_file_1 == NULL;
		fl_fclose(executable_file_1);
	}

	if (!abort) {
		executable_file_2 = fl_fopen(executable_filename_2, mode);
		abort = executable_file_2 == NULL;
		fl_fclose(executable_file_2);
	}

	if (!abort) {
		executable_file_3 = fl_fopen(executable_filename_3, mode);
		abort = executable_file_3 == NULL;
		fl_fclose(executable_file_3);
	}

	if(abort) {
		kernel_error(FILE_UNKNOWN, "Cannot open hex files for execution. One or more files doesn't exist");
	} else {

		// get the file length
		executable_file_0 = fl_fopen(executable_filename_0, mode);
		fl_fseek(executable_file_0, 0, SEEK_END);
		file_length = fl_ftell(executable_file_0);
		fl_fseek(executable_file_0, 0, SEEK_SET);

		byte_file_0 = malloc(sizeof(char) * file_length);
		byte_file_1 = malloc(sizeof(char) * file_length);
		byte_file_2 = malloc(sizeof(char) * file_length);
		byte_file_3 = malloc(sizeof(char) * file_length);

		fl_fread(byte_file_0, sizeof(char), file_length, executable_file_0);
		fl_fclose(executable_file_0);
		byte_file_0[file_length] = 0;

		executable_file_1 = fl_fopen(executable_filename_1, mode);
		fl_fread(byte_file_1, sizeof(char), file_length, executable_file_1);
		fl_fclose(executable_file_1);
		byte_file_1[file_length] = 0;

		executable_file_2 = fl_fopen(executable_filename_2, mode);
		fl_fread(byte_file_2, sizeof(char), file_length, executable_file_2);
		fl_fclose(executable_file_2);
		byte_file_2[file_length] = 0;

		executable_file_3 = fl_fopen(executable_filename_3, mode);
		fl_fread(byte_file_3, sizeof(char), file_length, executable_file_3);
		fl_fclose(executable_file_3);
		byte_file_3[file_length] = 0;

		code = malloc(sizeof(code_bytes_t));
		code->byte_0 = byte_file_0; // data in executable_file.i0
		code->byte_1 = byte_file_1; // data in executable_file.i1
		code->byte_2 = byte_file_2; // data in executable_file.i2
		code->byte_3 = byte_file_3; // data in executable_file.i3
	}

	free(executable_filename_0);
	free(executable_filename_1);
	free(executable_filename_2);
	free(executable_filename_3);

	return code;
}
