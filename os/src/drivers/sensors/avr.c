/* avr.c
 * 
 * Copyright (c) 2011 The ottos project.
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
 *  Created on: 1 Feb 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#include <ottos/kernel.h>
#include <ottos/memory.h>
#include <ottos/error.h>
#include "avr.h"

enum Sensor {
  TEMP = 0, PRESSURE = 1, SOLAR = 2
};

//void avr_readval(void);

//double read_temp();
//double read_pressure();
//double read_solar();


const double ADCtoTempTable[256] = { 300, 214.3, 178.2, 159.4, 147, 137.7, 130.5, 124.5, 119.5, 115.1, 111.3, 107.8, 104.7, 101.9, 99.4, 97, 94.8,
                                     92.7, 90.8, 89, 87.3, 85.7, 84.2, 82.7, 81.3, 80, 78.7, 77.5, 76.3, 75.2, 74.1, 73.1, 72, 71, 70.1, 69.2, 68.3,
                                     67.4, 66.5, 65.7, 64.9, 64.1, 63.4, 62.6, 61.9, 61.2, 60.5, 59.8, 59.1, 58.4, 57.8, 57.2, 56.5, 55.9, 55.3,
                                     54.7, 54.2, 53.6, 53, 52.5, 51.9, 51.4, 50.9, 50.3, 49.8, 49.3, 48.8, 48.3, 47.8, 47.3, 46.9, 46.4, 45.9, 45.5,
                                     45, 44.6, 44.1, 43.7, 43.2, 42.8, 42.4, 42, 41.5, 41.1, 40.7, 40.3, 39.9, 39.5, 39.1, 38.7, 38.3, 37.9, 37.5,
                                     37.2, 36.8, 36.4, 36, 35.6, 35.3, 34.9, 34.5, 34.2, 33.8, 33.5, 33.1, 32.7, 32.4, 32, 31.7, 31.3, 31, 30.6,
                                     30.3, 30, 29.6, 29.3, 28.9, 28.6, 28.3, 27.9, 27.6, 27.3, 26.9, 26.6, 26.3, 25.9, 25.6, 25.3, 25, 24.6, 24.3,
                                     24, 23.6, 23.3, 23, 22.7, 22.4, 22, 21.7, 21.4, 21.1, 20.8, 20.4, 20.1, 19.8, 19.5, 19.1, 18.8, 18.5, 18.2,
                                     17.9, 17.5, 17.2, 16.9, 16.6, 16.3, 15.9, 15.6, 15.3, 15, 14.6, 14.3, 14, 13.7, 13.3, 13, 12.7, 12.4, 12, 11.7,
                                     11.4, 11, 10.7, 10.4, 10, 9.7, 9.3, 9, 8.7, 8.3, 8, 7.6, 7.3, 6.9, 6.6, 6.2, 5.8, 5.5, 5.1, 4.7, 4.4, 4, 3.6,
                                     3.3, 2.9, 2.5, 2.1, 1.7, 1.3, 0.9, 0.5, 0.1, -0.3, -0.7, -1.1, -1.5, -2, -2.4, -2.8, -3.3, -3.7, -4.2, -4.6,
                                     -5.1, -5.6, -6.1, -6.6, -7.1, -7.6, -8.1, -8.6, -9.1, -9.7, -10.3, -10.8, -11.4, -12, -12.6, -13.3, -13.9,
                                     -14.6, -15.2, -15.9, -16.7, -17.4, -18.2, -19, -19.9, -20.7, -21.6, -22.6, -23.6, -24.7, -25.8, -27, -28.3,
                                     -29.7, -31.2, -32.9, -34.7, -36.8, -39.2, -42.1, -45.7, -50.6, -58.4 };

const double ADCtoPressureTable[256] = { 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024, 1025, 1026, 1027,
                                         1028, 1029, 1030, 1031, 1032, 1033, 1034, 1035, 1036, 1037, 1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045,
                                         1046, 1047, 1048, 1049, 1050, 1051, 1052, 1053, 1054, 1055, 1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063,
                                         1064, 1065, 1066, 1067, 1068, 1069, 1070, 1071, 1072, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081,
                                         1082, 1083, 1084, 1085, 1086, 1087, 1088, 1089, 1090, 1091, 1092, 1093, 1094, 1095, 1096, 1097, 1098, 1099,
                                         1100, 1101, 1102, 1103, 1104, 1105, 1106, 1107, 1108, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1116, 1117,
                                         1118, 1119, 1120, 1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1130, 1131, 1132, 1133, 1134, 1135,
                                         1136, 1137, 1138, 1139, 1140, 1141, 1142, 1143, 1144, 1145, 1146, 1147, 1148, 1149, 1150, 1151, 1152, 1153,
                                         1154, 1155, 1156, 1157, 1158, 1159, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171,
                                         1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1187, 1188, 1189,
                                         1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207,
                                         1208, 1209, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1224, 1225,
                                         1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240, 1241, 1242, 1243,
                                         1244, 1245, 1246, 1247, 1248, 1249, 1250, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1260, 1261,
                                         1262, 1263, 1264, 1265 };

const double ADCtoSolarTable[256] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                      31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
                                      59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
                                      87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
                                      112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
                                      134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
                                      156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177,
                                      178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                                      200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221,
                                      222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
                                      244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255 };

static BOOLEAN avr_initialized = FALSE;

int avr_init() {

  //resets the MUX register for the coresponding BITs
  MMIO_AND16(MUX_GPIO139, 0x00);
  MMIO_AND16(MUX_GPIO138, 0x00);
  MMIO_AND16(MUX_GPIO137, 0x00);
  MMIO_AND16(MUX_GPIO136, 0x00);
  MMIO_AND16(MUX_GPIO135, 0x00);
  MMIO_AND16(MUX_GPIO134, 0x00);
  MMIO_AND16(MUX_GPIO133, 0x00);
  MMIO_AND16(MUX_GPIO132, 0x00);

  //sets MUXMODE 4, pulup and input enabled bits to the MUX register
  MMIO_OR16(MUX_GPIO139, GPIO_MUX_SETTINGS);
  MMIO_OR16(MUX_GPIO138, GPIO_MUX_SETTINGS);
  MMIO_OR16(MUX_GPIO137, GPIO_MUX_SETTINGS);
  MMIO_OR16(MUX_GPIO136, GPIO_MUX_SETTINGS);
  MMIO_OR16(MUX_GPIO135, GPIO_MUX_SETTINGS);
  MMIO_OR16(MUX_GPIO134, GPIO_MUX_SETTINGS);
  MMIO_OR16(MUX_GPIO133, GPIO_MUX_SETTINGS);
  MMIO_OR16(MUX_GPIO132, GPIO_MUX_SETTINGS);

  //set this eight pins as input
  MMIO_OR32(GPIO5_OE, GPIO_139); //Pin 3
  MMIO_OR32(GPIO5_OE, GPIO_138); //Pin 5
  MMIO_OR32(GPIO5_OE, GPIO_137); //Pin 7
  MMIO_OR32(GPIO5_OE, GPIO_136); //Pin 9
  MMIO_OR32(GPIO5_OE, GPIO_135); //Pin 11
  MMIO_OR32(GPIO5_OE, GPIO_134); //Pin 13
  MMIO_OR32(GPIO5_OE, GPIO_133); //Pin 15
  MMIO_OR32(GPIO5_OE, GPIO_132); //Pin 17

  //sets pin 19 and 21 as output
  MMIO_AND32(GPIO5_OE, ~GPIO_131); //Pin 19
  MMIO_AND32(GPIO5_OE, ~GPIO_130); //Pin 21


  //  MMIO_OR32(GPIO5_OE, GPIO_183);    //Pin 23
  return 1;
}

static unsigned int readPort() {
  unsigned int gpioval;
  int flipVal = 0;

  gpioval = MMIO_READ32(GPIO5_DATAIN);

  if ((gpioval & GPIO_132) != GPIO_132) {
    flipVal += 128;
  }
  if ((gpioval & GPIO_133) != GPIO_133) {
    flipVal += 64;
  }
  if ((gpioval & GPIO_134) != GPIO_134) {
    flipVal += 32;
  }
  if ((gpioval & GPIO_135) != GPIO_135) {
    flipVal += 16;
  }
  if ((gpioval & GPIO_136) != GPIO_136) {
    flipVal += 8;
  }
  if ((gpioval & GPIO_137) != GPIO_137) {
    flipVal += 4;
  }
  if ((gpioval & GPIO_138) != GPIO_138) {
    flipVal += 2;
  }
  if ((gpioval & GPIO_139) != GPIO_139) {
    flipVal += 1;
  }

  return flipVal;

}

static int selSensor(enum Sensor sensor) {
  int retval = -1;
  if (sensor == TEMP) { // 00 to AVR -> AVR switches to temp messure mode
    MMIO_AND32(GPIO5_DATAOUT, ~GPIO_131);
    MMIO_AND32(GPIO5_DATAOUT, ~GPIO_130);
    retval = 1;
  } else if (sensor == PRESSURE) { // 01 to AVR -> AVR switches to pressure messure mode
    MMIO_AND32(GPIO5_DATAOUT, ~GPIO_130);
    MMIO_AND32(GPIO5_DATAOUT, ~GPIO_131);
    MMIO_OR32(GPIO5_DATAOUT, GPIO_131);
    retval = 1;
  } else if (sensor == SOLAR) { // 11 to AVR -> AVR switches to solar messure mode
    MMIO_AND32(GPIO5_DATAOUT, ~GPIO_130);
    MMIO_AND32(GPIO5_DATAOUT, ~GPIO_131);
    MMIO_OR32(GPIO5_DATAOUT, GPIO_130);
    MMIO_OR32(GPIO5_DATAOUT, GPIO_131);
    retval = 1;
  }
  return retval;
}

int avr_open_(device_t dev) {
  if (avr_initialized == FALSE) {
    avr_init();
    avr_initialized = TRUE;
  }
  return DRIVER_NO_ERROR;
}

int avr_close_(device_t dev) {
  return DRIVER_NO_ERROR;
}

int avr_read_(device_t dev, int count, char* buffer) {
  double value = 0.0;

  switch (dev) {
    case TEMP_0:
      selSensor(TEMP);
      kernel_sleep(1);
      value = ADCtoTempTable[readPort()];
      break;
    case PRESSURE_0:
      selSensor(PRESSURE);
      kernel_sleep(1);
      value = ADCtoPressureTable[readPort()];
      break;
    case SOLAR_0:
      selSensor(SOLAR);
      kernel_sleep(1);
      value = 255 - ADCtoSolarTable[readPort()];
      break;
    default:
      return DRIVER_ERROR_NOT_SUPPORTED;
  }

  if (count < sizeof(double)) {
    return DRIVER_ERROR_CANNOT_READ;
  }

  *((double*) buffer) = value;

  return sizeof(double);
}

int avr_write_(device_t dev, int count, char* buffer) {
  return DRIVER_ERROR_NOT_SUPPORTED;
}

int avr_ioctl_(device_t dev, ioctl_t msg) {
  return DRIVER_ERROR_NOT_SUPPORTED;
}

int avr_create_(device_t dev) {
  return DRIVER_ERROR_NOT_SUPPORTED;
}
