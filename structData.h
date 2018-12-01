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

#define TIPO_LEN 3

typedef enum {ST_INV, ST_HELP, ST_OK, ST_EPTNULL, ST_ENOMEM, ST_EAGR, ST_EFILEO, ST_EFILEC} status_t;
typedef enum{invalido, fix_GPS, fix_DGPS, fix_PPS, real_time_kinematic, float_rtk, estimada, manual, simulacion} cal_t; 
typedef enum{HELP, NAME, PROTOCOL, INFILE, OUTFILE, LOGFILE, MAXLEN} args_t;
extern const char dic_args[][MAX_STR];
typedef enum{INV, UBX, NMEA} protocol_t;
typedef enum{GGA, RMC, ZDA, NAV_PVT, TIM_TOS, NAV_POSLLH, NING} sent_t;
typedef enum{SYNC, ID_D, MSJ_OK, CARGA_MSJ, IMP_MSJ, LIST_FULL}debug_t;
typedef enum{S_EPTNULL, S_ENOMEM, S_EREAD, S_CLASS_INV, S_ID_INV, S_CK_INV, S_LARGO_INV, S_FIX_INV, S_EAGR, S_OK} ubxst_t;

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


struct s_GGA{
	struct fecha f;
	double lat;
	double lon;
	double ele;
	double hdop;
	double separacion;
	cal_t calidad;
	long int cantSat;
};


struct s_RMC{
	struct fecha f;
	char status;
	double lat;
	double lon;
};

struct s_ZDA{
	struct fecha f;
	int time_zone;
	int dif_tmzone;
};

struct s_PVT{
	struct fecha f;
	unsigned char valid;
	unsigned char flags;
	unsigned char numSV;
	double lon;
	double lat;
	double hmsl;
	unsigned short int pDOP;
};

struct s_POSLLH{
	struct fecha f;
	double lon;
	double lat;
	double hmsl;
	unsigned int hAcc;
	unsigned int vAcc;
};

struct s_TIM_TOS{
	unsigned char version;
	unsigned char gnss;
	struct fecha f;
};

struct trkpt{
	struct fecha f;
	double lat;
	double lon;
	double ele;
};


#endif
