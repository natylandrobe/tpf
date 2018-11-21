#include "funcmain.h"

int main (int argc, char *argv[]){

	FILE *fin, *fout, *flog;
	char linea[MAX_LINE];
	struct args arg;
	struct data track;	
	struct fecha date;
	status_t st;

	if((st = defaultArgs(&arg)) == ST_EPTNULL){
		fprintf(stderr, "%s\n", MSJ_ERR_INV); //este creo que deberia quedar en stderr, cambiar mensaje
		return EXIT_FAILURE;
	}

	else if(st == ST_ENOMEM){
		fprintf(stderr, "%s\n", MSJ_ERR_INV); //este creo que deberia quedar en stderr, cambiar mensaje
		return EXIT_FAILURE;
	}

	if((st = takeArgs(argc, argv, &arg)) == ST_HELP){

		printHelp();
		return EXIT_SUCCESS;
	}
	
	else if(st == ST_INV){

		fprintf(stderr, "%s\n", MSJ_ERR_INV);//poner el archivo correspondientes
		return EXIT_FAILURE;
	}

	else if(st == ST_EPTNULL){

		fprintf(stderr, "%s\n", MSJ_ERR_INV);//poner el archivo y mensaje correspondientes
		return EXIT_FAILURE;
	}

	if(!strcmp(arg.logfile_n, DEFAULT_LOGFILE)){
		flog = stderr;
	}

	else{
		if((flog = fopen(arg.logfile_n, "w")) == NULL){
		fprintf(stderr, "%s\n", MSJ_ERR_INV);//poner el archivo y mensaje correspondientes
		return EXIT_FAILURE;
		}
	}

	if(!strcmp(arg.infile_n, DEFAULT_INFILE)){
		fin = stdin;
	}

	else{
		if((fin = fopen(arg.infile_n, "rb")) == NULL){
		fprintf(stderr, "%s\n", MSJ_ERR_INV);//poner el archivo y mensaje correspondientes
		return EXIT_FAILURE;
		}
	}

	if(!strcmp(arg.outfile_n, DEFAULT_OUTFILE)){
		fout = stdout;
	}

	else{
		if((fout = fopen(arg.outfile_n, "w")) == NULL){
		fprintf(stderr, "%s\n", MSJ_ERR_INV);//poner el archivo y mensaje correspondientes
		return EXIT_FAILURE;
	}
	/*esto queda igual*/
	if(!printMetadata(name)){

		fprintf(stderr, "%s\n", MSJ_ERR_INV);//poner el archivo y mensaje correspondientes
		return EXIT_FAILURE;
	}

	while (fgets(linea, MAX_LINE, stdin) != NULL){
		
		if(checkLine(linea) && cargarStruct(linea, &track, date)){
			
			printStruct(track);
		}
	}

	printTrkC();
	return EXIT_SUCCESS;
}
