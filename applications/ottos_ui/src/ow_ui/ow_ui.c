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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_HEIGHT                500
#define BLOCK_WIDTH                 280
#define BLOCK_MARGIN_HORIZONTAL_M   40
#define BLOCK_MARGIN_HORIZONTAL     46
#define MARGIN_VERTICAL_TOP         120
#define MARGIN_VERTICAL_ARROW       MARGIN_VERTICAL_TOP+200+5
#define BLOCK_COLOR                 COLOR_WhiteSmoke
#define ARROW_COLOR_TREND_UP        COLOR_Green
#define ARROW_COLOR_TREND_DOWN      COLOR_Red
#define ARROW_COLOR_TREND_STABLE    COLOR_Black


#define AVG_RANGE 5
static sensor_values_t avg_old;
static int cur_index = 0;
static sensor_values_t value_history[AVG_RANGE];
static int old_trend_temp = TREND_STABLE;
static int old_trend_pressure = TREND_STABLE;
static int old_trend_solar = TREND_STABLE;


static double calculate_avg(int sensor_type) {
  double sum = 0.0;
  int i;

  if (sensor_type == SENSOR_TYPE_TEMP) {
    for (i = 0; i < AVG_RANGE; i++) {
      sum += value_history[i].temp;
    }
  } else if (sensor_type == SENSOR_TYPE_SOLAR) {
    for (i = 0; i < AVG_RANGE; i++) {
      sum += value_history[i].solar;
    }
  } else if (sensor_type == SENSOR_TYPE_PRESSURE) {
    for (i = 0; i < AVG_RANGE; i++) {
      sum += value_history[i].pressure;
    }
  }

  return sum / AVG_RANGE;
}

static int calculate_trend(double avg, double old_avg, float sensitivity) {
  if (avg > (1+sensitivity)*old_avg) {
    // UP TREND
    return TREND_UP;
  } else if (avg < (1-sensitivity)*old_avg) {
    // DOWN TREND
    return TREND_DOWN;
  } else {
    // STABLE TREND
    return TREND_STABLE;
  }
}

static void draw_time_block() {
  char str[12];
  time_t time;

  // clear value area
  graphics_draw_rect(COLOR_Blue, 300, RESOLUTION_HEIGHT-120, 600, 100, FALSE);

  // write current time
  time = sys_get_time();
  sprintf(str, "%02d.%02d.20%02d-%02d:%02d:%02d", time.days, time.month, time.year, time.hours, time.minutes, time.seconds);
  graphics_draw_string(COLOR_WhiteSmoke, 335, RESOLUTION_HEIGHT-40, str, 3, TRUE);
}

static void draw_temparature_block(double current_value) {
  double old_avg = 0.0;
  int trend = 0;
  char str[12];

  // clear value area
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+1, MARGIN_VERTICAL_TOP+70, BLOCK_WIDTH-2, 132, FALSE);
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+1, MARGIN_VERTICAL_ARROW + 4, BLOCK_WIDTH-2, 290, FALSE);

  // write current temperature value
  sprintf(str, "%2.1f °C", current_value);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+65, MARGIN_VERTICAL_TOP+200, str, 4, FALSE);

  old_avg = avg_old.temp;
  avg_old.temp = calculate_avg(SENSOR_TYPE_TEMP);
  trend = calculate_trend(avg_old.temp, old_avg, 0.1);

  if (trend != old_trend_temp) {
    old_trend_temp = trend;
    switch(trend) {
    case TREND_UP:
      graphics_draw_arrow(ARROW_COLOR_TREND_UP, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH/2, MARGIN_VERTICAL_ARROW+45, 0, 0, 1, FALSE);
      break;
    case TREND_DOWN:
      graphics_draw_arrow(ARROW_COLOR_TREND_DOWN, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH/2, MARGIN_VERTICAL_ARROW+245, 0, 0, 5, FALSE);
      break;
    case TREND_STABLE:
      graphics_draw_arrow(ARROW_COLOR_TREND_STABLE, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH-40, MARGIN_VERTICAL_ARROW+145, 0, 0, 3, FALSE);
      break;
    }
  }
}

static void draw_solar_block(double current_value) {
  double old_avg = 0.0;
  int trend = 0;
  char str[12];

  // clear value area
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+1, MARGIN_VERTICAL_TOP+70, BLOCK_WIDTH-2, 132, FALSE);
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+1, MARGIN_VERTICAL_ARROW + 4, BLOCK_WIDTH-2, 290, FALSE);

  // write current solar value
  sprintf(str, "%3.0f lux", current_value);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+60, MARGIN_VERTICAL_TOP+200, str, 4, FALSE);

  old_avg = avg_old.solar;
  avg_old.solar = calculate_avg(SENSOR_TYPE_SOLAR);
  trend = calculate_trend(avg_old.solar, old_avg, 0.1);

  if (trend != old_trend_solar) {
    old_trend_solar = trend;
    switch(trend) {
    case TREND_UP:
      graphics_draw_arrow(ARROW_COLOR_TREND_UP, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M+BLOCK_WIDTH+BLOCK_WIDTH/2, MARGIN_VERTICAL_ARROW+45, 0, 0, 1, FALSE);
      break;
    case TREND_DOWN:
      graphics_draw_arrow(ARROW_COLOR_TREND_DOWN, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M+BLOCK_WIDTH+BLOCK_WIDTH/2, MARGIN_VERTICAL_ARROW+245, 0, 0, 5, FALSE);
      break;
    case TREND_STABLE:
      graphics_draw_arrow(ARROW_COLOR_TREND_STABLE, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M+BLOCK_WIDTH*2-40, MARGIN_VERTICAL_ARROW+145, 0, 0, 3, FALSE);
      break;
    }
  }
}

