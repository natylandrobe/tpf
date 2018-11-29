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

#define AUX_PARA_HOR_MIN_SEG 10000
#define AUX_PARA_MIN_SEG 100

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
#define COMPARE_RMC_STATUS_ACT "A"
#define COMPARE_RMC_STATUS_VOID "V"
#define CANT_TOKEN_RMC 10
#define INDEX_LAT_RMC 3
#define INDEX_LAT_CARD_RMC 4
#define INDEX_LON_RMC 5 
#define INDEX_LON_CARD_RMC 6
#define INDEX_FECHA_RMC 9
#define INDEX_HORARIO_RMC 1
#define INDEX_STATUS_RMC 2

//para el zda
#define DELIM_AUX_ZDA "*"
#define CANT_TOKEN_ZDA 6
#define CANT_TOKEN_AUX_ZDA 2
#define INDEX_HORARIO_ZDA 1 
#define INDEX_DIA_ZDA 2
#define INDEX_MES_ZDA 3 
#define INDEX_ANIO_ZDA 4
#define INDEX_TIME_ZONE_ZDA 5
#define INDEX_DIF_TIME_ZONE_AUX_ZDA 6
#define INDEX_DIF_TIME_ZONE_ZDA 0

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
        sent_t tipo;
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
        sent_t tipo;
        struct fecha f;
        char status;
        double lat;
        double lon;
};

struct s_ZDA{
        sent_t tipo;
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
                         cal  > MAX_CAL  || cal  <  MIN_CAL_CANT || cal==invalido ||
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
        char *tokens[CANT_TOKEN_GGA];
        double lat, lon, hdop, ele, separacion;
        long int cal, cant;
        double horario;
        cal_t calidad;

       if(!s ){
             return false;
    	}

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

  		Gga->f.hora= horario/AUX_PARA_HOR_MIN_SEG;
        Gga->f.minutos = ((int)horario%AUX_PARA_HOR_MIN_SEG)/AUX_PARA_MIN_SEG;
        Gga->f.segundos = horario-Gga->f.hora*AUX_PARA_HOR_MIN_SEG-Gga->f.minutos*AUX_PARA_MIN_SEG;
        *date=Gga->f;
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
        char *tokens[CANT_TOKEN_RMC];
        double lat, lon;
        double horario;
       
     	if(!s){
                return false;
        }

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

        if (strcmp(tokens[INDEX_STATUS_RMC],COMPARE_RMC_STATUS_ACT)==0){
        	status='A';
        }

        
        if(strcmp(tokens[INDEX_STATUS_RMC],COMPARE_RMC_STATUS_VOID)==0){
			status='V';
			return false;
        }


        lat = convertirLat(tokens[INDEX_LAT_RMC], tokens[INDEX_LAT_CARD_RMC]);
        lon = convertirLon(tokens[INDEX_LON_RMC], tokens[INDEX_LON_CARD_RMC]);

        if(!checkMembersrmc(lat, lon)){
                return false;
        }
		
        Rmc->f.hora= horario/AUX_PARA_HOR_MIN_SEG;
        Rmc->f.minutos = ((int)horario%AUX_PARA_HOR_MIN_SEG)/AUX_PARA_MIN_SEG;
        Rmc->f.segundos = horario-Rmc->f.hora*AUX_PARA_HOR_MIN_SEG-Rmc->f.minutos*AUX_PARA_MIN_SEG;
        *date= Rmc->f;  
        Rmc->lat = lat;
        Rmc->lon = lon;
        Rmc->status= status;
        
        return true;
}

bool cargar_struct_zda(char *s, struct s_ZDA *Zda, struct fecha *date){


        char *str, *check;
        char *tokens[CANT_TOKEN_ZDA],*token[CANT_TOKEN_AUX_ZDA];
        double time_zone,dif_tmzone;
        double dia,mes,anio,horario;

       if(!s ){
               return false;
        }

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

        time_zone = strtod(tokens[INDEX_TIME_ZONE_ZDA], &check);

        if(*check != END_STR){
                return false;
        }
        str= strtok(tokens[INDEX_DIF_TIME_ZONE_AUX_ZDA],DELIM_AUX_ZDA);
                i = 0;
        while(str != NULL){
                token[i] = str;
                str = strtok(NULL,DELIM_AUX_ZDA);
                i++;
        }
        dif_tmzone = strtod(token[INDEX_DIF_TIME_ZONE_ZDA], &check);

       if(*check != END_STR){
                return false;
        }
        // por que el zda tiene que actualizar la fecha
        if (!checkDia(dia) || !checkMes(mes) || !checkAnio(anio)){
        	return false ;
        }

        Zda->f.dia =dia;
        Zda->f.mes = mes;
        Zda->f.anio = anio;
        Zda->f.hora= horario/AUX_PARA_HOR_MIN_SEG;
        Zda->f.minutos = ((int)horario%AUX_PARA_HOR_MIN_SEG)/AUX_PARA_MIN_SEG;
        Zda->f.segundos = horario-Zda->f.hora*AUX_PARA_HOR_MIN_SEG-Zda->f.minutos*AUX_PARA_MIN_SEG;
        *date=Zda->f;
        Zda->time_zone = time_zone;
        Zda->dif_tmzone =dif_tmzone;
        
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
		printf("%.3f\n",Gga.lat);
		printf("%.3f\n",Gga.lon);
		printf("%.1f\n",Gga.ele);
		printf("%.1f\n",Gga.hdop);
		printf("%.1f\n",Gga.separacion);
		printf("%li\n",Gga.cantSat);
		printf("%i\n",fecha.hora);
		printf("%i\n",fecha.minutos);
		printf("%.3f\n",fecha.segundos);
							} break;
		case RMC :
		if (cargar_struct_rmc(linea,&Rmc,&fecha)){
		printf("%s\n","BIENRM");
		printf("%.3f\n",Rmc.lat);
		printf("%.3f\n",Rmc.lon);
		printf("%c\n",Rmc.status);
		printf("%i\n",fecha.hora);
		printf("%i\n",fecha.minutos);
		printf("%.3f\n",fecha.segundos);							}
						 break;
		case ZDA :
		if(cargar_struct_zda(linea,&Zda,&fecha)){

		printf("%s\n","BIENZD");
		printf("%i\n",fecha.dia);
		printf("%i\n",fecha.mes);
		printf("%i\n",fecha.anio);
		printf("%i\n",fecha.hora);
		printf("%i\n",fecha.minutos);
		printf("%.3f\n",fecha.segundos);
		printf("%i\n",Zda.time_zone);
		printf("%i\n",Zda.dif_tmzone);
				 }
				break;
		case NING: printf("%s\n","MAL");
		
		}
	}
	return 	EXIT_SUCCESS;
}
