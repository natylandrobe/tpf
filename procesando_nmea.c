#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


#define MAX_STR 11

#define TIPO_LEN 3

#define CHECK_GGA "$GPGGA,"
#define CHECK_RMC "$GPRMC,"
#define CHECK_ZDA "$GPZDA,"
#define CANT_CSUM 2
#define ASTERISCO '*'

#define MAX_LINE 300



#define TIPO_LEN 3

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

#define DELIM ","
#define CANT_TOKEN 15
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


#define DIGITO_GRADO  100.0
#define LAT_MAX 9000.000
#define GRADO_MIN 0
#define MIN_POR_GRADO 60
#define CARD_SUR 'S'
#define CARD_NORTE 'N'
#define CARD_ESTE 'E'
#define CARD_OESTE 'W'
#define LONG_MAX 18000
#define VALOR_SW -1
#define ERR_LATLON 181.0
#define END_STR '\0'
#define ERR_CAL 9

//para el rmc
#define CANT_TOKEN_RMC 10
#define INDEX_LAT_RMC 3
#define INDEX_LAT_CARD_RMC 4
#define INDEX_LON_RMC 5 
#define INDEX_LON_CARD_RMC 6
#define INDEX_FECHA_RMC 9
#define INDEX_HORARIO_RMC 1
#define INDEX_STATUS_RMC 2

//para el zda

#define CANT_TOKEN_ZDA 6
#define INDEX_HORARIO_ZDA 1 
#define INDEX_DIA_ZDA 2
#define INDEX_MES_ZDA 3 
#define INDEX_ANIO_ZDA 4
#define INDEX_TIME_ZONE_ZDA 5
#define INDEX_DIF_TIME_ZONE_ZDA 6


typedef enum{invalido, fix_GPS, fix_DGPS, fix_PPS, real_time_kinematic, float_rtk, estimada, manual, simulacion} cal_t;

typedef enum{GGA, RMC, ZDA, NAV_PVT, TIM_TOS, NAV_POSLLH, NING} sent_t;

struct fecha {
        int dia;
        int mes;
        int anio;
        int hora;
        int minutos;
        float segundos;
};


struct s_GGA{
        char tipo[TIPO_LEN];
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
        char tipo[TIPO_LEN];
        struct fecha f;
        char status;
        double lat;
        double lon;
};

struct s_ZDA{
        char tipo[TIPO_LEN];
        struct fecha f;
        int time_zone;
        int dif_tmzone;
}; 


//calcula la suma de verificacion
unsigned char nmea_checksum(const char * s){

	unsigned char sum = 0;
	int i;

	for(i = 1; s[i] != ASTERISCO; i++){
		
		sum ^= s[i];
	}

	return sum;
}

// verifica que tipo de nma es

sent_t checkLine(char *s){

	char checkSum[CANT_CSUM];

	if(!s){
		
		return NING;
	}

	/* Asumimos que todas las lineas traen '*' */
	checkSum[0] = *(strrchr(s, ASTERISCO) + 1);
	checkSum[1] = *(strrchr(s, ASTERISCO) + 2);

	if(strstr(s, CHECK_GGA) != NULL && nmea_checksum(s) == strtol(checkSum, NULL, 16)){
	
		return GGA;
	}
	if(strstr(s, CHECK_RMC) != NULL && nmea_checksum(s) == strtol(checkSum, NULL, 16)){
	
		return RMC;
	}
	if(strstr(s, CHECK_ZDA) != NULL && nmea_checksum(s) == strtol(checkSum, NULL, 16)){
	
		return ZDA;
	}
	return NING;
}


bool checkMembers(double lat, double lon, cal_t cal, long int cant){

        return !(lat  > MAX_LAT  || lat  < -MAX_LAT      ||
                         lon  > MAX_LON  || lon  < -MAX_LON      ||
                         cal  > MAX_CAL  || cal  <  MIN_CAL_CANT ||
                         cant > MAX_CANT || cant <  MIN_CAL_CANT);
}

/* Verifica que el numero de dia sea valido */
bool checkDia(int dia){

        return !(dia < MIN_ANIOMESDIA || dia > MAX_DIA);
}

/* Verifica que el numero de mes sea valido */
bool checkMes(int mes){

        return !(mes < MIN_ANIOMESDIA || mes > MAX_MES);
}

/* Verifica que el numero de anio sea valido */
bool checkAnio(int anio){
        
        return !(anio < MIN_ANIOMESDIA || anio > MAX_ANIO);
}

/* Verifica que la cadena recibida representa un numero decimal */
bool checkNum(char *s){

        int i;

        if(!s){

                return false;
        }

        for(i = 0; i < strlen(s); i++){

                if(!(s[i] >= MIN_DIG && s[i] <= MAX_DIG)){
                        
                        return false;
                }
        }
        return true;
}




