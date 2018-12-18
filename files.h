#ifndef FILES_H
#define FILES_H
#include "structData.h"


status_t abrir_archivos(FILE **fin, FILE **fout, FILE **flog, const struct args *arg);
status_t cerrar_archivos(FILE **fin, FILE **fout, FILE **flog, const struct args *arg);


#endif