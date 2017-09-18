#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.c"

typedef struct list {
  Node *node;
  struct list *next;
}List;

/*PROTOTIPOS*/
List *newList(List *l, Node *p);
void insertLast(List *l, Node *dato);
void insertFirst(List **l, Node *dato);
Node *findElem(List *l, char _id[],int tag);
void deleteList(List **l);
void showList(List *l);
int longList(List *l);
List *deleteElem(List *l, char _id[],int tag);


// Inicializa la lista con un nodo
List *newList(List *l, Node *p) {
    l = (List *) malloc(sizeof(List));
    l->node = p;
    l->next = NULL;
    return l;
}


// Inserta el dato al final de la lista
void insertLast(List *l, Node *dato) {
    List *p,*q;
    q = (List *) malloc(sizeof(List));
    q->node = dato;
    q->next = NULL;
    p = l;
    while (p->next != NULL)
        p = p->next;
    p->next = q;
}


// Inserta el dato al comienzo de la lista
void insertFirst(List **l, Node *dato) {
    List *q;
    q = (List *) malloc(sizeof(List));
    q->node = dato;
    q->next = *l;
    *l = q;
}


// Retorna el elemento si fue en contrado en la lista, busca por ID. (0:Busqueda de variable, otherwise:Busqueda de funcion)
Node *findElem(List *l, char _id[], int tag ) {
    int cond;
    if (l == NULL){  // elemento no encontrado
        return NULL;
    }

    if(tag == 0) {cond = strcmp(l->node->info->var.id,_id);}  // 0:variable
    else {cond = strcmp(l->node->info->func.id,_id);}         // else:funcion

    if (cond == 0) {  // encontrado!
  	    return (l->node);
    }else{
        return ( findElem(l->next,_id,tag) );
    }
    return NULL;
}


// Elimina el elemento si se encuentra en la lista, busca por ID. (0:Busqueda de variable, otherwise:Busqueda de funcion)
List *deleteElem(List *l, char _id[], int tag) {
    int cond;
    if (l == NULL)
        return l;

    if(tag == 0) {cond = strcmp(l->node->info->var.id, _id);}  // 0:variable
    else {cond = strcmp(l->node->info->func.id, _id);}         // else:funcion

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


// Devuelve la longitud de una lista
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


// Anula una lista liberando la memoria
void deleteList(List **l) {
    List *q;
    while (*l != NULL) {
        q = (*l)->next;
        free(*l);
        *l = q;
    }
}


// Imprimir la lista
void showList(List *l) {
    List *p;
    p = l;
    while (p != NULL) {
        switch ( p->node->tag ) {
        case 0:
            showVar(p->node);
            break;
        case 1:
            showConst(p->node);
            break;
        case 2:
            showOp(p->node);
            break;
        default:
            showFunc(p->node);
            break;
        }
        p = p->next;
    }
}


  /*int main(int argc, char const *argv[]) {
  Node *root;
  Node *left;
  Node *right;
  Node *aux;
  Node *aux2;

  root = newNode(0,"pepe",10);
  left = newNode(0,"jose",20);
  right = newNode(0,"juan",30);
  aux = newNode(0,"lucas",40);
  aux2 = newNode(0,"fer",48);
  //insertTree(root,left,right);
  //preorden(root);*/

  /* Prueba lista */

  /*List *test;
  test = newList(test,root);
  insertLast(test,left);
  insertLast(test,right);
  insertFirst(&test,aux);
  insertFirst(&test,aux2);
  //deleteList(&test);
  showList(test);
  //Node *find;
  //find = findElem(test, "pepe");
  printf("%s\n","---------");
  //printf("Elem.Buscado: %s\n",find->id);

  test = deleteElem(test,"fer");
  showList(test);
  //int length = longList(test);
  //printf("%i\n", length);
  return 0;
}*/