/* Devuelve la longitud como numero, positivo si es este, negativo si es oeste */
double convertirLon(const char lon[], const char * cardinal){

        char *check;

        if(!cardinal){

                return ERR_LATLON;
        }

        double longitud = strtod(lon, &check);

        if(*check != END_STR){

                return ERR_LATLON;
        }

        if(longitud < GRADO_MIN || longitud > LONG_MAX){

                return ERR_LATLON;
        }
        if(*cardinal != CARD_ESTE && *cardinal != CARD_OESTE){

                return ERR_LATLON;
        }

        longitud = floor(longitud/ DIGITO_GRADO) + fmod(longitud, DIGITO_GRADO) / MIN_POR_GRADO;
        
        if(*cardinal == CARD_OESTE){

                longitud *= VALOR_SW;
        }
        
        return longitud;
}

/* Devuelve la latitud como numero, positivo si es norte, negativo si es sur */
double convertirLat(const char lat[], const char * cardinal){

        char *check;

        if(!cardinal){

                return ERR_LATLON;
        }

        double latitud = strtod(lat, &check);

        if(*check != END_STR){

                return ERR_LATLON;
        }

        if(latitud < GRADO_MIN || latitud > LAT_MAX){

                return ERR_LATLON;
        }
        if (*cardinal != CARD_NORTE && *cardinal != CARD_SUR){

                return ERR_LATLON;
        }

        latitud = floor(latitud / DIGITO_GRADO) + fmod(latitud, DIGITO_GRADO) / MIN_POR_GRADO;
        
        if (*cardinal == CARD_SUR){

                latitud *= VALOR_SW;
        }
        
        return latitud;
}

/* Devuelve la calidad en tipo enumerativo cal_t */
cal_t convertirCal(long int cal){

        switch (cal){
                case 0: 
                        return invalido;
                case 1:
                        return fix_GPS;
                case 2: 
                        return fix_DGPS;
                case 3:
                        return fix_PPS;
                case 4: 
                        return real_time_kinematic;
                case 5:
                        return float_rtk;
                case 6:
                        return estimada;
                case 7: 
                        return manual;
                case 8:
                        return simulacion;      
                default:
                        return ERR_CAL; 
        }
}

bool cargar_struct_gga(char *s,struct s_GGA *Gga,struct fecha *date){


        char *str, *check;
        char *tokens[CANT_TOKEN];
        double lat, lon, hdop, ele, separacion;
        long int cal, cant;
        double horario;
        cal_t calidad;

     //   if(!s || !newS){
       //         return false;
        //}

        str = strtok(s, DELIM);
        int i = 0;
        while(str != NULL){
                tokens[i] = str;
                str = strtok(NULL, DELIM);
                i++;
        }

        horario= strtod(tokens[INDEX_HORARIO_GGA], &check);

        if(*check != END_STR){
               return false;
        }

        
        cant = strtol(tokens[INDEX_CANT], &check, 10);

        if(*check != END_STR){
                return false;
        }

        hdop = strtod(tokens[INDEX_HDOP], &check);

        if(*check != END_STR){
                return false;
        }

        ele = strtod(tokens[INDEX_ELE], &check);

        if(*check != END_STR){
                return false;
        }

        separacion= strtod(tokens[INDEX_SEP], &check);

        if(*check != END_STR){
                return false;
        }

        cal = strtol(tokens[INDEX_CAL], &check, 10);

        if(*check != END_STR){
                return false;
        }

        calidad = convertirCal(cal);
        lat = convertirLat(tokens[INDEX_LAT], tokens[INDEX_LAT_CARD]);
        lon = convertirLon(tokens[INDEX_LON], tokens[INDEX_LON_CARD]);

        if(!checkMembers(lat, lon, calidad, cant)){
                return false;
        }

        //Gga->f= date;


        date->hora= horario/10000;
        date->minutos = ((int)horario%10000)/100;
        //falta ver los decimales de segundos
        date->segundos = ((int)horario%100);
        Gga->lat = lat;
        Gga->lon = lon;
        Gga->calidad = calidad;
        Gga->cantSat = cant;
        Gga->hdop = hdop;
        Gga->ele = ele;
        Gga->separacion = separacion;
        
        return true;
}


bool checkMembersrmc(double lat, double lon){

        return !(lat  > MAX_LAT  || lat  < -MAX_LAT      ||
                         lon  > MAX_LON  || lon  < -MAX_LON );
}



