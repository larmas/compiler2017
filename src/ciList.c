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
            printf(" %s\n ",a->info->var.id);
        } else {
            printf(" NULL ");
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
        }else{
             printf(" NULL ");
        }

        break;
    case 2:
        if(a != NULL){
            printf(" %s ",a->info->op.id);

        }else{
            printf(" NULL ");
        }
        break;
    default:
        //showFunc(p);
        break;
    }
}
}

void showCIList(CIList *l){
    CIList *p;
    p = l;
    while (p != NULL) {
        printf("\n");
        printf("%s", p->node->codOp);
        showCiNode(p->node->firstOp);
        showCiNode(p->node->secondOp);
        showCiNode(p->node->temp);
        p = p->next;
    }
}

#endif
