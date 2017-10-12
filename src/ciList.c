#ifndef CILIST_C
#define CILIST_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.c"

typedef struct nodeCI{
    char codOp[20];
    Node *firstOp;
    Node *secondOp;
    Node *temp;
}NodeCI;

typedef struct ciList{
    NodeCI *node;
    struct ciList *next;
}CIList;

/*PROTOTIPOS*/
CIList *newCIList(CIList *l);
NodeCI *newNodeCI(char _codOp[], Node *_firstOp, Node *_secondOp, Node *_temp);
CIList *insertLastCI(CIList *l, NodeCI *_node);
void showCIList(CIList *l);
//Node *findElemCI(CIList *l, char _codOp[],int tag);


CIList *newCIList(CIList *l) {
    l = (CIList *) malloc(sizeof(CIList));
    l = NULL;
    return l;
}

NodeCI *newNodeCI(char _codOp[], Node *_firstOp, Node *_secondOp, Node *_temp){
    NodeCI *new = (NodeCI *)malloc(sizeof(NodeCI));
    strcpy(new->codOp, _codOp);
    new->firstOp = _firstOp;
    new->secondOp = _secondOp;
    new->temp = _temp;
    return new;
}

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
        break;
    case 2:
        if(a != NULL){
            printf(" %s ",a->info->op.id);
        }
        break;
    case 3:
        if(a != NULL){
            printf("%s",a->info->func.id);
        }
        break;
    default:
        printf(" LISTA VACIA ");
    }
}
}

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
