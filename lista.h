#ifndef LISTA_H
#define LISTA_H

#ifndef STRUCTDATA_H
#include "structData.h"
#endif
#include "print.h"

#define DEF_ELE 0


typedef struct nodo{
	struct trkpt *mensaje;
	struct nodo *sig;
} nodo_t;


typedef struct nodo *lista_t;
struct trkpt *cargar_trkpt(const struct fecha *fecha, const double lat, const double lon, const double ele);
struct trkpt *proc_sentencias(void * dato, sent_t tipo);
nodo_t *crear_nodo(void *dato, struct trkpt *(*procesar)(void *, sent_t), sent_t tipo);
status_t agregar_nodo(void * dato, lista_t *l, sent_t tipo);
status_t crear_lista(lista_t *l);
status_t imprimir_lista(lista_t l, FILE *pf);
status_t destruir_lista(lista_t *l);
void destruir_nodo(lista_t *l);

#endif