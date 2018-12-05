#include "args.h"

/* Lee los argumentos recibidos, verifica que sean correctos y decide que accion tomar segun el argumento */

status_t takeArgs(int argc, char *argv[], struct args *arg){
	int argumento;
	size_t i;
	status_t st;
	args_t argp;

	if(!argc || !argv || !arg){
		return ST_EPTNULL;
	}

	for (i = 1; i < argc; i++){

		/*se fija si el argumento empieza con '-' */
		if (*argv[i] == ARG_C){
			argumento = *(argv[i]+1);

			if((st = procesar_arg(argumento, argv, &argp, i)) == ST_HELP){
				return ST_HELP;
			}

			else if(st == ST_OK && argc > 1+i){
				switch(argp){
					case NAME_A:
						if(*argv[i+1] == ARG_C){
							return ST_INV;	
						}

						free(arg->name);
						arg->name = strdup(argv[++i]);

						if(arg->name == NULL){
							free(arg->name);
							return ST_ENOMEM;
						}
						break;

					case PROTOCOL:
						if(!strcmp(argv[i+1], PROT_NMEA)){
							arg->protocol = NMEA;
							i++;
						}
						else if(!strcmp(argv[i+1], PROT_UBX)){
							arg->protocol = UBX;
							i++;
						}
						else{
							return ST_INV;
						}
						break;

					case INFILE:
						if(!strcmp(argv[i+1], DEFAULT_FILE)){
							i++;
							break;
						}
						free(arg->infile_n);
						arg->infile_n = strdup(argv[++i]);

						if(arg->infile_n == NULL){

							return ST_ENOMEM;
						}
						break;

					case OUTFILE:
						if(!strcmp(argv[i+1], DEFAULT_FILE)){
							i++;
							break;
						}

						free(arg->outfile_n);
						arg->outfile_n = strdup(argv[++i]);

						if(arg->outfile_n == NULL){

							return ST_ENOMEM;
						}

						break;

					case LOGFILE:
							if(!strcmp(argv[i+1], DEFAULT_FILE)){
								i++;
								break;
							}

							free(arg->logfile_n);
							arg->logfile_n = strdup(argv[++i]);

							if(arg->logfile_n == NULL){

								return ST_ENOMEM;
							}

							break;
					case MAXLEN:

						if(checkNum(argv[i+1])){
								arg->maxlen = strtol(argv[++i], NULL, 10);
							}

						else{
							return ST_INV;
						}

				}
			}
			else{
				if(!(argc > 1+i)){
					return ST_INV;
				}
				return st;
			}
		}
		else{
			return ST_INV;
		}
	}
	return ST_OK;
}


//toma el argumento y por interfaz devuelve el tipo de argumento correspondiente
status_t procesar_arg(int argum, char *argv[], args_t *argp, size_t index){

	const char dic_args[][MAX_STR] = {"--help", "--name", "--protocol", "--infile", "--outfile", "--logfile", "--maxlen"};
	size_t i;

	if(!argv || !argp){
		return ST_EPTNULL;
	}

	if(strlen(argv[index]) == 2){
		argum = tolower(argum);
		for(i = 0; i < CANT_ARGS; i++){

			if(argum == dic_args[i][ARGC_IND]){
				*argp = i;
				if(*argp == HELP_A){
					return ST_HELP;
				}
				return ST_OK;
			}
			
		}
		return ST_INV;
	}

	else if(argum == ARG_C){

		for(i = 0; i < CANT_ARGS; i++){
			if(!strcmp(argv[index], dic_args[i])){
				*argp = i;
		
				if(*argp == HELP_A){
					return ST_HELP;
				}
				return ST_OK;
			}
		}

		return ST_INV;
	}

	return ST_INV;
}

status_t liberar_args(struct args *arg){

	if(!arg){
		return ST_EPTNULL;
	}

	free(arg->name);
	free(arg->infile_n);
	free(arg->outfile_n);
	free(arg->logfile_n);

	return ST_OK;
}

