#ifndef LIST_C
#define LIST_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.c"

/**
 * Estructura que representa una lista
 */
typedef struct list {
  Node *node;
  struct list *next;
}List;


/*PROTOTIPOS*/
List *newList(List *l);
List *insertLast(List *l, Node *dato);
List *insertFirst(List *l, Node *dato);
int exists(List *l, Node *dato);
Node *findElem(List *l, char _id[],int tag);
void deleteList(List **l);
void showList(List *l);
int longList(List *l);
List *deleteElem(List *l, char _id[],int tag);
List *insertParam(List *l, Node *dato);

/**
 * Inicializa una lista.
 */
List *newList(List *l) {
    l = (List *) malloc(sizeof(List));
    l = NULL;
    return l;
}

/**
 * Busca un elemento en la lista y lo retorna.
 */
Node *findElem(List *l, char _id[], int tag ) {
    int cond;
    if (l == NULL){
        return NULL;
    }
    if(tag == 0)
        cond = strcmp(l->node->info->var.id,_id);
    else
        cond = strcmp(l->node->info->func.id,_id);
    if (cond == 0)
        return (l->node);
    else
        return ( findElem(l->next,_id,tag) );
    return NULL;
}

/**
 * Inserta un elemento al final de la lista.
 */
List *insertLast(List *l, Node *dato) {
    if( exists(l,dato) == 1 ){
        if(l != NULL){
        	List *p,*q;
        	q = (List *) malloc(sizeof(List));
        	q->node = dato;
        	q->next = NULL;
        	p = l;
        	while (p->next != NULL){
        	    p = p->next;
            }
        	p->next = q;
        }else{
            List *q;
            q = (List *) malloc(sizeof(List));
        	q->node = dato;
        	q->next = NULL;
        	l = q;
        }
    }else{
        printf("%s%i\n", "El elemento ya existe. Linea: ",dato->noline);
        exit(1);
    }
    return l;
}

/**
 * Inserta un elemento al inicio de la lista.
 */
List *insertFirst(List *l, Node *dato) {
	if(exists(l,dato) == 1){
        if(l !=NULL){
        	List *q;
        	q = (List *) malloc(sizeof(List));
        	q->node = dato;
        	q->next = l;
        	l = q;
        }else{
            List *q;
            q = (List *) malloc(sizeof(List));
        	q->node = dato;
        	q->next = NULL;
        	l = q;
        }
    }else{
        printf("%s%i\n", "El elemento ya existe. Linea: ",dato->noline);
        exit(1);
    }
    return l;
}

/**
 * Inserta elementos en una lista, permite insertar repetidos.
 */
List *insertParam(List *l, Node *dato) {
    if(l !=NULL){
        List *q;
        q = (List *) malloc(sizeof(List));
        q->node = dato;
        q->next = l;
        l = q;
    }else{
        List *q;
        q = (List *) malloc(sizeof(List));
        q->node = dato;
        q->next = NULL;
        l = q;
    }
    return l;
}

/**
 * Retorna 0 si el elemento (variable o funcion) existe en la lista, 1 en caso contrario.
 */
int exists(List *l, Node *dato){
    int result;
    switch ( dato->tag ) {
    case 0:
        if(findElem(l,dato->info->var.id,dato->tag) == NULL){
            result = 1;
        }else{
            result = 0;
        }
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        if(findElem(l,dato->info->func.id,dato->tag) == NULL){
            result = 1;
        }else{
            result = 0;
        }
        break;
    }
    return result;
}

/**
 * Busca un elemento y si existe, lo elimina de la lista.
 */
List *deleteElem(List *l, char _id[], int tag) {
    int cond;
    if (l == NULL)
        return l;
    if(tag == 0)
        cond = strcmp(l->node->info->var.id, _id);
    else
        cond = strcmp(l->node->info->func.id, _id);
    if (cond == 0) {
        List *q;
        q = l->next;
        free(l);
        l = q;
        return l;
    }else
        l->next = deleteElem(l->next,_id,tag);
    return l;
}

/**
 * Retorna la longitud de la lista.
 */
int longList(List *l) {
    List *p;
    int n;
    n = 0;
    p = l;
    while (p != NULL) {
        ++n;
        p = p->next;
    }
    return n;
}

/**
 * Elimina una lista.
 */
void deleteList(List **l) {
    List *q;
    while (*l != NULL) {
        q = (*l)->next;
        free(*l);
        *l = q;
    }
}

/**
 * Muestra una lista de nodos por pantalla.
 */
void showList(List *l) {
    List *p;
    p = l;
    while (p != NULL) {
        showNode(p->node);
        p = p->next;
    }
}

#endif
