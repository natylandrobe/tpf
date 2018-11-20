#ifndef DEFARGS_H
#define DEFARGS_H

#include <ctype.h>
#include <string.h>
#include "check.h"

#define DEFAULT_NAME "Default Name"
#define ARG_HELP "--help"
#define ARG_NAME "--name"
#define ARG_PROT "--protocol"
#define ARG_INFILE "--infile"
#define ARG_OUTFILE "--outfile"
#define ARG_LOGFILE	"--logfile"
#define ARG_MAX "--maxlen"
#define HELP_C 'h'
#define NAME_C 'n'
#define PROT_C 'p'
#define INFILE_C 'i'
#define OUTFILE_C 'o'
#define LOGFILE_C 'l'
#define MAX_C 'm'
#define ARG_C '-'
#define SPACE ' '
#define PROT_NMEA "nmea"
#define PROT_UBX "ubx"

//bool cargarFecha(char *s, struct fecha *date); mepa que no se usa
status_t defaultFecha(struct fecha *def);


#endif
