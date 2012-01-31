/* tty.c
 * 
 * Copyright (c) 2011 The tty project.
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
 *  Created on: Dec 22, 2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include "tty.h"

#include <string.h>
#include <stdlib.h>
#include <ottos/io.h>
#include <ottos/const.h>
#include <ottos/types.h>
#include <api/io.h>
#include <api/system.h>

#define FILE void

// linked list for arguments
typedef struct argument {
  char* value;
  int value_length;

  struct argument* next;
} argument_t;

// keep track of the current working directory
// start with the users $HOME directory
static char* tty_cwd;

// private functions
static void tty_error(const char* message) {
  print("-");
  print(TTY_NAME);
  print(": ");
  print(message);
  print("\n\r");
}

static void tty_print_prefix() {
  print(USERNAME);
  print("@");
  print(HOSTNAME);
  print(":");
  print(tty_cwd);
  print("#");
}

static BOOLEAN tty_find_binary(const char* name) {
  char* filename;
  int fd;

  filename = malloc(sizeof(char) * (strlen(name) + strlen(BIN_DIRECTORY) + 1));
  sprintf(filename, "%s/%s", BIN_DIRECTORY, name);

  fd = sys_open(filename, SYSTEM_FLAG_READ);
  free(filename);
  if (fd != SYSTEM_FD_INVALID) {
    sys_close(fd);
    return TRUE;
  }
  return FALSE;
}

static int tty_start_process(const char* bin, char* args, BOOLEAN background) {

  char* buffer = NULL;
  //	char** args_test = malloc(sizeof(char*) * 3);
  char** arguments = NULL;
  int argc = 1;
  int i = 0;

  /*
   char arg1[50] = { 0 };
   char arg2[50] = { 0 };
   char arg3[50] = { 0 };
   char* args_stupid[3];
   args_stupid[0] = arg1;
   args_stupid[1] = arg2;
   args_stupid[2] = arg3;

   while (args != NULL) {
   strcpy(args_stupid[argc], args);
   argc++;
   args = strtok(NULL, SPLIT_CHARS);
   }
   */

  // use a linked list to allocate memory for the arguments
  // after reading the arguments, the argv array will be initialised
  // and the argument values will be copied into the argv array

  // TODO this is so ugly - but we have only 2 days left until presentation ;)
  argument_t* head;
  argument_t* argument = malloc(sizeof(argument_t));
  argument->value_length = (strlen(bin) + 1);
  argument->value = malloc(sizeof(char) * argument->value_length);
  strcpy(argument->value, bin);
  argument->next = NULL;

  head = argument;

  // get next argument
  args = strtok(NULL, SPLIT_CHARS);
  while (args != NULL) {

    argument_t* next = malloc(sizeof(argument_t));
    next->value_length = (strlen(args) + 1);
    next->value = malloc(sizeof(char) * next->value_length);
    strcpy(next->value, args);
    next->next = NULL;

    argument->next = next;
    argument = next;

    argc++;

    // get next argument
    args = strtok(NULL, SPLIT_CHARS);
  }

  arguments = (char**) malloc(sizeof(char**) * argc);

  argument = head;
  while (argument != NULL) {
    argument_t* next;

    arguments[i] = malloc(sizeof(char) * argument->value_length);
    strcpy(arguments[i], argument->value);
    i++;

    next = argument->next;

    free(argument->value);
    free(argument);

    argument = next;
  }

  //	// split arguments by space
  //	char* args_ = args;
  //	while (args_ != NULL) {
  //		argc++;
  //		args_ = strtok(NULL, SPLIT_CHARS);
  //	}
  //	if (argc > 0) {
  //		arguments = (char**) malloc(sizeof(char*) * argc);
  //
  //		args_ = args;
  //		i = 0;
  //		while (args_ != NULL) {
  //			print(args_);
  //			print("\r\n");
  //			arguments[i] = (char*) malloc(sizeof(char) * (strlen(args_) + 1));
  //			if (arguments[i] == NULL) {
  //				print("WTF?\r\n");
  //			}
  //			strcpy(arguments[i], args_);
  //			//			arguments[i] = args_;
  //			i++;
  //			args_ = strtok(NULL, SPLIT_CHARS);
  //		}
  //	}

  buffer = malloc(sizeof(char) * MAX_PATH_LENGTH);

  //	args_test[0] = malloc(sizeof(char) * (strlen("arg1") + 1));
  //	args_test[1] = malloc(sizeof(char) * (strlen("arg2") + 1));
  //	args_test[2] = malloc(sizeof(char) * (strlen("arg3") + 1));
  //	strcpy(args_test[0], "arg1");
  //	strcpy(args_test[1], "arg2");
  //	strcpy(args_test[2], "arg3");

  sprintf(buffer, "/bin/%s", bin);

  //print("before starting process\r\n");
  //	sys_execute(1, background, bin, 3, args_test);
  //	sys_execute(1, background, bin, 0, NULL);
  sys_execute(1, background == FALSE, bin, argc, arguments);
  //	sys_execute(1, background, bin, argc, args_stupid);

  if (argc > 0) {
    for (i = 0; i < argc; i++) {
      free(arguments[i]);
    }
    free(arguments);
  }
  //		free(args_test[0]);
  //		free(args_test[1]);
  //		free(args_test[2]);
  //		free(args_test);
  free(buffer);

  // TODO (fdomig@gmail.com) return the process return state on foreground process
  return 0;
}

