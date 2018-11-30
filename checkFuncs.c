#include "defCheck.h"

/*verifica que tipo de nma es*/

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

/* Calcula la suma de verificacion */
unsigned char nmea_checksum(const char * s){

	unsigned char sum = 0;
	int i;

	for(i = 1; s[i] != ASTERISCO; i++){
		
		sum ^= s[i];
	}

	return sum;
}

/* Verifica que los datos recibidos sean validos */
bool checkMembers(double lat, double lon, cal_t cal, long int cant){

        return !(lat  > MAX_LAT  || lat  < -MAX_LAT      ||
                         lon  > MAX_LON  || lon  < -MAX_LON      ||
                         cal  > MAX_CAL  || cal  <  MIN_CAL_CANT || cal==invalido ||
                         cant > MAX_CANT || cant <  MIN_CAL_CANT);
}

/*verifica que los datos recividos sean validos*/
bool checkMembersrmc(double lat, double lon){

        return !(lat  > MAX_LAT  || lat  < -MAX_LAT      ||
                         lon  > MAX_LON  || lon  < -MAX_LON );
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


