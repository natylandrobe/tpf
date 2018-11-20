#include "funcmain.h"

int main (int argc, char *argv[]){

	FILE *fin, *fout, *flog;
	protocol_t protocol;
	char linea[MAX_LINE], *name, *fin_name, *fout_name, *flog_name;
	struct args arg;
	struct data track;	
	struct fecha date;
	status_t st;

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
