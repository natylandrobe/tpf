#include "structData.h"
#include "print.h"

#define DEF_ELE 0


typedef struct nodo{
	struct trkpt *mensaje;
	struct nodo *sig;
} nodo_t;

typedef enum {ST_INV, ST_HELP, ST_OK, ST_EPTNULL, ST_ENOMEM, ST_EAGR} status_t; //desp incluir el .h de donde este

typedef struct nodo *lista_t;
struct trkpt *cargar_trkpt(const struct fecha *fecha, const double lat, const double lon, const double ele);
struct trkpt *proc_sentencias(void * dato, sent_t tipo);
nodo_t *crear_nodo(void *dato, struct trkpt *(*procesar)(void *, sent_t), sent_t tipo);
status_t agregar_nodo(void * dato, lista_t *l, sent_t tipo);
status_t crear_lista(lista_t *l);
status_t imprimir_lista(lista_t l, FILE *pf);
status_t destruir_lista(lista_t *l);
void destruir_nodo(lista_t *l);

status_t destruir_lista(lista_t *l){
	nodo_t *siguiente;
	
	if(!l){
		return ST_EPTNULL;
	}
	if(!(*l)){
		return ST_OK;
	}

	siguiente = (*l)->sig;

	destruir_nodo(l);

	return destruir_lista(&siguiente);
}

void destruir_nodo(lista_t *l){

	if(!l || !(*l)){
		return ;
	}
	free((*l)->mensaje);
	free(*l);
	*l = NULL;
}

status_t imprimir_lista(lista_t l, FILE *pf){
	if(!pf || !l){
		return ST_EPTNULL;
	}
	while(l){
		printStruct(l->mensaje);
		l = l->sig;
	}
	return ST_OK;
}

status_t crear_lista(lista_t *l){
	if(!l){
		return ST_EPTNULL;
	}
	*l = NULL;
	return ST_OK;
}

status_t agregar_nodo(void * dato, lista_t *l, sent_t tipo){
	nodo_t *temp, *aux;
	if(!dato || !l){
		return ST_EPTNULL;
	}
	if((aux = crear_nodo(dato, &proc_sentencias, tipo)) == NULL){
		return ST_EAGR;
	}
	if((*l) == NULL){
		*l = aux;
	}
	else{
		temp = *l;
		while(temp->sig)
			temp = temp->sig;

		temp->sig = aux;
	}
	return ST_OK;
}

nodo_t *crear_nodo(void *dato, struct trkpt *(*procesar)(void *, sent_t), sent_t tipo){
	nodo_t *pn;
	
	if(!dato || !procesar){
		return NULL;
	}

	pn = (nodo_t *)malloc(sizeof(nodo_t));
	if(!pn){
		return NULL;
	}

	if(!(pn->mensaje = (*procesar)(dato, tipo))){
		free(pn);
		pn = NULL;
		return NULL;
	}
	pn->sig = NULL;

	return pn;
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

struct trkpt *proc_sentencias(void * dato, sent_t tipo){
	if(!dato){
		return NULL;
	}

	switch(tipo){
		case GGA:
			return cargar_trkpt(&(((struct s_GGA *) dato)->f), ((struct s_GGA *) dato)->lat, ((struct s_GGA *) dato)->lon, ((struct s_GGA *) dato)->ele);
		case RMC:
			return cargar_trkpt(&(((struct s_RMC *) dato)->f), ((struct s_RMC *) dato)->lat, ((struct s_RMC *) dato)->lon, DEF_ELE);
		case NAV_PVT:
			return cargar_trkpt(&(((struct s_PVT *) dato)->f), ((struct s_PVT *) dato)->lat, ((struct s_PVT *) dato)->lon, ((struct s_PVT *) dato)->hmsl);
		case NAV_POSLLH:
			return cargar_trkpt(&(((struct s_POSLLH *) dato)->f), ((struct s_POSLLH *) dato)->lat, ((struct s_POSLLH *) dato)->lon, ((struct s_POSLLH *) dato)->hmsl);
		default:
			return NULL;
	}

	
}

