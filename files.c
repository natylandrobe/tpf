#include "files.h"



status_t abrir_archivos(FILE **fin, FILE **fout, FILE **flog, struct args *arg){

    if(!fin || !fout || !flog || !arg){
        return ST_EPTNULL;
    }

    if(!strcmp(arg->logfile_n, DEFAULT_LOGFILE)){
		*flog = stderr;
	}

	else{
		if((*flog = fopen(arg->logfile_n, "w")) == NULL){
		fprintf(stderr, "%s\n", "MSJ_ERR_OPENL");//cambiar mensaje
		return ST_EFILEO;
		}
	}

	if(!strcmp(arg->infile_n, DEFAULT_INFILE)){
		*fin = stdin;
	}

	else{
		if((*fin = fopen(arg->infile_n, "rb")) == NULL){
		fprintf(*flog, "%s\n", "MSJ_ERR_OPENI");//cambiar mensaje
		return ST_EFILEO;
		}
	}
	if(!strcmp(arg->outfile_n, DEFAULT_OUTFILE)){
		*fout = stdout;
	}

	else{
		if((*fout = fopen(arg->outfile_n, "w")) == NULL){
			fprintf(*flog, "%s\n", "MSJ_ERR_OPENO");//cambiar mensaje
			return ST_EFILEO;
		}
	}

    return ST_OK;
}

status_t cerrar_archivos(FILE **fin, FILE **fout, FILE **flog, struct args *arg){

    if(!fin || !fout || !flog || !arg){
        return ST_EPTNULL;
    }

    if(strcmp(arg->infile_n, DEFAULT_INFILE)){
		if(fclose(*fin)){
            return ST_EFILEC;
        }
	}

	if(strcmp(arg->outfile_n, DEFAULT_OUTFILE)){
		if(fclose(*fout)){
            return ST_EFILEC;
        }
	}

	if(strcmp(arg->logfile_n, DEFAULT_LOGFILE)){
		if(fclose(*flog)){
            return ST_EFILEC;
        }
        *flog = stderr;
	}

    return ST_OK; 
}

