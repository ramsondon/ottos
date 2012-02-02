/* main.c
 * 
 * Copyright (c) 2011 The ottos_ps project.
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
 *  Created on: Jan 27, 2012
 *      Author: Matthias Schmid <ramsondon@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ottos/const.h>
#include <api/proc.h>
#include <api/io.h>
#include <api/memory.h>
#include <api/time.h>

static uint32_t process_count = 0;

pinfo_t* get_pinfos() {

  pinfo_t* pinfos = NULL;
  process_count = pcount();

  if (process_count <= 0) {
    return NULL;
  }
  pinfos = malloc(sizeof(pinfo_t) * process_count);
  process_count = pinfo(pinfos, process_count);
  return pinfos;
}

void free_pinfos(pinfo_t* pinfos) {
  int i = 0;
  for (i = 0; i < process_count; i++) {
    free(&pinfos[i]);
  }
}

pinfo_t* get_treenode(pid_t pid, pinfo_t* pinfos) {
  int i = 0;
  for (i = 0; i < process_count; i++) {
    if (pid == pinfos[i].pid) {
      return &pinfos[i];
    }
  }
  return NULL;
}

void print_treenode(pinfo_t* pinfo, int level) {
  char paddbuf[50] = { 0 };
  char buffer[100] = { 0 };
  int i = 0;
  for (i = 0; i < level; i++) {
    if (i != 0) {
      paddbuf[i] = '\t';
    }
  }
  if (level > 0) {
    paddbuf[i++] = '|';
    paddbuf[i++] = '\n';
    paddbuf[i++] = '\r';
  }
  sprintf(buffer, "%s+--%s\n\r", paddbuf, pinfo->cmd);
  print(buffer);
}

void traverse_tree(pid_t parent, pinfo_t* pinfo, int level) {
  int i = 0;

  pinfo_t* p = get_treenode(parent, pinfo);
  print_treenode(p, level);

  for (i = 0; i < process_count; i++) {
    if (pinfo[i].parent == parent) {
      traverse_tree(pinfo[i].pid, pinfo, level + 1);
    }
  }
}

void print_pstree(pinfo_t* pinfo) {

  int i = 0;
  print("+\n\r");
  for (i = 0; i < process_count; i++) {
    // we found the init process - let's traverse our process tree
    if (pinfo[i].parent == PID_INVALID) {
      print("|\n\r");
      traverse_tree(pinfo[i].pid, pinfo, 0);
      //break;
    }
  }
}

int main(int argc, char **argv) {

  // allocate resources and get process infos
  pinfo_t* pinfo = get_pinfos();

  if (pinfo != NULL) {

    print_pstree(pinfo);
    // free allocated resources
    free_pinfos(pinfo);
  }
  pexit(0);
  return 0;
}
