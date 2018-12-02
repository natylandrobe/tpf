#include "print.h"

/* Imprime la metadata en XML en el archivo de salida*/
status_t printMetadata(char *name, struct fecha *fecha, FILE *fout){

   	if(!name || !fecha || !fout){

   		return ST_EPTNULL;
   	}

   	
   	fprintf(fout, "%s\n", XML);
	fprintf(fout, "%s\n", GPX);
	fprintf(fout, "%s%s\n%s%s%s%s\n%s%s%d-%d-%d%s%d:%d:%.0f%s%s\n%s%s\n%s%s\n%s%s\n",
		SPC,META,
		SPC2,NAME,name,NAME_CT,
		SPC2,TIME,
		fecha->anio,
		fecha->mes,
		fecha->dia,
		T,
		fecha->hora,
		fecha->minutos,
		fecha->segundos,
		Z,
		TIME_C,
		SPC,META_C,
		SPC,TRK,
		SPC2,TRKSEG);

	return ST_OK;
}

/* Imprime cierres de los tags Trackseg, Track y gpx en el archivo de salida*/
status_t printTrkC(FILE *fout){

	if(!fout){
		return ST_EPTNULL;
	}

	fprintf(fout, "%s%s\n%s%s\n%s\n",
			SPC2,TRKSEG_C,
			SPC, TRK_C,
			GPX_C);
	return ST_OK;
}

/* Imprime la estructura en formato XML en el archivo de salida*/
status_t printStruct(struct trkpt *track, FILE *fout){

	if(!track || !fout){
		return ST_EPTNULL;
	}

	fprintf(fout, "%s%s%f%s%f%s\n%s%s%s%f%s\n%s%s%s%d-%d-%d%s%d:%d:%.3f%s%s\n%s%s\n",
		SPC3, TRKPT_START,
		track->lat,
		TRKPT_MID,
		track->lon,
		TRKPT_FIN,
		SPC3, SPC, ELE,
		track->ele,
		ELE_C,
		SPC3, SPC, TIME,track->f.anio, track->f.mes, track->f.dia,
		T,track->f.hora,track->f.minutos,track->f.segundos,
		Z, TIME_C,
		SPC3, TRKPT_C);

	return ST_OK;
}

/* Imprime mensaje de ayuda */
void printHelp(void){

	printf("%s\n", HELP);
}

//de acuerdo al tipo de pointer ingresado imprime mensajes de error, warning y debug en el archivo log
void imp_log(FILE *flog, status_t * status, ubxst_t *ubx_st, debug_t *deb){

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
            case ST_SENTINV:
                fprintf(flog, "%s\n", MSJ_ESENTINV);
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
                break;
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
                break;
            default:
                return ;
        }
    }

    if(deb){
        switch(*deb){
            case SYNC:
                fprintf(flog, "%s\n", MSJ_SYNC);
                break;
            case ID_D:
                fprintf(flog, "%s\n", MSJ_IDD);
                break;
            case MSJ_OK:
                fprintf(flog, "%s\n", MSJ_MSJOK);
                break;
            case CARGA_MSJ:
                fprintf(flog, "%s\n", MSJ_CARGA);
                break;
            case IMP_MSJ:
                fprintf(flog, "%s\n", MSJ_IMP);
                break;
            case LIST_FULL:
                fprintf(flog, "%s\n", MSJ_FULLLIST);
                break;
            default:
                return ;
        }

    }

}