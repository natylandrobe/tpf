#include "structData.h"

struct trkpt{
	struct fecha f;
	double lat;
	double lon;
	double ele;
};


typedef struct nodo{
	struct trkpt mensaje;
	struct nodo *sig;
} nodo_t;

typedef struct nodo *lista_t;