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

#include <ottos/io.h>
#include <ottos/const.h>
#include <ottos/types.h>
#include <ottos/kernel.h>

#include "../fs/vfat/fat_filelib.h"
#include "../drivers/serial/serial.h"

// keep track of the current working directory
// start with the users $HOME directory
static char tty_cwd[MAX_PATH_LENGTH] = HOME_DIRECTORY;

// private functions
static void tty_error(const char* message) {
  char buffer[2096];
  sprintf(buffer, "-%s: %s\n\r", TTY_NAME, message);
  kernel_print(buffer);
}

static void tty_username(char* buffer) {
  sprintf(buffer, "%s", USERNAME);
}

static void tty_hostname(char* buffer) {
  sprintf(buffer, "%s", HOSTNAME);
}

static void tty_print_prefix() {
  char username[16];
  char hostname[16];
  char prefix[64 + MAX_PATH_LENGTH];

  tty_username(username);
  tty_hostname(hostname);

  sprintf(prefix, "%s@%s:%s%s ", username, hostname, tty_cwd, "#");
  kernel_print(prefix);
}

static BOOLEAN tty_find_binary(const char* name) {
  return FALSE;
}

// TODO (fdomig@gmail.com) move the CMDs to a separate file
static void tty_cmd_ls(char* directory) {
  FL_DIR dirstat;

  if (directory == NULL) {
    directory = tty_cwd;
  }

  if (fl_opendir(directory, &dirstat)) {
    struct fs_dir_ent dirent;

    while (fl_readdir(&dirstat, &dirent) == 0) {
      char buffer[512];
      sprintf(buffer, "%srwx------ %8d root root   YYY %s\n\r", dirent.is_dir
          ? 'd' : '-', dirent.size, dirent.filename);
      kernel_print(buffer);
    }

    fl_closedir(&dirstat);
  }
}

static void tty_cmd_cd(char* args) {
  // TODO (fdomig@gmail.com) ensure, the directory to change to exists
  if (!fl_is_dir(args)) {
    char buffer[512];
    sprintf("cd: %s: No such directory", args);
    tty_error(buffer);
    return;
  }

  // change to home directory if no path is given
  if (args == NULL) {
    sprintf(tty_cwd, "%s", HOME_DIRECTORY);
    return;
  }

  // remove trailing DIRECTORY_SEPARATOR
  if (strlen(args) > 1) {
    strtrim_right(args, DIRECTORY_SEPARATOR);
  }

  // change to a directory with an absolute path
  if (*args == DIRECTORY_SEPARATOR) {
    sprintf(tty_cwd, "%s", args);

    // change to parent directory or stay in current directory
  } else if (*args == '.') {
    // change to parent directory
    if (*(args + 1) == '.' && strlen(tty_cwd) > 1) {
      int i = strlen(tty_cwd);
      while (tty_cwd[--i] != DIRECTORY_SEPARATOR)
        ;
      if (i > 0) {
        tty_cwd[i] = '\0';
      } else {
        tty_cwd[i + 1] = '\0';
      }
    }

    // change to a child directory
  } else {
    strtrim_right(tty_cwd, DIRECTORY_SEPARATOR);
    sprintf(tty_cwd, "%s%c%s", tty_cwd, DIRECTORY_SEPARATOR, args);
  }
}

static void tty_cmd_pwd() {
  char buffer[MAX_PATH_LENGTH];
  sprintf(buffer, "%s\n", tty_cwd);
  kernel_print(buffer);
}

static int tty_start_process(const char* bin, char* args, BOOLEAN background) {
  // TODO (fdomig@gmail.com) create a new foreground/background process

  // TODO (fdomig@gmail.com) return the process return state on foreground process
  return 0;
}

void tty_run() {
  while (TRUE) {
    // XXX: how do we ensure, we do not read more than 1025 characters?
    char line[1024 + 1] = { '\0' };
    char* tokens;
    char* cmd;
    int rc;
    BOOLEAN background = FALSE;

    // print prefix
    tty_print_prefix();

    // read one line
    // TODO (m.schmid@students.fhv.at) read from serial device
    // this has to be a system call in the end since we move this to
    // a user mode application
    //rc = scanf("%1024[^\n]%*[^\n]", line);
    rc = serial_getline(line, 1024);
    if (rc == EOF) {
      // TODO (fdomig@gmail.com) line was already at EOF while trying to get
      // the first character
    } else if (rc == 0) {
      // TODO (fdomig@gmail.com) the user entered an empty line
    }

    // TODO (m.schmid@students.fhv.at) getchar(), feof() and ferror() have to be
    // changed to use our serial device - I have no idea how to do that
    // in the end we need a system call to get this done since we need to
    // move the TTY to the user space
    //if (!feof(stdin) && !ferror(stdin)) getchar();

    // split input string into tokens
    tokens = strtok(line, SPLIT_CHARS);
    cmd = tokens;
    tokens = strtok(NULL, SPLIT_CHARS);

    // XXX: built in CMDs are for test only; late each built in CMD may get
    // an own binary

    // check for a built in command
    if (strcmp(cmd, "cd") == 0) {
      tty_cmd_cd(tokens);

    } else if (strcmp(cmd, "pwd") == 0) {
      tty_cmd_pwd();

    } else if (strcmp(cmd, "ls") == 0) {
      tty_cmd_ls(tokens);

      // finally, is there a application with the entered name?
    } else if (!tty_find_binary(cmd)) {
      char debug[256];
      sprintf(debug, "%s command not found", line);
      tty_error(debug);
      continue;
    }

    // run a new process
    if (cmd[strlen(cmd) - 1] == START_IN_BACKGROUND_SYSMBOL) {
      background = TRUE;
    }

    tty_start_process(cmd, tokens, background);
  }
}

void tty_test() {
  tty_run();
}
