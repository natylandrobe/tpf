#include <stdio.h>
#include <string.h>
#include "funcmain.h"

#define MSJ_ERR_1 "puntero nulo en defaultArgs"
#define MSJ_ERR_2 "no mem en defaultArgs"

int main (int argc, char *argv[]){
FILE *fin, *fout, *flog;
	char linea[MAX_LINE];
	struct args arg;
	//struct data track;	
	//struct fecha date;
	status_t st;

	if((st = defaultArgs(&arg)) == ST_EPTNULL){
		fprintf(stderr, "%s\n", MSJ_ERR_1); //cambiar mensaje
		return EXIT_FAILURE;
	}

	else if(st == ST_ENOMEM){
		fprintf(stderr, "%s\n", MSJ_ERR_2); //cambiar mensaje
		return EXIT_FAILURE;
	}

	if((st = takeArgs(argc, argv, &arg)) == ST_HELP){

		printHelp();
		return EXIT_SUCCESS;
	}
	
	else if(st == ST_INV){

		fprintf(stderr, "%s\n", MSJ_ERR_INV);//cambiar mensaje
		return EXIT_FAILURE;
	}

	else if(st == ST_EPTNULL){

		fprintf(stderr, "%s\n", MSJ_ERR_INV);//cambiar mensaje
		return EXIT_FAILURE;
	}

	if(!strcmp(arg.logfile_n, DEFAULT_LOGFILE)){
		flog = stderr;
	}

	else{
		if((flog = fopen(arg.logfile_n, "w")) == NULL){
		fprintf(stderr, "%s\n", MSJ_ERR_INV);//cambiar mensaje
		return EXIT_FAILURE;
		}
	}

	if(!strcmp(arg.infile_n, DEFAULT_INFILE)){
		fin = stdin;
	}

	else{
		if((fin = fopen(arg.infile_n, "rb")) == NULL){
		fprintf(flog, "%s\n", MSJ_ERR_INV);//cambiar mensaje
		return EXIT_FAILURE;
		}
	}
	if(!strcmp(arg.outfile_n, DEFAULT_OUTFILE)){
		fout = stdout;
	}

	else{
		if((fout = fopen(arg.outfile_n, "w")) == NULL){
			fprintf(flog, "%s\n", MSJ_ERR_INV);//cambiar mensaje
			return EXIT_FAILURE;
		}
	}

	// este while es para hacer pruebas, no es parte del programa
	while(fgets(linea, MAX_LINE, fin)){

		fprintf(fout, "%s", linea);

		
	}
	
	return EXIT_SUCCESS;
}
