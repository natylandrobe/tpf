#ifndef FILES_H
#define FILES_H
#include "structData.h"
#include "ubx.h"

//defines imp_log

#define MSJ_ESTINV "[ERROR] Ingrese un argumento valido"
#define MSJ_EPTNULL "[ERROR] Puntero Nulo"
#define MSJ_ENOMEM "[ERROR] No se puede alocar la memoria"
#define MSJ_EAGR "[WARNING] No se pudo agregar el nodo"
#define MSJ_EFILEO "[ERROR] No se pudo abrir correctamente un archivo"
#define MSJ_EFILEC "[ERROR] No se pudo cerrar correctamente un archivo"
#define MSJ_CLASSINV "[WARNING] No se reconoce la clase"
#define MSJ_IDINV "[WARNING] No se reconoce el ID"
#define MSJ_CKINV "[ERROR] No concuerda el checksum"
#define MSJ_LARGONIV "[ERROR] Mensaje contiene largo invalido"
#define MSJ_FIXINV "[WARNING] Mensaje contiene fix invalido"
#define MSJ_EOF "[DEBUG] Se termino de leer el archivo"

status_t abrir_archivos(FILE **fin, FILE **fout, FILE **flog, struct args *arg);
status_t cerrar_archivos(FILE **fin, FILE **fout, FILE **flog, struct args *arg);
void imp_log(FILE *flog, status_t * status, ubxst_t *ubx_st);

#endif