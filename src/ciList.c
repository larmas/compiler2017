#ifndef CILIST_C
#define CILIST_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.c"

/**
 * Estructura que representa un codigo de 3 direcciones.
 */
typedef struct nodeCI{
    char codOp[256];
    Node *firstOp;
    Node *secondOp;
    Node *temp;
}NodeCI;

/**
 * Estructura que representa una lista de codigos de 3 direcciones.
 */
typedef struct ciList{
    NodeCI *node;
    struct ciList *next;
}CIList;

/*PROTOTIPOS*/
CIList *newCIList(CIList *l);
NodeCI *newNodeCI(char _codOp[], Node *_firstOp, Node *_secondOp, Node *_temp);
CIList *insertLastCI(CIList *l, NodeCI *_node);
void showCiNode(Node *a);
void showCIList(CIList *l);

/**
 * Inicializa una lista de codigo de 3 direcciones en NULL.
 */
CIList *newCIList(CIList *l) {
    l = (CIList *) malloc(sizeof(CIList));
    l = NULL;
    return l;
}

/**
 * Inicializa un nodo que representa un codigo de 3 direcciones.
 */
NodeCI *newNodeCI(char _codOp[], Node *_firstOp, Node *_secondOp, Node *_temp){
    NodeCI *new = (NodeCI *)malloc(sizeof(NodeCI));
    strcpy(new->codOp, _codOp);
    new->firstOp = _firstOp;
    new->secondOp = _secondOp;
    new->temp = _temp;
    return new;
}

/**
 * Inserta un elemento al final de la lista.
 */
CIList *insertLastCI(CIList *l, NodeCI *_node){
    if(l != NULL){
    	CIList *p,*q;
    	q = (CIList *) malloc(sizeof(CIList));
        q->node = _node;
    	q->next = NULL;
    	p = l;
    	while (p->next != NULL){
    	    p = p->next;
        }
    	p->next = q;
    }else{
        CIList *q;
        q = (CIList *) malloc(sizeof(CIList));
        q->node = _node;
    	q->next = NULL;
    	l = q;
    }
    return l;
}

/**
 * Muestra por pantalla un nodo que representa un codigo de tres direcciones.
 */
void showCiNode(Node *a){
    switch ( a->tag ) {
    case 0:
        if(a != NULL){
            printf(" %s ",a->info->var.id);
        }
        break;
    case 1:
        if(a != NULL){
            if (a->type == 0){
                printf(" %i ",a->info->cons.value);
            }else{
                if(a->info->cons.value == 0){
                    printf(" false ");
                }else{
                    printf(" true ");
                }
            }
        }
        break;
    case 2:
        if(a != NULL){
            printf(" %s ",a->info->op.id);
        }
        break;
    case 3:
        if(a != NULL){
            printf(" %s ",a->info->func.id);
        }
        break;
    case 4:
        if(a != NULL){
            printf(" %s ",a->info->var.id);
        }
        break;
    default:
        break;
    }
}

/**
 * Muestra por pantalla una lista de codigo intermedio.
 */
void showCIList(CIList *l){
    CIList *p;
    p = l;
    while (p != NULL) {
        printf("%s", p->node->codOp);
        if (p->node->firstOp != NULL)
            showCiNode(p->node->firstOp);
        if (p->node->secondOp != NULL)
            showCiNode(p->node->secondOp);
        if (p->node->temp != NULL)
            showCiNode(p->node->temp);
        printf("\n");
        p = p->next;
    }
}

#endif
