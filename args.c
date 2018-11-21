#include "defArgs.h"

#define MIN_DIG 0
#define MAX_DIG 9
/* Lee los argumentos recibidos, verifica que sean correctos y decide que accion tomar segun el argumento */

status_t takeArgs(int argc, char *argv[], struct args *arg){
	int i, argumento;

	if(!argc || !argv || !arg){
		return ST_EPTNULL;
	}

	for (i = 1; i < argc; i++){
		printf("arg: %d\n", i);
		/*se fija si el argumento empieza con '-' */
		if (*argv[i] == ARG_C){
			argumento = *(argv[i]+1);
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

							arg->name = (char *)realloc(arg->name, sizeof(char)*(strlen(argv[i+1])+1));

							if(arg->name == NULL){
								free(arg->name);
								return ST_ENOMEM;
							}

							strcpy(arg->name, argv[i+1]);

							break;

						case PROT_C:
							if(!strcmp(argv[i+1], PROT_NMEA)){
								arg->protocol = NMEA;
							}
							if(!strcmp(argv[i+1], PROT_UBX)){
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

							arg->infile_n = (char *)realloc(arg->infile_n, sizeof(char)*(strlen(argv[i+1])+1));

							if(arg->infile_n == NULL){
								free(arg->infile_n);
								return ST_ENOMEM;
							}

							strcpy(arg->infile_n, argv[i+1]);
							break;

						case OUTFILE_C:

							if(!strcmp(argv[i+1], DEFAULT_FILE)){
								break;
							}

							arg->outfile_n = (char *)realloc(arg->outfile_n, sizeof(char)*(strlen(argv[i+1])+1));

							if(arg->outfile_n == NULL){
								free(arg->outfile_n);
								return ST_ENOMEM;
							}

							strcpy(arg->outfile_n, argv[i+1]);
							break;

						case LOGFILE_C:

							if(!strcmp(argv[i+1], DEFAULT_FILE)){
								break;
							}

							arg->logfile_n = (char *)realloc(arg->logfile_n, sizeof(char)*(strlen(argv[i+1])+1));

							if(arg->logfile_n == NULL){
								free(arg->logfile_n);
								return ST_ENOMEM;
							}

							strcpy(arg->logfile_n, argv[i+1]);
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

						arg->name = (char *)realloc(arg->name, sizeof(char)*(strlen(argv[i+1])+1));

						if(arg->name == NULL){
							free(arg->name);
							return ST_ENOMEM;
						}

						strcpy(arg->name, argv[i+1]);
					}

					else if(!strcmp(argv[i], ARG_PROT)){

						if(!strcmp(argv[i+1], PROT_NMEA)){
								arg->protocol = NMEA;
							}

						if(!strcmp(argv[i+1], PROT_UBX)){
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
							arg->infile_n = (char *)realloc(arg->infile_n, sizeof(char)*(strlen(argv[i+1])+1));

							if(arg->infile_n == NULL){
								free(arg->infile_n);
								return ST_ENOMEM;
							}
							strcpy(arg->infile_n, argv[i+1]);
						}
					}

					else if(!strcmp(argv[i], ARG_OUTFILE)){

						if(!strcmp(argv[i+1], DEFAULT_FILE)){
								;
						}

						else{
							arg->outfile_n = (char *)realloc(arg->outfile_n, sizeof(char)*(strlen(argv[i+1])+1));

							if(arg->outfile_n == NULL){
								free(arg->outfile_n);
								return ST_ENOMEM;
							}
							strcpy(arg->outfile_n, argv[i+1]);
						}
					}

					else if(!strcmp(argv[i], ARG_LOGFILE)){

						if(!strcmp(argv[i+1], DEFAULT_FILE)){
								;
						}

						else{
							arg->logfile_n = (char *)realloc(arg->logfile_n, sizeof(char)*(strlen(argv[i+1])+1));
							
							if(arg->logfile_n == NULL){
								free(arg->logfile_n);
								return ST_ENOMEM;
							}

							strcpy(arg->logfile_n, argv[i+1]);
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



