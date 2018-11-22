#include "defArgs.h"

#define MIN_DIG 0
#define MAX_DIG 9
#define CANT_ARGS 6

//status_t procesar_arg(int arg, char *argv[], args_t *argp, int argc, size_t index);

/* Lee los argumentos recibidos, verifica que sean correctos y decide que accion tomar segun el argumento */

status_t takeArgs(int argc, char *argv[], struct args *arg){
	int argumento;
	size_t i;
	//args_t argp;

	if(!argc || !argv || !arg){
		return ST_EPTNULL;
	}

	for (i = 1; i < argc; i++){
		printf("arg: %ld\n", i);
		/*se fija si el argumento empieza con '-' */
		if (*argv[i] == ARG_C){
			argumento = *(argv[i]+1);
			//procesar_arg(argumento, argv, argp, argc, i);
			if(argumento == HELP_C || argumento == toupper(HELP_C) || !strcmp(argv[i], ARG_HELP)){
				return ST_HELP;
			}

			else if(argc > i + 1){
				if(strlen(argv[i]) == 2){
					switch (tolower(argumento)){
						case NAME_C:
							if(*argv[i+1] == ARG_C){
								return ST_INV;	
							}

							free(arg->name);
							arg->name = strdup(argv[i+1]);

							if(arg->name == NULL){
								free(arg->name);
								return ST_ENOMEM;
							}

							break;

						case PROT_C:
							if(!strcmp(argv[i+1], PROT_NMEA)){
								arg->protocol = NMEA;
							}
							else if(!strcmp(argv[i+1], PROT_UBX)){
								arg->protocol = UBX;
							}
							else{
								return ST_INV;
							}
							break;

						case INFILE_C:

							if(!strcmp(argv[i+1], DEFAULT_FILE)){
								printf("argumento in es default\n");
								break;
							}

							free(arg->infile_n);
							arg->infile_n = strdup(argv[i+1]);

							if(arg->infile_n == NULL){
								free(arg->infile_n);
								return ST_ENOMEM;
							}

							break;

						case OUTFILE_C:

							if(!strcmp(argv[i+1], DEFAULT_FILE)){
								break;
							}

							free(arg->outfile_n);
							arg->outfile_n = strdup(argv[i+1]);

							if(arg->outfile_n == NULL){
								free(arg->outfile_n);
								return ST_ENOMEM;
							}

							break;

						case LOGFILE_C:

							if(!strcmp(argv[i+1], DEFAULT_FILE)){
								break;
							}

							free(arg->logfile_n);
							arg->logfile_n = strdup(argv[i+1]);

							if(arg->logfile_n == NULL){
								free(arg->logfile_n);
								return ST_ENOMEM;
							}

							break;
							
						case MAX_C:
							if(checkNum(argv[i+1])){
								arg->maxlen = strtol(argv[i+1], NULL, 10);
							}

							else{
								printf("aca01\n");
								return ST_INV;
							}
					}
				}

				else if(argumento == ARG_C){

					if(!strcmp(argv[i], ARG_NAME)){

						if(*argv[i+1] == ARG_C){
							printf("aca02\n");
							return ST_INV;
						}

						free(arg->name);
						arg->name = strdup(argv[i+1]);

						if(arg->name == NULL){
							free(arg->name);
							return ST_ENOMEM;
						}
					}

					else if(!strcmp(argv[i], ARG_PROT)){
						if(!strcmp(argv[i+1], PROT_NMEA)){
								arg->protocol = NMEA;
							}

						else if(!strcmp(argv[i+1], PROT_UBX)){
							arg->protocol = UBX;
						}

						else{
							printf("aca03\n");
							return ST_INV;
							}
					}

					else if(!strcmp(argv[i], ARG_INFILE)){

						if(!strcmp(argv[i+1], DEFAULT_FILE)){
								;
						}

						else{
							free(arg->infile_n);
							arg->infile_n = strdup(argv[i+1]);

							if(arg->infile_n == NULL){
								free(arg->infile_n);
								return ST_ENOMEM;
							}
						}
					}

					else if(!strcmp(argv[i], ARG_OUTFILE)){

						if(!strcmp(argv[i+1], DEFAULT_FILE)){
								;
						}

						else{
							free(arg->outfile_n);
							arg->outfile_n = strdup(argv[i+1]);

							if(arg->outfile_n == NULL){
								free(arg->outfile_n);
								return ST_ENOMEM;
							}
						}
					}

					else if(!strcmp(argv[i], ARG_LOGFILE)){

						if(!strcmp(argv[i+1], DEFAULT_FILE)){
								;
						}

						else{
							free(arg->logfile_n);
							arg->logfile_n = strdup(argv[i+1]);	

							if(arg->logfile_n == NULL){
								free(arg->logfile_n);
								return ST_ENOMEM;
							}

						}						
					}

					else if(!strcmp(argv[i], ARG_MAX)){

						if(checkNum(argv[i+1])){
							arg->maxlen = strtol(argv[i+1], NULL, 10);
						}

						else{
							printf("aca04\n");
							return ST_INV;
						}
					}

					else{
						printf("aca05\n");
						return ST_INV;

					}
						
				}

			}

		}
	}

	return ST_OK;
}

/*status_t procesar_arg(int arg, char *argv[], args_t *argp, int argc, size_t index){

	size_t i;
	char *argumento;

	if(!argv || !argp){
		return ST_EPTNULL;
	}

	argumento= (char *)malloc(sizeof(char)*(strlen(argv[index])+1));
							
	if(argumento == NULL){
		return ST_ENOMEM;
	}

	strcpy(arg->logfile_n, argv[i+1]);

	if(argumento == HELP_C || argumento == toupper(HELP_C) || !strcmp(argv[index], ARG_HELP)){
				return ST_HELP;
	}

	else if(argc > index + 1){
		if(strlen(argv[i]) == 2){
			switch (tolower(argumento)){
				case NAME_C:
		//evaluar casos minuscula
			}
		}
	}

	for(i = 0; i < CANT_ARGS; i++){
		if(!strcmp(argv[index], dic_args[i]){
			*argp = i;
			break;
		}
	}
}
*/


