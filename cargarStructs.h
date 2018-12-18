#ifndef CARGARSTRUCTS_H
#define CARGARSTRUCTS_H

#include "check.h"
#include <sys/time.h>

#define END_STR '\0'
#define AUX_PARA_HOR_MIN_SEG 10000
#define AUX_PARA_MIN_SEG 100
#define ASTERISCO "*"


/*para el GGA*/
#define DELIM ","
#define CANT_TOKEN_GGA 15
#define INDEX_HORARIO_GGA 1
#define INDEX_LAT 2
#define INDEX_LAT_CARD 3
#define INDEX_LON 4
#define INDEX_LON_CARD 5
#define INDEX_CAL 6
#define INDEX_CANT 7
#define INDEX_HDOP 8
#define INDEX_ELE 9
#define INDEX_SEP 11

/*para el RMC*/
#define STATUS_RMC_ACT 'A'
#define STATUS_RMC_VOID 'V'
#define COMPARE_STATUS_RMC_ACT "A"
#define COMPARE_STATUS_RMC_VOID "V"
#define CANT_TOKEN_RMC 12
#define INDEX_LAT_RMC 3
#define INDEX_LAT_CARD_RMC 4
#define INDEX_LON_RMC 5 
#define INDEX_LON_CARD_RMC 6
#define INDEX_FECHA_RMC 9
#define INDEX_HORARIO_RMC 1
#define INDEX_STATUS_RMC 2
#define AUX_1_PARA_FECHA 10000
#define AUX_2_PARA_FECHA 100
#define AUX_3_PARA_FECHA 2000

/*Para el ZDA*/
#define CANT_TOKEN_ZDA 6
#define CANT_TOKEN_AUX_ZDA 2
#define INDEX_HORARIO_ZDA 1 
#define INDEX_DIA_ZDA 2
#define INDEX_MES_ZDA 3 
#define INDEX_ANIO_ZDA 4
#define INDEX_TIME_ZONE_ZDA 5
#define INDEX_DIF_TIME_ZONE_AUX_ZDA 6
#define INDEX_DIF_TIME_ZONE_ZDA 0


#define MICROSEC 1000000.0 
#define YEAR_DIFF 1900 
#define MON_DIFF 1 
#define DIG_ANIO 10000 
#define DIG_MES 100 
#define CANT_DIG_FECHA 8 

#define RAND_DELIM 50

double convertirLon(const char lon[], const char * cardinal);
double convertirLat(const char lat[], const char * cardinal);
cal_t convertirCal(long int cal);
status_t cargar_struct_zda(char *s, struct s_ZDA *Zda, struct fecha *date);
status_t cargar_struct_rmc(char *s, struct s_RMC *Rmc, struct fecha *date);
status_t cargar_struct_gga(char *s,struct s_GGA *Gga,struct fecha *date);
status_t defaultFecha(struct fecha *def);
status_t defaultArgs(struct args *arg);

#endif

