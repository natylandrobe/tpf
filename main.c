#include <stdio.h>
#include <string.h>
#include "funcmain.h"
#include "ubx.h"
#include "cargarStructs.h"
#include "files.h"

#define MSJ_ERR_1 "puntero nulo en defaultArgs"
#define MSJ_ERR_2 "no mem en defaultArgs"
#define MSJ_ERR_EPTNULL "puntero nulo takeargs"
#define MSJ_ERR_OPENL "no se pudo abrir logfile"
#define MSJ_ERR_OPENI "no se pudo abrir infile"
#define MSJ_ERR_OPENO "no se pudo abrir outfile"
#define MSJ_ERR_ENOMEM "no hay memoria en takeargs"

int main (int argc, char *argv[]){
FILE *fin = NULL, *fout = NULL, *flog = NULL;
	struct s_RMC Rmc;
	struct s_GGA Gga;
	struct s_ZDA Zda;
	struct fecha fecha;
	char linea[MAX_LINE];
	struct args arg;
	//struct data track;	
	status_t st, file_st;
	sent_t t;
	size_t i = 0;
	lista_t lista;
	ubxst_t proc_ubx;

	if(defaultFecha(&fecha) != ST_OK){
		printf("default fecha error\n");
		return EXIT_SUCCESS;
	}

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

	if((file_st = abrir_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		return EXIT_SUCCESS;
	}

	if(!(printMetadata(arg.name, &fecha, fout))){
		fprintf(flog, "%s\n", "Error ptr nulo");
		return EXIT_SUCCESS;
	}

	if(crear_lista(&lista) != ST_OK){
		fprintf(flog, "%s", "no se pudo crear la lista");
		return EXIT_SUCCESS;
	}

	if(arg.protocol == NMEA){
	// este while es para hacer pruebas, no es parte del programa
		while(fgets(linea, MAX_LINE, fin)){

			fprintf(fout, "%s", linea);

			t=checkLine(linea);
			switch (t){
			case GGA : if ( cargar_struct_gga(linea,&Gga,&fecha)){
			//para probar
			if (agregar_nodo(&Gga, &lista,t)==ST_OK)
				printf("%s\n","biengga");

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
			if (agregar_nodo(&Rmc, &lista,t)==ST_OK)
			printf("%s\n","bienrmc");

			printf("%s\n","BIENRM");
			printf("%.3f\n",Rmc.lat);
			printf("%.3f\n",Rmc.lon);
			printf("%c\n",Rmc.status);
			printf("%i\n",fecha.hora);
			printf("%i\n",fecha.minutos);
			printf("%.3f\n",fecha.segundos);							
			}
						 break;
			case ZDA :
			if(cargar_struct_zda(linea,&Zda,&fecha)){
			//para probar
			Gga.f=Zda.f;
			if (agregar_nodo(&Gga, &lista,t)==ST_OK)
				printf("%s\n","bienzda");
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

	else if(arg.protocol == UBX){
		while(i < arg.maxlen && (proc_ubx = procesar_ubx(fin, &fecha, &lista, &i, &agregar_nodo)) != S_EREAD){
			if(proc_ubx != S_OK){
				fprintf(flog, "%s\n", "error procesando una linea");
			}
		}
	}



	if(imprimir_lista(lista, fout) != ST_OK){
		fprintf(flog, "%s\n", "error imprimiendo lista");
		return EXIT_SUCCESS;
	}

	if(!(printTrkC(fout))){
		fprintf(flog, "%s\n", "Error ptr nulo");
		return EXIT_SUCCESS;
	}

	if(destruir_lista(&lista) != ST_OK){
		fprintf(flog, "%s\n", "error destruyendo lista");
		return EXIT_SUCCESS;
	}
	

	if(cerrar_archivos(&fin, &fout, &flog, &arg) != ST_OK){
		fprintf(stderr, "%s\n", "error cerrando archivos");
		return EXIT_SUCCESS;
	}


	if(liberar_args(&arg) != ST_OK){
		fprintf(flog, "%s\n", "Error ptr nulo");
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
