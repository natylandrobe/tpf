#include "structData.h"

#define DEF_ELE 0

struct trkpt{
	struct fecha f;
	double lat;
	double lon;
	double ele;
};


typedef struct nodo{
	struct trkpt *mensaje;
	struct nodo *sig;
} nodo_t;

typedef struct nodo *lista_t;
struct trkpt *cargar_trkpt(const struct fecha *fecha, const double lat, const double lon, const double ele);

struct trkpt *proc_gga(struct s_GGA * dato){
	if(!dato){
		return NULL;
	}

	return cargar_trkpt(&(dato->f), dato->lat, dato->lon, dato->ele);
}


struct trkpt *cargar_trkpt(const struct fecha *fecha, const double lat, const double lon, const double ele){
	struct trkpt *aux;

	if(!fecha){
		return NULL;
	}

	aux = (struct trkpt *)malloc(sizeof(struct trkpt));
	if(!aux){
		return NULL;
	}

	aux->f = *fecha;
	aux->lat = lat;
	aux->lon = lon;
	aux->ele = ele;
	
	return aux;
}