bool cargar_struct_rmc(char *s, struct s_RMC *Rmc, struct fecha *date){

        char *str, *check;
        char status;
        double stat;
        char *tokens[CANT_TOKEN_RMC];
        double lat, lon;
        double horario;
       
     //   if(!s || !newS){
       //         return false;
        //}

        str = strtok(s, DELIM);
        int i = 0;
        while(str != NULL){
                tokens[i] = str;
                str = strtok(NULL, DELIM);
                i++;
        }
        
        horario= strtod(tokens[INDEX_HORARIO_RMC], &check);

        if(*check != END_STR){
                return false;
        }

        stat=strtod(tokens[INDEX_STATUS_RMC], &check);
        
        if(*check != END_STR){
        	//esto es solo una prueba de que esto se cumple
        	printf("%s\n","ERROR");
        	//no se porque si desmarco esto retorna false
              //return false;
        }
        if (stat==65.00)
        	status='A';
        if(stat==86.00)
		status='V';


        lat = convertirLat(tokens[INDEX_LAT_RMC], tokens[INDEX_LAT_CARD_RMC]);
        lon = convertirLon(tokens[INDEX_LON_RMC], tokens[INDEX_LON_CARD_RMC]);

        if(!checkMembersrmc(lat, lon)){
                return false;
        }
		
		date->hora= horario/10000;
        date->minutos = ((int)horario%10000)/100;
        //falta ver los decimales de segundos
        date->segundos = ((int)horario%100);    
        Rmc->lat = lat;
        Rmc->lon = lon;
        Rmc->status= status;
        
        return true;
}

bool cargar_struct_zda(char *s, struct s_ZDA *Zda, struct fecha *date){


        char *str, *check;
        char *tokens[CANT_TOKEN];
        double time_zone,dif_tmzone,horario;
        //double aux=10000;
        double dia,mes,anio;
       

     //   if(!s || !newS){
       //         return false;
        //}

        str = strtok(s, DELIM);
        int i = 0;
        while(str != NULL){
                tokens[i] = str;
                str = strtok(NULL, DELIM);
                i++;
        }
        
        

        horario= strtod(tokens[INDEX_HORARIO_ZDA], &check);

        if(*check != END_STR){
               return false;
        }

        dia= strtod(tokens[INDEX_DIA_ZDA], &check);

        if(*check != END_STR){
                return false;
        }

        mes= strtod(tokens[INDEX_MES_ZDA], &check);

        if(*check != END_STR){
                return false;
        }

        anio= strtod(tokens[INDEX_ANIO_ZDA], &check);

        if(*check != END_STR){
                return false;
        }

        time_zone = strtol(tokens[INDEX_TIME_ZONE_ZDA], &check, 10);

        if(*check != END_STR){
                return false;
        }
        dif_tmzone = strtol(tokens[INDEX_DIF_TIME_ZONE_ZDA], &check, 10);
        // No se porque si desmarco esto , retorna falso
        /*if(*check != END_STR){
                return false;
        }*/

        //Zda->f=date;
        date->dia =dia;
        date->mes = mes;
        date->anio = anio;
        date->hora= horario/10000;
        date->minutos = ((int)horario%10000)/100;
        //falta ver los decimales de segundos
        date->segundos = ((int)horario%100);
        Zda->time_zone = time_zone;
        Zda->dif_tmzone = dif_tmzone;
        
        return true;
}

// lo probe de esta manera

int main(void){

	struct s_RMC Rmc;
	struct s_GGA Gga;
	struct s_ZDA Zda;
	struct fecha fecha;

	char linea[MAX_LINE];
	sent_t t;
	while (fgets(linea, MAX_LINE, stdin) != NULL){
		t=checkLine(linea);
		switch (t){
		case GGA : if ( cargar_struct_gga(linea,&Gga,&fecha)){
		printf("%s\n","BIENGGA");
		printf("%f\n",Gga.lat);
		printf("%f\n",Gga.lon);
		printf("%f\n",Gga.ele);
		printf("%f\n",Gga.hdop);
		printf("%f\n",Gga.separacion);
		printf("%li\n",Gga.cantSat);
		printf("%i\n",fecha.hora);
		printf("%i\n",fecha.minutos);
		printf("%f\n",fecha.segundos);
							} break;
		case RMC :
		if (cargar_struct_rmc(linea,&Rmc,&fecha)){
		printf("%s\n","BIENRM");
		printf("%f\n",Rmc.lat);
		printf("%f\n",Rmc.lon);
		printf("%c\n",Rmc.status);
		printf("%i\n",fecha.hora);
		printf("%i\n",fecha.minutos);
		printf("%f\n",fecha.segundos);							}
						 break;
		case ZDA :
		if(cargar_struct_zda(linea,&Zda,&fecha)){

		printf("%s\n","BIENZD");
		printf("%i\n",fecha.dia);
		printf("%i\n",fecha.mes);
		printf("%i\n",fecha.anio);
		printf("%i\n",fecha.hora);
		printf("%i\n",fecha.minutos);
		printf("%f\n",fecha.segundos);
		printf("%i\n",Zda.time_zone);
		printf("%i\n",Zda.dif_tmzone);
				 }
				break;
		case NING: printf("%s\n","MAL");
		
		}
	}
	return 	EXIT_SUCCESS;
}
