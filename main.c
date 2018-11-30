#include <stdio.h>
#include <string.h>
#include "funcmain.h"

#define MSJ_ERR_1 "puntero nulo en defaultArgs"
#define MSJ_ERR_2 "no mem en defaultArgs"
#define MSJ_ERR_EPTNULL "puntero nulo takeargs"
#define MSJ_ERR_OPENL "no se pudo abrir logfile"
#define MSJ_ERR_OPENI "no se pudo abrir infile"
#define MSJ_ERR_OPENO "no se pudo abrir outfile"
#define MSJ_ERR_ENOMEM "no hay memoria en takeargs"

int main (int argc, char *argv[]){
FILE *fin, *fout, *flog;
	struct s_RMC Rmc;
	struct s_GGA Gga;
	struct s_ZDA Zda;
	struct fecha fecha;
	char linea[MAX_LINE];
	struct args arg;
	//struct data track;	
	//struct fecha date;
	status_t st;
	sent_t t;

	if((st = defaultArgs(&arg)) == ST_EPTNULL){
		fprintf(stderr, "%s\n", MSJ_ERR_1); //cambiar mensaje
		return EXIT_SUCCESS;
	}

	else if(st == ST_ENOMEM){
		fprintf(stderr, "%s\n", MSJ_ERR_2); //cambiar mensaje
		return EXIT_SUCCESS;
	}

	if((st = takeArgs(argc, argv, &arg)) == ST_HELP){

		printHelp();
		return EXIT_SUCCESS;
	}
	
	else if(st == ST_INV){

		fprintf(stderr, "%s\n", MSJ_ERR_INV);//cambiar mensaje
		return EXIT_SUCCESS;
	}

	else if(st == ST_EPTNULL){

		fprintf(stderr, "%s\n", MSJ_ERR_EPTNULL);//cambiar mensaje
		return EXIT_SUCCESS;
	}
	else if(st == ST_ENOMEM){

		fprintf(stderr, "%s\n", MSJ_ERR_ENOMEM);//cambiar mensaje
		return EXIT_SUCCESS;
	}

	if(!strcmp(arg.logfile_n, DEFAULT_LOGFILE)){
		flog = stderr;
	}

	else{
		if((flog = fopen(arg.logfile_n, "w")) == NULL){
		fprintf(stderr, "%s\n", MSJ_ERR_OPENL);//cambiar mensaje
		return EXIT_SUCCESS;
		}
	}

	if(!strcmp(arg.infile_n, DEFAULT_INFILE)){
		fin = stdin;
	}

	else{
		if((fin = fopen(arg.infile_n, "rb")) == NULL){
		fprintf(flog, "%s\n", MSJ_ERR_OPENI);//cambiar mensaje
		return EXIT_SUCCESS;
		}
	}
	if(!strcmp(arg.outfile_n, DEFAULT_OUTFILE)){
		fout = stdout;
	}

	else{
		if((fout = fopen(arg.outfile_n, "w")) == NULL){
			fprintf(flog, "%s\n", MSJ_ERR_OPENO);//cambiar mensaje
			return EXIT_SUCCESS;
		}
	}

	if (arg.protocol==NMEA){
	// este while es para hacer pruebas, no es parte del programa
		while(fgets(linea, MAX_LINE, fin)){

			fprintf(fout, "%s", linea);

			t=checkLine(linea);
			switch (t){
			case GGA : if ( cargar_struct_gga(linea,&Gga,&fecha)){
			//para probar
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
		//para probar
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
			//para probar
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
	}

	else if(arg.protocol==UBX){
		//aca iria lo de ubx
		printf("%s\n","UBX");

	}

	printf("name: %s\n", arg.name);
	printf("protocol: %d\n", arg.protocol);
	printf("maxlen: %ld\n", arg.maxlen);
	
	return EXIT_SUCCESS;
}
