#include "print.h"

/* Imprime la metadata en XML*/
bool printMetadata(char *name, struct fecha *fecha, FILE *fout){

   	if(!name || !fecha || !fout){

   		return false;
   	}

   	
   	fprintf(fout, "%s\n", XML);
	fprintf(fout, "%s\n", GPX);
	fprintf(fout, "%s%s\n%s%s%s%s\n%s%s%d-%d-%d%s%d:%d:%.0f%s%s\n%s%s\n%s%s\n%s%s\n",
		SPC,META,
		SPC2,NAME,name,NAME_C,
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

	return true;
}

/* Imprime cierres de los tags Trackseg, Track y gpx */
bool printTrkC(FILE *fout){

	if(!fout){
		return false;
	}

	fprintf(fout, "%s%s\n%s%s\n%s\n",
			SPC2,TRKSEG_C,
			SPC, TRK_C,
			GPX_C);
	return true;
}

/* Imprime la estructura en formato XML */
bool printStruct(struct trkpt *track, FILE *fout){

	if(!track || !fout){
		return false;
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

	return true;
}

/* Imprime mensaje de ayuda */
void printHelp(void){

	printf("%s\n", HELP);
}