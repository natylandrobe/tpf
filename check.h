#ifndef CHECK_H
#define CHECK_H

#ifndef STRUCTDATA_H
#include "structData.h"
#endif


bool checkLine(char *s); 
bool checkDia(int dia); 
bool checkMes(int mes);
bool checkAnio(int anio);
bool checkNum(char *s);
bool checkMembers(double lat, double lon, cal_t cal, long int cant);
bool checkMembersrmc(double lat, double lon);

#endif
