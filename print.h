#ifndef PRINT_H
#define PRINT_H

#ifndef STRUCTDATA_H
#include "structData.h"
#endif


bool printMetadata(char *name, struct fecha *fecha, FILE *fout);
bool printTrkC(FILE *fout);
bool printStruct(struct trkpt *track, FILE *fout);
void printHelp(void);

#endif
