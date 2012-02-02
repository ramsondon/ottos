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
#include <api/ipc.h>
#include <api/proc.h>
#include <api/sensor.h>
#include <ottos/types.h>
#include <vfat/fat_filelib.h>

#include <string.h>
#include <stdlib.h>

#define DISPLAYED_DATA_SET_SIZE   20
#define DATA_ENTERY_LENGTH        34    // 12 12 31 12 12 12 -01.4 24.3 0987    --> date time temperature humidity pressure
#define BLOCK_HEIGHT              500
#define BLOCK_WIDTH               280
#define BLOCK_MARGIN_HORIZONTAL_M 40
#define BLOCK_MARGIN_HORIZONTAL   46
#define MARGIN_VERTICAL_TOP       120
#define MARGIN_VERTICAL_ARROW     MARGIN_VERTICAL_TOP+200+5
#define BLOCK_COLOR COLOR_WhiteSmoke

void video_test() {
  //int i = 0, entries;
  //WEATHER_DATA data[DISPLAYED_DATA_SET_SIZE];time
  //GRAPH_DATA graph_data[DISPLAYED_DATA_SET_SIZE];
  char str[24];
  float temp, solar, pres;
  int i, j;
  time_t time;
  message_t msg;
  sensor_values_t values;

  // set background
  graphics_draw_rect(COLOR_Blue, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, FALSE);

  // set heading
  graphics_draw_string(COLOR_WhiteSmoke, 50, 100, "..::  O t t O S - W E T T E R S T A T I O N  ::..", 3, FALSE);

  // draw temperature block
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL, MARGIN_VERTICAL_TOP, BLOCK_WIDTH, BLOCK_HEIGHT, FALSE);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+60, MARGIN_VERTICAL_TOP+60, "TEMPERATUR [°C]", 2, FALSE);
  graphics_draw_rect(COLOR_DarkGray, BLOCK_MARGIN_HORIZONTAL+5, MARGIN_VERTICAL_TOP+200+5, BLOCK_WIDTH-10, 2, FALSE);

  // draw humidity block
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M+BLOCK_WIDTH, MARGIN_VERTICAL_TOP, BLOCK_WIDTH, BLOCK_HEIGHT, FALSE);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+30, MARGIN_VERTICAL_TOP+60, "SOLARLEISTUNG [lux]", 2, FALSE);
  graphics_draw_rect(COLOR_DarkGray, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+5, MARGIN_VERTICAL_TOP+200+5, BLOCK_WIDTH-10, 2, FALSE);

  // draw barometric pressure block
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_MARGIN_HORIZONTAL_M+2*BLOCK_WIDTH, MARGIN_VERTICAL_TOP, BLOCK_WIDTH, BLOCK_HEIGHT, FALSE);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+50, MARGIN_VERTICAL_TOP+60, "LUFTDRUCK [hPa]", 2, FALSE);
  graphics_draw_rect(COLOR_DarkGray, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+5, MARGIN_VERTICAL_TOP+200+5, BLOCK_WIDTH-10, 2, FALSE);

  j = 0;
  temp = 22.334;
  solar = 423.9837;
  pres = 955.34;
  msg.content = &values;
  msg.size = sizeof(sensor_values_t);
  msg.count = 1;
  msg.type = 1;

  while (TRUE) {
    // fake that fucking sensor data
    temp *= (j % 2 == 0 ? 1.3 : 0.88);
    solar *= (j % 5 == 0 ? 1.3 : 0.9);
    pres *= (j % 3 == 0 ? 1.3 : 0.9);

    if (receive("ottossensor", &msg) == IPC_SUCCESS) {

      // clear value area
      graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+1, MARGIN_VERTICAL_TOP+70, BLOCK_WIDTH-2, 132, FALSE);
      graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+1, MARGIN_VERTICAL_TOP+70, BLOCK_WIDTH-2, 132, FALSE);
      graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+1, MARGIN_VERTICAL_TOP+70, BLOCK_WIDTH-2, 132, FALSE);
      graphics_draw_rect(COLOR_Blue, 300, RESOLUTION_HEIGHT-120, 600, 100, FALSE);

      // write current temperature value
      sprintf(str, "%2.1f °C", temp, FALSE);
      graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+65, MARGIN_VERTICAL_TOP+200, str, 4, FALSE);

      // write current solar value
      sprintf(str, "%3.0f lux", solar);
      graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+60, MARGIN_VERTICAL_TOP+200, str, 4, FALSE);

      // write current pressure value
      sprintf(str, "%4.0f hPa", pres);
      graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+40, MARGIN_VERTICAL_TOP+200, str, 4, FALSE);

      // write current time
      time = sys_get_time();
      sprintf(str, "%02d.%02d.%04d-%02d:%02d:%02d", time.days, time.month, time.year, time.hours, time.minutes, time.seconds);
      graphics_draw_string(COLOR_WhiteSmoke, 340, RESOLUTION_HEIGHT-40, str, 3, FALSE);

      // draw arrows
      graphics_draw_arrow(COLOR_Green, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH/2, MARGIN_VERTICAL_ARROW+70, 0, 0, 1, FALSE);
      graphics_draw_arrow(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M+BLOCK_WIDTH*2-40, MARGIN_VERTICAL_ARROW + 140, 0, 0, 3, FALSE);
      graphics_draw_arrow(COLOR_Red, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M*2+BLOCK_WIDTH*2+BLOCK_WIDTH/2-40, MARGIN_VERTICAL_ARROW+140, 0, 0, 5, TRUE);

    }

    //psleep(2000);

    // pause for 1 minute, 10 seconds, 1 second???
    for (i = 0; i < 100000; i++) {
      j = i % 33;
    }
    j = time.days+time.hours+time.minutes+time.seconds+time.miliseconds;
  }
}