static void tty_print_startup() {
  print("\n\r");
  print("\n\r");
  print("  *****************************************\n\r");
  print("  *           Welcome to OttOS!           *\n\r");
  print("  *****************************************\n\r");
  print("\n\r");
  print("\n\r");
}

int tty_getline(char* buffer, int length) {
  return read_serial_with_end_char(buffer, length, '\n');
}

void tty_run() {

  // FIXME
  tty_cwd = malloc(sizeof(char) * MAX_PATH_LENGTH);

  tty_print_startup();

  while (TRUE) {
    // XXX: how do we ensure, we do not read more than 1025 characters?
    char line[MAX_PATH_LENGTH + 1] = { '\0' };

    char* tokens;
    char cmd[64];
    int rc;
    BOOLEAN background = TRUE;

    // print prefix
    tty_print_prefix();

    // read one line
    // TODO (m.schmid@students.fhv.at) read from serial device
    // this has to be a system call in the end since we move this to
    // a user mode application
    // rc = scanf("%1024[^\n]%*[^\n]", line);
    // rc = serial_getline(line, 1024);
    // print("command: ");
    rc = tty_getline(line, MAX_PATH_LENGTH);
    print("\r\n");
    print("your command: ");

    line[MAX_PATH_LENGTH] = 0;
    print(line);
    print("\r\n");
    //    if (rc == EOF) {
    //      // TODO (fdomig@gmail.com) line was already at EOF while trying to get
    //      // the first character
    //    } else if (rc == 0) {
    //      // TODO (fdomig@gmail.com) the user entered an empty line
    //    }

    // TODO (m.schmid@students.fhv.at) getchar(), feof() and ferror() have to be
    // changed to use our serial device - I have no idea how to do that
    // in the end we need a system call to get this done since we need to
    // move the TTY to the user space
    //if (!feof(stdin) && !ferror(stdin)) getchar();

    // split input string into tokens
    tokens = strtok(line, SPLIT_CHARS);
    strcpy(cmd, tokens);
    //cmd = tokens;
    //tokens = strtok(NULL, SPLIT_CHARS);

    // XXX: built in CMDs are for test only; later each built in CMD will get
    // an own binary file

    //    // check for a built in command
    //    if (strcmp(cmd, "cd") == 0) {
    //      tty_cmd_cd(tokens);
    //
    //    } else if (strcmp(cmd, "pwd") == 0) {
    //      tty_cmd_pwd();
    //
    //    } else if (strcmp(cmd, "ls") == 0) {
    //      tty_cmd_ls(tokens);
    //
    //    } else if (strcmp(cmd, "cat") == 0) {
    //      tty_cmd_cat(tokens);
    //
    //      // finally, is there a application with the entered name?
    //    } else

    if (!tty_find_binary(cmd)) {
      char* debug = malloc(sizeof(char) * 256);
      sprintf(debug, "%s command not found", line);
      tty_error(debug);
      free(debug);
      continue;
    }

    // TODO if there is a & at the end of the command, the tty_find_binary returns false
    // run a new process
    if (cmd[strlen(cmd) - 1] == START_IN_BACKGROUND_SYSMBOL) {
      background = TRUE;
    }
    {
      char* tmp_cmd = NULL;
      tmp_cmd = malloc(sizeof(char) * (strlen(cmd) + strlen(BIN_DIRECTORY) + 1));
      sprintf(tmp_cmd, "%s/%s", BIN_DIRECTORY, cmd);
      tty_start_process(tmp_cmd, tokens, background);
      free(tmp_cmd);
    }
  }
}
