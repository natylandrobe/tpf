#include <stdio.h>
#include <string.h>
#include "main.h"
#include "ubx.h"
#include "cargarStructs.h"
#include "files.h"

int main (int argc, char *argv[]){
FILE *fin = NULL, *fout = NULL, *flog = NULL;
	struct s_RMC Rmc;
	struct s_GGA Gga;
	struct s_ZDA Zda;
	struct fecha fecha;
	char linea[MAX_LINE];
	struct args arg;
	//struct data track;	
	status_t st;
	sent_t t;
	size_t i = 0;
	lista_t lista;
	ubxst_t proc_ubx;

	if((st = defaultFecha(&fecha)) != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}

	if((st = defaultArgs(&arg)) != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}

	if((st = takeArgs(argc, argv, &arg)) == ST_HELP){

		printHelp();
		return EXIT_SUCCESS;
	}
	
	else if(st != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}

	if((st = abrir_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}

	if((st = printMetadata(arg.name, &fecha, fout)) != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}

	if((st = crear_lista(&lista)) != ST_OK){
		imp_log(flog, &st, NULL);
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
			printf("%.3f\n",fecha.segundos);							
			}
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

	else if(arg.protocol == UBX){
		while(i < arg.maxlen && (proc_ubx = procesar_ubx(fin, &fecha, &lista, &i, &agregar_nodo)) != S_EREAD){
			if(proc_ubx != S_OK){
				imp_log(flog, NULL, &proc_ubx);
				printf("aca printeo en el log mal fix\n");
				/*if(proc_ubx == S_EPTNULL || proc_ubx == S_ENOMEM){
					return EXIT_SUCCESS;
				}*/
			}
		}
	}



	if((st = imprimir_lista(lista, fout)) != ST_OK){
		imp_log(flog, &st, NULL);
		printf("aca tira puntero nulo\n");
		return EXIT_SUCCESS;
	}

	if((st = printTrkC(fout)) != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}

	if((st = destruir_lista(&lista)) != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}
	

	if((st = cerrar_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}


	if((st = liberar_args(&arg)) != ST_OK){
		imp_log(flog, &st, NULL);
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
