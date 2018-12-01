#ifndef PRINT_H
#define PRINT_H

#ifndef STRUCTDATA_H
#include "structData.h"
#endif

#define TRK "<trk>"
#define TRK_C "</trk>"
#define TRKSEG "<trkseg>"
#define TRKSEG_C "</trkseg>"
#define TRKPT_START "<trkpt lat=\""
#define TRKPT_MID "\" lon=\""
#define TRKPT_FIN "\">"
#define ELE "<ele>"
#define ELE_C "</ele>"
#define TRKPT_C "</trkpt>"
#define SPC  "  "
#define SPC2 "    "
#define SPC3 "      "
#define T "T"
#define Z "Z"
#define XML "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define GPX "<gpx version=\"1.1\" creator=\"ubxnmea-gpx\" xmlns=\"http://www.topografix.com/GPX/1/1\">"
#define GPX_C "</gpx>"

#define META "<metadata>"
#define META_C "</metadata>"
#define NAME "<name>"
#define NAME_C "</name>"
#define TIME "<time>"
#define TIME_C "</time>"
#define YEAR_DIFF 1900
#define MON_DIFF 1

#define HELP "Utilice en cualquier orden los siguientes argumentos:\n-n o --name para ingresar el nombre de la ruta\n-i o --infile para ingresar el nombre del archivo de entrada (utilice - para stdin)\n-o o --outfile para ingresar el nombre del archivo de salida (utilice - para stdout)\n-l o --logfile para ingresar el nombre del archivo log (utilice - para stderr)\n-p o --protocol para indicar el protocolo a leer\n-m o --maxlen para indicar la cantidad maxima de mensajes a leer\n"

bool printMetadata(char *name, struct fecha *fecha, FILE *fout);
bool printTrkC(FILE *fout);
bool printStruct(struct trkpt *track, FILE *fout);
void printHelp(void);

#endif
