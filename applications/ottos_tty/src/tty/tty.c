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
//#include <ottos/io.h>
#include <ottos/const.h>
#include <ottos/types.h>
#include <ottos_api/io.h>
#include <ottos_api/system.h>

// keep track of the current working directory
// start with the users $HOME directory
static char tty_cwd[MAX_PATH_LENGTH] = HOME_DIRECTORY;

// private functions
//static void tty_error(const char* message) {
//  char buffer[2096];
//  sprintf(buffer, "-%s: %s\n\r", TTY_NAME, message);
//  print(buffer);
//}

//static void tty_username(char* buffer) {
//  sprintf(buffer, "%s", USERNAME);
//}
//
//static void tty_hostname(char* buffer) {
//  sprintf(buffer, "%s", HOSTNAME);
//}

static void tty_print_prefix() {
//  char username[16];
//  char hostname[16];
//  char prefix[64 + MAX_PATH_LENGTH];

//  tty_username(username);
//  tty_hostname(hostname);

  //sprintf(prefix, "%s@%s:%s%s ", username, hostname, tty_cwd, "#");
  print(USERNAME);
  print("@");
  print(HOSTNAME);
  print(":");
  print(tty_cwd);
  print("#");
  //print(prefix);
}

//static BOOLEAN tty_find_binary(const char* name) {
//  char filename[512];
//  FILE* file;
//  sprintf(filename, "/bin/%s", name);
//
//  file = fl_fopen(filename, "r");
//  if (file != NULL) {
//    fl_fclose(file);
//    return TRUE;
//  }
//  return FALSE;
//}
//
//// TODO (fdomig@gmail.com) move the CMDs to a separate file
//static void tty_cmd_ls(char* directory) {
//  FL_DIR dirstat;
//
//  if (directory == NULL) {
//    directory = tty_cwd;
//  }
//
//  fl_listdirectory(directory);
//}
//
//static void tty_cmd_cat(char* args) {
//  FILE* file;
//  char filename[1024];
//  char line[512];
//
//  if (*args != '/') {
//    sprintf(filename, "%s/%s", tty_cwd, args);
//  } else {
//    sprintf(filename, "%s", args);
//  }
//
//  file = fl_fopen(filename, "r");
//  if (file == NULL) {
//    char buffer[512];
//    sprintf(buffer, "cat: %s: No such file", args);
//    tty_error(buffer);
//    return;
//  }
//
//  while (!fl_feof(file)) {
//    fl_fread(line, 512, 512, file);
//    kernel_print(line);
//  }
//  kernel_print("\n\r");
//
//  fl_fclose(file);
//}
//
//static void tty_cmd_cd(char* args) {
//  // TODO (fdomig@gmail.com) ensure, the directory to change to exists
//  if (!fl_is_dir(args)) {
//    char buffer[512];
//    sprintf("cd: %s: No such directory", args);
//    tty_error(buffer);
//    return;
//  }
//
//  // change to home directory if no path is given
//  if (args == NULL) {
//    sprintf(tty_cwd, "%s", HOME_DIRECTORY);
//    return;
//  }
//
//  // remove trailing DIRECTORY_SEPARATOR
//  if (strlen(args) > 1) {
//    strtrim_right(args, DIRECTORY_SEPARATOR);
//  }
//
//  // change to a directory with an absolute path
//  if (*args == DIRECTORY_SEPARATOR) {
//    sprintf(tty_cwd, "%s", args);
//
//    // change to parent directory or stay in current directory
//  } else if (*args == '.') {
//    // change to parent directory
//    if (*(args + 1) == '.' && strlen(tty_cwd) > 1) {
//      int i = strlen(tty_cwd);
//      while (tty_cwd[--i] != DIRECTORY_SEPARATOR)
//        ;
//      if (i > 0) {
//        tty_cwd[i] = '\0';
//      } else {
//        tty_cwd[i + 1] = '\0';
//      }
//    }
//
//    // change to a child directory
//  } else {
//    strtrim_right(tty_cwd, DIRECTORY_SEPARATOR);
//    sprintf(tty_cwd, "%s%c%s", tty_cwd, DIRECTORY_SEPARATOR, args);
//  }
//}
//
//static void tty_cmd_pwd() {
//  char buffer[MAX_PATH_LENGTH];
//  sprintf(buffer, "%s\n\r", tty_cwd);
//  kernel_print(buffer);
//}

static int tty_start_process(const char* bin, char* args, BOOLEAN background) {
	// ignoring args
    sys_execute(1, background, bin);

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
  int i = 0;
  char c;
  do {
	  read_serial(&c, 1);
	  print(&c);
	  buffer[i] = c;
  } while (c != '\n' && i++ < length);

  buffer[i] = '\0';
  return i;
}

void tty_run() {

  tty_print_startup();

  while (TRUE) {
    // XXX: how do we ensure, we do not read more than 1025 characters?
    char line[1024 + 1] = { '\0' };
    char* tokens;
    char cmd[64];
    int rc;
    BOOLEAN background = FALSE;

    // print prefix
    //tty_print_prefix();

    // read one line
    // TODO (m.schmid@students.fhv.at) read from serial device
    // this has to be a system call in the end since we move this to
    // a user mode application
    // rc = scanf("%1024[^\n]%*[^\n]", line);
    // rc = serial_getline(line, 1024);
    print("command: ");
    rc = tty_getline(line, 1024);
    print("\r\n");
    print("your command: ");
    line[1024] = 0;
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
    tokens = strtok(NULL, SPLIT_CHARS);

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
//    if (!tty_find_binary(cmd)) {
//      char debug[256];
//      sprintf(debug, "%s command not found", line);
//      tty_error(debug);
//      continue;
//    }

    // run a new process
    if (cmd[strlen(cmd) - 1] == START_IN_BACKGROUND_SYSMBOL) {
      background = TRUE;
    }

    print("starting process from binary...\r\n");
    tty_start_process(cmd, tokens, background);
  }
}