//int read_weather_data(WEATHER_DATA* data, int count) {
//  int i, fd, length, read_count, index;
//  char* line = malloc(sizeof(char)*DATA_ENTERY_LENGTH);
//  /*
//  fd = sys_open("data/weather.log", 0);
//
//  if (fd == NULL) {
//    free(line);
//    return FALSE;
//  }
//
//  sys_read(fd, line, 20);
//  */
//
//  FILE* file = (FILE*)fopen("data/weather.log", "r");
//
//  if (fd == NULL) {
//    free(line);
//    return -1;
//  }
//
//  fseek(file, 0, SEEK_END);
//  length = ftell(file);
//
//  // check if there are enough data entries available
//  if (length <= DISPLAYED_DATA_SET_SIZE*DATA_ENTERY_LENGTH+2) {
//    // there are fewer entries then asked
//    fseek(file, 0, SEEK_SET);
//  } else {
//    // there are more entries then asked
//    fseek(file, -DISPLAYED_DATA_SET_SIZE*DATA_ENTERY_LENGTH-2, SEEK_END);
//  }
//
//  index = 0;
//
//  // read 20 data enteries
//  do {
//    read_count = fread(line, sizeof(char), DATA_ENTERY_LENGTH, file);
//    if (read_count != sizeof(char)*DATA_ENTERY_LENGTH) {
//      break;
//    }
//
//    sscanf(line, "%2hd %2hd %2hd %2hd %2hd %2hd %f %f %f",
//                   &data[index].year, &data[index].month, &data[index].day,
//                   &data[index].hour, &data[index].minute, &data[index].second,
//                   &data[index].temp, &data[index].humidity, &data[index].pressure);
//    index++;
//  } while (index < DISPLAYED_DATA_SET_SIZE);
//
////  for (index = 0; index < DISPLAYED_DATA_SET_SIZE; index++) {
////    printf("Datum: %02d.%02d.%02d    ", data[index].day, data[index].month, data[index].year);
////    printf("Zeit: %d:%d:%d\n", data[index].hour, data[index].minute, data[index].second);
////    printf("Temp: %f °C     Feuchte: %f Proz     Luftdruck: %f hPa\n\n", data[index].temp, data[index].humidity, data[index].pressure);
////  }
//
//  free(line);
//
//  return index;
//}