static void draw_pressure_block(double current_value) {
  double old_avg = 0.0;
  int trend = 0;
  char str[12];

  // clear value area
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+1, MARGIN_VERTICAL_TOP+70, BLOCK_WIDTH-2, 132, FALSE);
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+1, MARGIN_VERTICAL_ARROW + 4, BLOCK_WIDTH-2, 290, FALSE);

  // write current pressure value
  sprintf(str, "%4.0f hPa", current_value);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+40, MARGIN_VERTICAL_TOP+200, str, 4, FALSE);

  old_avg = avg_old.pressure;
  avg_old.pressure = calculate_avg(SENSOR_TYPE_PRESSURE);
  trend = calculate_trend(avg_old.pressure, old_avg, 0.1);

  if (trend != old_trend_pressure) {
    old_trend_pressure = trend;
    switch(trend) {
    case TREND_UP:
      graphics_draw_arrow(ARROW_COLOR_TREND_UP, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M*2+BLOCK_WIDTH*2+BLOCK_WIDTH/2, MARGIN_VERTICAL_ARROW+45, 0, 0, 1, FALSE);
      break;
    case TREND_DOWN:
      graphics_draw_arrow(ARROW_COLOR_TREND_DOWN, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M*2+BLOCK_WIDTH*2+BLOCK_WIDTH/2, MARGIN_VERTICAL_ARROW+245, 0, 0, 5, FALSE);
      break;
    case TREND_STABLE:
      graphics_draw_arrow(ARROW_COLOR_TREND_STABLE, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M*2+BLOCK_WIDTH*3-40, MARGIN_VERTICAL_ARROW+145, 0, 0, 3, FALSE);
      break;
    }
  }
}

void video_test() {
  message_t msg;
  sensor_values_t values;

  // set background
  graphics_draw_rect(COLOR_Blue, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, FALSE);

  // set heading
  graphics_draw_string(COLOR_WhiteSmoke, 50, 100, "..::  O t t O S - W E T T E R S T A T I O N  ::..", 3, TRUE);

  // draw temperature block
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL, MARGIN_VERTICAL_TOP, BLOCK_WIDTH, BLOCK_HEIGHT, FALSE);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+60, MARGIN_VERTICAL_TOP+60, "TEMPERATUR [°C]", 2, FALSE);
  graphics_draw_rect(COLOR_DarkGray, BLOCK_MARGIN_HORIZONTAL+5, MARGIN_VERTICAL_ARROW, BLOCK_WIDTH-10, 2, FALSE);

  // draw humidity block
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+BLOCK_MARGIN_HORIZONTAL_M+BLOCK_WIDTH, MARGIN_VERTICAL_TOP, BLOCK_WIDTH, BLOCK_HEIGHT, FALSE);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+30, MARGIN_VERTICAL_TOP+60, "SOLARLEISTUNG [lux]", 2, FALSE);
  graphics_draw_rect(COLOR_DarkGray, BLOCK_MARGIN_HORIZONTAL+BLOCK_WIDTH+BLOCK_MARGIN_HORIZONTAL_M+5, MARGIN_VERTICAL_ARROW, BLOCK_WIDTH-10, 2, FALSE);

  // draw barometric pressure block
  graphics_draw_rect(BLOCK_COLOR, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_MARGIN_HORIZONTAL_M+2*BLOCK_WIDTH, MARGIN_VERTICAL_TOP, BLOCK_WIDTH, BLOCK_HEIGHT, FALSE);
  graphics_draw_string(COLOR_Black, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+50, MARGIN_VERTICAL_TOP+60, "LUFTDRUCK [hPa]", 2, FALSE);
  graphics_draw_rect(COLOR_DarkGray, BLOCK_MARGIN_HORIZONTAL+2*BLOCK_WIDTH+2*BLOCK_MARGIN_HORIZONTAL_M+5, MARGIN_VERTICAL_ARROW, BLOCK_WIDTH-10, 2, FALSE);

  msg.content = &values;
  msg.size = sizeof(sensor_values_t);
  msg.count = 1;
  msg.type = 1;

  while (TRUE) {
    // read sensordata
    if (receive("ottossensor", &msg) == IPC_SUCCESS) {
      // write values to history
      value_history[cur_index].temp = values.temp;
      value_history[cur_index].pressure = values.pressure;
      value_history[cur_index].solar = values.solar;

      cur_index++;
      if (cur_index >= AVG_RANGE) {
        cur_index = 0;

        draw_temparature_block(values.temp);
        draw_solar_block(values.solar);
        draw_pressure_block(values.pressure);
      }
    }

    draw_time_block();
  }
}
