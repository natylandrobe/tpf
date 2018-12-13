#include <stdio.h>
#include <string.h>
#include "main.h"


int main (int argc, char *argv[]){
	FILE *fin = NULL, *fout = NULL, *flog = NULL;
	struct s_RMC Rmc;
	struct s_GGA Gga;
	struct s_ZDA Zda;
	struct fecha fecha;
	char linea[MAX_LINE];
	struct args arg;
	status_t st;
	sent_t t;
	size_t i = 0;
	lista_t lista;
	ubxst_t proc_ubx;
	debug_t deb;

	if((st = defaultFecha(&fecha)) != ST_OK){
		imp_log(stderr, &st, NULL, NULL);
		return EXIT_SUCCESS;
	}

	if((st = defaultArgs(&arg)) != ST_OK){
		imp_log(stderr, &st, NULL, NULL);
		return EXIT_SUCCESS;
	}

	if((st = takeArgs(argc, argv, &arg)) == ST_HELP){

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(stderr, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}
		printHelp();
		return EXIT_SUCCESS;
	}
	
	else if(st != ST_OK){

		imp_log(stderr, &st, NULL, NULL);

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(stderr, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}
		
		return EXIT_SUCCESS;
	}

	if(arg.protocol == INV){
		st = ST_INV;
		
		imp_log(stderr, &st, NULL, NULL);

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(stderr, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}
		
		return EXIT_SUCCESS;
	}

	if((st = abrir_archivos(&fin, &fout, &flog, &arg)) != ST_OK){

		imp_log(stderr, &st, NULL, NULL);

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(stderr, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}

		return EXIT_SUCCESS;
	}

	if((st = printMetadata(arg.name, &fecha, fout)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);

		if((st = cerrar_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);
		return EXIT_SUCCESS;
		}

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(flog, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}

		return EXIT_SUCCESS;
	}



	if((st = crear_lista(&lista)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);

		if((st = cerrar_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);
		return EXIT_SUCCESS;
		}

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(flog, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}

		return EXIT_SUCCESS;
	}

	if(arg.protocol == NMEA){

		while(i < arg.maxlen && fgets(linea, MAX_LINE, fin)){

			t = checkLine(linea, flog);

			switch (t){
				case GGA: 
					if((st = cargar_struct_gga(linea,&Gga,&fecha)) == ST_OK){

						deb = MSJ_OK;
						imp_log(flog, NULL, NULL, &deb);

						if((st = agregar_nodo(&Gga, &lista, t)) == ST_OK){
							deb = CARGA_MSJ;
							imp_log(flog, NULL, NULL, &deb);
							i++;
							break;
						}

						else{
							imp_log(flog, &st, NULL, NULL);
							break;
						}
					}
					else{
							imp_log(flog, &st, NULL, NULL);
							break;
						}
					
				case RMC:
					if ((st = cargar_struct_rmc(linea,&Rmc,&fecha)) == ST_OK){
						deb = MSJ_OK;
						imp_log(flog, NULL, NULL, &deb);

						if ((st = agregar_nodo(&Rmc, &lista,t)) == ST_OK){
							deb = CARGA_MSJ;
							imp_log(flog, NULL, NULL, &deb);
							i++;
							break;
						}
						else{
							imp_log(flog, &st, NULL, NULL);
							break;
						}
					}

					else{
						imp_log(flog, &st, NULL, NULL);
						break;
					}

				case ZDA :
				if((st = cargar_struct_zda(linea,&Zda,&fecha)) == ST_OK){
					deb = MSJ_OK;
					imp_log(flog, NULL, NULL, &deb);
					break;
				}

				else{
					imp_log(flog, &st, NULL, NULL);	
					break;
				}
				case NING: 
					break;		
			
			}
		}

		if(!feof(fin)){
			deb = LIST_FULL;
			imp_log(flog, NULL, NULL, &deb);
		}
	}

	else if(arg.protocol == UBX){
		while((proc_ubx = procesar_ubx(fin, &fecha, &lista, &i, &agregar_nodo, flog, &arg)) != S_EREAD && i < arg.maxlen){
			if(proc_ubx != S_OK){
				imp_log(flog, NULL, &proc_ubx, NULL);
				if(proc_ubx == S_EPTNULL || proc_ubx == S_ENOMEM){

					if((st = cerrar_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
						imp_log(flog, &st, NULL, NULL);
						return EXIT_SUCCESS;
					}

					if((st = liberar_args(&arg)) != ST_OK){
						imp_log(flog, &st, NULL, NULL);
						return EXIT_SUCCESS;
					}

					return EXIT_SUCCESS;
				}
			}
		}
		if(proc_ubx != S_EREAD){
			deb = LIST_FULL;
			imp_log(flog, NULL, NULL, &deb);

		}
	}



	if((st = imprimir_lista(lista, fout, flog)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);

		if((st = cerrar_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);
		return EXIT_SUCCESS;
		}

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(flog, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}

		return EXIT_SUCCESS;
	}

	if((st = printTrkC(fout)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);

		if((st = cerrar_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);
		return EXIT_SUCCESS;
		}

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(flog, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}

		return EXIT_SUCCESS;
	}

	if((st = destruir_lista(&lista)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);

		if((st = cerrar_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);
		return EXIT_SUCCESS;
		}

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(flog, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}

		return EXIT_SUCCESS;
	}
	

	if((st = cerrar_archivos(&fin, &fout, &flog, &arg)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);

		if((st = liberar_args(&arg)) != ST_OK){
			imp_log(flog, &st, NULL, NULL);
			return EXIT_SUCCESS;
		}

		return EXIT_SUCCESS;
	}


	if((st = liberar_args(&arg)) != ST_OK){
		imp_log(flog, &st, NULL, NULL);
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
