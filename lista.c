#include "lista.h"

/*libera la memoria de la lista*/
status_t destruir_lista(lista_t *l){
	nodo_t *siguiente;
	
	if(!l){
		return ST_EPTNULL;
	}
	if(!(*l)){
		return ST_OK;
	}

	siguiente = (*l)->sig;

	if(destruir_nodo(l) != ST_OK){
		return ST_EPTNULL;
	}

	return destruir_lista(&siguiente);
}

/*libera la memoria del nodo*/
status_t destruir_nodo(lista_t *l){

	if(!l || !(*l)){
		return ST_EPTNULL;
	}
	free((*l)->mensaje);
	free(*l);
	*l = NULL;
	return ST_OK;
}

/*imprime nodo por nodo la lista*/
status_t imprimir_lista(lista_t l, FILE *fout, FILE *flog){

	debug_t deb;

	if(!fout || !flog){
		return ST_EPTNULL;
	}
	while(l){
		printStruct(l->mensaje, fout);
		deb = IMP_MSJ;
		imp_log(flog, NULL, NULL, &deb);
		l = l->sig;
	}
	return ST_OK;
}

/*toma un puntero a lista y le asigna NULL a la lista apuntada*/
status_t crear_lista(lista_t *l){
	if(!l){
		return ST_EPTNULL;
	}
	*l = NULL;
	return ST_OK;
}

/*toma un puntero a cualquiera de las estructuras de las sentencias y agrega un nodo al final de la lista*/
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

/*toma un puntero a cualquier estructura de las sentencias y crea un nodo a partir de esta*/
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

/*toma los datos de una estructura y carga una del tipo trkpt*/
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

/*toma una estructura cualquiera de las sentencias y pasa los datos correspondientes para crear la trkpt de acuerdo al tipo de sentencia*/
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

