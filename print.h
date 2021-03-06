#ifndef PRINT_H
#define PRINT_H

#include "structData.h"

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
#define NAME_CT "</name>"
#define TIME "<time>"
#define TIME_C "</time>"
#define YEAR_DIFF 1900
#define MON_DIFF 1

#define HELP "Utilice en cualquier orden los siguientes argumentos:\n-n o --name para ingresar el nombre de la ruta\n-i o --infile para ingresar el nombre del archivo de entrada (utilice - para stdin)\n-o o --outfile para ingresar el nombre del archivo de salida (utilice - para stdout)\n-l o --logfile para ingresar el nombre del archivo log (utilice - para stderr)\n-p o --protocol para indicar el protocolo a leer\n-m o --maxlen para indicar la cantidad maxima de mensajes a leer\n"

/*defines imp_log*/

#define MSJ_ESTINV "[ERROR] Ingrese un argumento valido"
#define MSJ_EPTNULL "[ERROR] Puntero Nulo"
#define MSJ_ENOMEM "[ERROR] No se puede alocar la memoria"
#define MSJ_EAGR "[WARNING] No se pudo agregar el nodo"
#define MSJ_EFILEO "[ERROR] No se pudo abrir correctamente un archivo"
#define MSJ_EFILEC "[ERROR] No se pudo cerrar correctamente un archivo"
#define MSJ_ESENTINV "[WARNING] Se descarta sentencia invalida"
#define MSJ_CLASSINV "[WARNING] No se reconoce la clase"
#define MSJ_IDINV "[WARNING] No se reconoce el ID"
#define MSJ_CKINV "[ERROR] No concuerda el checksum"
#define MSJ_LARGONIV "[ERROR] Mensaje contiene largo invalido"
#define MSJ_FIXINV "[WARNING] Mensaje contiene fix invalido"
#define MSJ_EOF "[DEBUG] Se termino de leer el archivo"
#define MSJ_SYNC "[DEBUG] Buscando bytes de sincronismo"
#define MSJ_IDD "[DEBUG] ID detectado"
#define MSJ_MSJOK "[DEBUG] Mensaje recolectado"
#define MSJ_CARGA "[DEBUG] Mensaje cargado a la lista"
#define MSJ_IMP "[DEBUG] Mensaje impreso"
#define MSJ_FULLLIST "[WARNING] Se descartan mensajes por lista llena"

status_t printMetadata(const char *name, const struct fecha *fecha, FILE *fout);
status_t printTrkC(FILE *fout);
status_t printStruct(const struct trkpt *track, FILE *fout);
void printHelp(void);
void imp_log(FILE *flog, const status_t * status, const ubxst_t *ubx_st, const debug_t *deb);

#endif
