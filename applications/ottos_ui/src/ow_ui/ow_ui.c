/* ow_ui.c
 *
 * Copyright (c) 2011 The ottos_ui project.
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
 *  Created on: 25.01.2012
 *      Author: Florian Gopp (go.goflo@gmail.com)
 */

#include "ow_ui.h"

#include <api/io.h>
#include <api/graphics.h>
#include <api/bitmap.h>
#include <api/colors.h>
#include <api/system.h>
#include <vfat/fat_filelib.h>

#include <string.h>
#include <stdlib.h>

#define DISPLAYED_DATA_SET_SIZE   20
#define DATA_ENTERY_LENGTH        34    // 12 12 31 12 12 12 -01.4 24.3 0987    --> date time temperature humidity pressure

void video_test() {
  int i = 0, entries;
  RastPort *rp;
  void* framebuffer = malloc(3200000);
  WEATHER_DATA data[DISPLAYED_DATA_SET_SIZE];
  GRAPH_DATA graph_data[DISPLAYED_DATA_SET_SIZE];
  char str[10];

  // init
  rp = graphics_init(framebuffer, RESOLUTION_WIDTH , RESOLUTION_HEIGHT, BM_RGB16);
  //video_attach_framebuffer(0, rp->drawable.bitmap);

  // also set it to the tv out (top-left corner of same data)
  //video_attach_framebuffer(VID_VID2 | VID_TVOUT, rp->drawable.bitmap);

  // set background
  graphics_move_to(rp, 0, 0);
  graphics_set_color(rp, COLOR_LightCyan);
  graphics_draw_rect(rp, RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

  // set heading
  graphics_set_color(rp, COLOR_DarkSlateGray);
  graphics_move_to(rp, 20, 70);
  graphics_draw_string(rp, "oWeather powered by ..::OttOS::..", 3);

  // set current temperature
  graphics_move_to(rp, 10, 200);
  graphics_draw_string(rp, "Temperatur:", 2);


  // set current humidity
  graphics_move_to(rp, 10, 360);
  graphics_draw_string(rp, "Luftfeuchtigkeit:", 2);


  // set current barometric pressure
  graphics_move_to(rp, 10, 480);
  graphics_draw_string(rp, "Luftdruck:", 2);


  while (TRUE) {
    entries = read_weather_data(data, DISPLAYED_DATA_SET_SIZE);

    if (entries > 0) {
      // write current temperature value
      sprintf(str, "%2f °C\0", data[entries - 1].temp);
      graphics_move_to(rp, 200, 160);
      graphics_draw_string(rp, str, 2);

      sprintf(str, "%2f %\0", data[entries - 1].humidity);
      graphics_move_to(rp, 200, 360);
      graphics_draw_string(rp, str, 2);
      graphics_redraw(rp);

      sprintf(str, "%2f hPa\0", data[entries - 1].pressure);
      graphics_move_to(rp, 200, 480);
      graphics_draw_string(rp, str, 2);
      graphics_redraw(rp);

      // draw the temperature graph
      graphics_move_to(rp, 10, 300);
      graphics_draw_graph(rp, graph_data, 20, 1, 400, 680, COLOR_Lime, COLOR_Red);
    } else {
      // no data available
    }

    // redraw the screen
    graphics_redraw(rp);

    // pause for 1 minute, 10 seconds, 1 second???
  }
}


int read_weather_data(WEATHER_DATA* data, int count) {
  int i, fd, length, read_count, index;
  char* line = malloc(sizeof(char)*DATA_ENTERY_LENGTH);
  /*
  fd = sys_open("data/weather.log", 0);

  if (fd == NULL) {
    free(line);
    return FALSE;
  }

  sys_read(fd, line, 20);
  */

  FILE* file = (FILE*)fopen("data/weather.log", "r");

  if (fd == NULL) {
    free(line);
    return -1;
  }

  fseek(file, 0, SEEK_END);
  length = ftell(file);

  // check if there are enough data entries available
  if (length <= DISPLAYED_DATA_SET_SIZE*DATA_ENTERY_LENGTH+2) {
    // there are fewer entries then asked
    fseek(file, 0, SEEK_SET);
  } else {
    // there are more entries then asked
    fseek(file, -DISPLAYED_DATA_SET_SIZE*DATA_ENTERY_LENGTH-2, SEEK_END);
  }

  index = 0;

  // read 20 data enteries
  do {
    read_count = fread(line, sizeof(char), DATA_ENTERY_LENGTH, file);
    if (read_count != sizeof(char)*DATA_ENTERY_LENGTH) {
      break;
    }

    sscanf(line, "%2hd %2hd %2hd %2hd %2hd %2hd %f %f %f",
                   &data[index].year, &data[index].month, &data[index].day,
                   &data[index].hour, &data[index].minute, &data[index].second,
                   &data[index].temp, &data[index].humidity, &data[index].pressure);
    index++;
  } while (index < DISPLAYED_DATA_SET_SIZE);

//  for (index = 0; index < DISPLAYED_DATA_SET_SIZE; index++) {
//    printf("Datum: %02d.%02d.%02d    ", data[index].day, data[index].month, data[index].year);
//    printf("Zeit: %d:%d:%d\n", data[index].hour, data[index].minute, data[index].second);
//    printf("Temp: %f °C     Feuchte: %f Proz     Luftdruck: %f hPa\n\n", data[index].temp, data[index].humidity, data[index].pressure);
//  }

  free(line);

  return index;
}

