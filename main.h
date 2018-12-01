#ifndef MAIN_H
#define MAIN_H

#include "print.h"
#include "check.h"
#include "args.h"

#define MAX_LINE 300 
#define MSJ_ERR_INV "Ingrese un dato valido"

//bool cargarStruct(char *s, struct data * new, struct fecha date);
status_t defaultArgs(struct args *arg);
bool cargar_struct_zda( char *s, struct s_ZDA *Zda, struct fecha *date);
bool cargar_struct_rmc(char *s, struct s_RMC *Rmc, struct fecha *date);
bool cargar_struct_gga(char *s,struct s_GGA *Gga,struct fecha *date);
#endif
