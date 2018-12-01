#ifndef ARGS_H
#define ARGS_H

#include <ctype.h>
#include "check.h"

#define ARG_HELP "--help"
#define ARG_NAME "--name"
#define ARG_PROT "--protocol"
#define ARG_INFILE "--infile"
#define ARG_OUTFILE "--outfile"
#define ARG_LOGFILE	"--logfile"
#define ARG_MAX "--maxlen"
#define DEFAULT_FILE "-"
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

#define MIN_DIG 0
#define MAX_DIG 9
#define CANT_ARGS 7
#define ARGC_IND 2


status_t procesar_arg(int argum, char *argv[], args_t *argp, size_t index);
status_t defaultFecha(struct fecha *def);
status_t liberar_args(struct args *arg);
status_t takeArgs(int argc, char *argv[], struct args *arg);


#endif
