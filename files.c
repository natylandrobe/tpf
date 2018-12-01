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

void imp_log(FILE *flog, status_t * status, ubxst_t *ubx_st){

    printf("entra a imp_log\n");

    if(!flog){
        return ;
    }

    if(status){
        switch(*status){
            case ST_INV:
                fprintf(stderr, "%s\n", MSJ_ESTINV);
                break;
            case ST_EPTNULL:
                fprintf(stderr, "%s\n", MSJ_EPTNULL);
                break;
            case ST_ENOMEM:
                fprintf(stderr, "%s\n", MSJ_ENOMEM);
                break;
            case ST_EAGR:
                fprintf(flog, "%s\n", MSJ_EAGR);
                break;
            case ST_EFILEO:
                fprintf(flog, "%s\n", MSJ_EFILEO);
                break;
            case ST_EFILEC:
                fprintf(flog, "%s\n", MSJ_EFILEC);
                break;
            default:
                return ;
        }
    }

    if(ubx_st){
        switch(*ubx_st){
            case S_EPTNULL:
                fprintf(stderr, "%s\n", MSJ_EPTNULL);
                break;
            case S_ENOMEM:
                fprintf(stderr, "%s\n", MSJ_ENOMEM);
                break;
            case S_EREAD:
                fprintf(flog, "%s\n", MSJ_EOF);
                break;
            case S_CLASS_INV:
                fprintf(flog, "%s\n", MSJ_CLASSINV);
                break;
            case S_ID_INV:
                fprintf(flog, "%s\n", MSJ_IDINV);
            case S_CK_INV:
                fprintf(flog, "%s\n", MSJ_CKINV);
                break;
            case S_LARGO_INV:
                fprintf(flog, "%s\n", MSJ_LARGONIV);
                break;
            case S_FIX_INV:
                fprintf(flog, "%s\n", MSJ_FIXINV);
                break;
            case S_EAGR:
                fprintf(flog, "%s\n", MSJ_EAGR);
        }
    }

}