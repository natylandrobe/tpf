#ifndef DEFCHECK_H
#define DEFCHECK_H

#include <string.h>
#include "structData.h"

#define CHECK_GGA "$GPGGA,"
#define CHECK_RMC "$GPRMC,"
#define CHECK_ZDA "$GPZDA,"

#define CANT_CSUM 2
#define ASTERISCO "*"
#define MAX_LAT 90.0
#define MAX_LON 180.0
#define MAX_CAL 8
#define MAX_CANT 12
#define MIN_CAL_CANT 0
#define MIN_ANIOMESDIA 1
#define MAX_MES 12
#define MAX_DIA 31
#define MAX_ANIO 9999
#define MAX_DIG '9'
#define MIN_DIG '0'
#define END_STR '\0'

#define CANT_CSUM 2

unsigned char nmea_checksum(const char * s);
sent_t checkLine(char *s);


#endif