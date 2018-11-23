#ifndef STRUCTDATA_H
#define STRUCTDATA_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_NAME "Default Name"
#define DEFAULT_INFILE "stdin"
#define DEFAULT_OUTFILE "stdout"
#define DEFAULT_LOGFILE "stderr"
#define MAX_STR 11

typedef enum{invalido, fix_GPS, fix_DGPS, fix_PPS, real_time_kinematic, float_rtk, estimada, manual, simulacion} cal_t; 
typedef enum{HELP, NAME, PROTOCOL, INFILE, OUTFILE, LOGFILE, MAXLEN} args_t;
extern const char dic_args[][MAX_STR];
typedef enum{INV, UBX, NMEA} protocol_t;

struct args {
	char *name;
	protocol_t protocol;
	char *infile_n;
	char *outfile_n;
	char *logfile_n;
	long maxlen;
};

struct fecha {
	int dia;
	int mes;
	int anio;
	int hora;
	int minutos;
	float segundos;
};

struct data {
	struct fecha f;
	double lat;
	double lon;
	double ele;
	double hdop;
	double separacion;
	cal_t calidad;
	long int cantSat;
};

#endif
