#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.c"


typedef struct list {
  Node *node;
  struct list *next;
}List;


/*PROTOTIPOS*/
List *newList(List *l);
List *insertLast(List *l, Node *dato);
List *insertFirst(List *l, Node *dato);
Node *findElem(List *l, char _id[],int tag);
void deleteList(List **l);
void showList(List *l);
int longList(List *l);
List *deleteElem(List *l, char _id[],int tag);
List *insertParam(List *l, Node *dato);


List *newList(List *l) {
    l = (List *) malloc(sizeof(List));
    l = NULL;
    return l;
}


Node *findElem(List *l, char _id[], int tag ) {
    int cond;
    if (l == NULL){  // elemento no encontrado
        return NULL;
    }
    if(tag == 0) { cond = strcmp(l->node->info->var.id,_id);}
    else { cond = strcmp(l->node->info->func.id,_id);}
  
    if (cond == 0) {
        return (l->node);
    }else{
        return ( findElem(l->next,_id,tag) );
    }
    return NULL;
}


List *insertLast(List *l, Node *dato) {
	if(l != NULL){
        char _id[20];
        if(dato->tag == 0){
            strcpy(_id, dato->info->var.id);
        }
        else{
            strcpy(_id, dato->info->func.id);
        }
        Node *exists = findElem(l,_id,dato->tag);
    
        if(exists == NULL){   //El elemento no existe en la lista actualmente
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
            if(dato->tag == 0){
              printf("%s%s%s%i\n","Error: Variable ",_id," y declarada en la linea ",dato->noline);
              exit(1);
            }else{
              printf("%s%s%s%i\n","Error: Metodo ",_id, " y declaradon la linea ",dato->noline);
              exit(1);  
            }  
        }     
    }else{
        List *q;
        q = (List *) malloc(sizeof(List));
    	q->node = dato;
    	q->next = NULL;
    	l = q;
    }
    return l;
}


List *insertFirst(List *l, Node *dato) {
	if(l !=NULL){

        char _id[20];
        if(dato->tag == 0){
            strcpy(_id, dato->info->var.id);
        }else{
            strcpy(_id, dato->info->func.id);
        }
        Node *exists = findElem(l,_id,dato->tag); 

        if(exists == NULL){   //El elemento no existe en la lista actualmente

    	   List *q;
    	   q = (List *) malloc(sizeof(List));
    	   q->node = dato;
    	   q->next = l;
    	   l = q;
        }else{
            if(dato->tag == 0){
              printf("%s%s%s%i\n","Error: Variable ",_id," ya declarada en la linea ",dato->noline);
              exit(1);
            }else{
              printf("%s%s%s%i\n","Error: Metodo ",_id, " ya declaradon la linea ",dato->noline);
              exit(1);  
            }  
        }  
    }else{
        List *q;
        q = (List *) malloc(sizeof(List));
    	q->node = dato;
    	q->next = NULL;
    	l = q;
    }
    return l;
}


List *insertParam(List *l, Node *dato) {   //permite insertar repetidos
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




List *deleteElem(List *l, char _id[], int tag) {
    int cond;
    if (l == NULL)
        return l;
    if(tag == 0) {
        cond = strcmp(l->node->info->var.id, _id);
    }else {
        cond = strcmp(l->node->info->func.id, _id);
    }
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


void deleteList(List **l) {
    List *q;
    while (*l != NULL) {
        q = (*l)->next;
        free(*l);
        *l = q;
    }
}

void showList(List *l) {
    List *p;
    p = l;
    while (p != NULL) {
        showNode(p->node);
        p = p->next;
    }
}


int main(int argc, char const *argv[]) {
    /*Node *var1 = newVar("x",0,10,50);
    Node *var2 = newVar("x",1,0,50);
    Node *var3 = newVar("z",0,20,50);
    Node *var4 = newVar("x",1,1,50);
    insertTree(root,left,right);
    preorden(root);*/
    if(NULL == 0){printf("NULL ES CEROOOOOO!");}
   /* List *test;
    test = newList(test);
    test = insertFirst(test,var1);
    test = insertFirst(test,var2);
    test = insertLast(test,var3);
    test = insertLast(test,var4);
    deleteList(&test);
    showList(test);
    Node *find;
    find = findElem(test, "pepe");
    printf("%s\n","---------");
    printf("Elem.Buscado: %s\n",find->id);

    test = deleteElem(test,"fer");
    showList(test);
    int length = longList(test);
    printf("%i\n", length);*/
    return 0;
}

