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

void showCIList(CIList *l){
    CIList *p;
    p = l;
    while (p != NULL) {
        printf("%s\n", p->node->codOp);
        showNode(p->node->firstOp);
        showNode(p->node->secondOp);
        showNode(p->node->temp);
        p = p->next;
    }
}
