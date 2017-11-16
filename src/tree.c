#ifndef TREE_C
#define TREE_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct node;
typedef struct node Node;

/**
 * Representación de un nodo que contiene una variable.
 */
typedef struct variable{
  char id[256];
  int value;
  int offset;
}TVar;

/**
 * Representación de un nodo que contiene una función.
 * El atributo param representa la lista de parametros que toma
 * una función.
 */
typedef struct funcion{
  char id[256];
  int offset;
  struct list *param;
  Node *AST;
}TFunc;

/**
 * Representación de un nodo que contiene una constante.
 */
typedef struct constante{
  int value;    // constante int o bool(0,1)
}TConst;

/**
 * Representación de un nodo que contiene un operador.
 */
typedef struct operador{
  char id[20];
}TOpe;

/**
 * Unión que identifica a un nodo como variable, constante, operador o
 * función.
 */
typedef union info{
  TVar var;
  TFunc func;
  TConst cons;
  TOpe op;
}TInfo;

/**
 *  Nodo utilizado para representar variables, constantes, operadores y funciones.
 *  El tag de un nodo indica si es una variable (tag=0), una constante (tag=1),
 *  un operador (tag=2), una función (tag=3) o un temporal (tag=4).
 *  Type indica el tipo del elemento, int = 0, boolean = 1 y void = 2.
 *  Mark es una marca utilizada en la función dfs para identificar nodos
 *  visitados (mark=-1) y no visitados (mark=0).
 */
struct node{
  int tag;
  int noline;
  int type;
  TInfo *info;
  struct node *left;
  struct node *mid;
  struct node *right;
  int mark;
};

/*PROTOTIPOS*/
Node *newVar(char xId[], int xType, int xValue, int xLine);
Node *newTemp(char xId[], int xType, int xValue, int xLine);
Node *newConst(int xType, int xValue, int xLine);
Node *newOp(char xId[], int xType, int xLine);
Node *newFunc(char xId[], int xType, struct list *xParam, Node *xAST,int xLine);
void showVar(Node *a);
void showConst(Node *a);
void showOp(Node *a);
void showFunc(Node *a);
void showNode(Node *p);
void insertTree(Node *raiz, Node *leafL, Node *leafM, Node *leafR);
void mark(Node *node);
void dfs(Node *root);
void checkType(Node *root);
void setOffset(Node *root, int xOffset);

/**
 * Inicialización de un nodo variable.
 */
Node *newVar(char xId[], int xType, int xValue, int xLine){
	Node *new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = 0;
    new->type = xType;
    new->noline = xLine;
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
    TInfo *i = (TInfo *) malloc(sizeof(TInfo));
    strcpy( i->var.id , xId );
    i->var.value = xValue;
    i->var.offset = 0;
    new->info = i;
    return new;
}

/**
 * Inicialización de un nodo temporal.
 */
Node *newTemp(char xId[], int xType, int xValue, int xLine){
	Node *new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = 4;
    new->type = xType;
    new->noline = xLine;
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
    TInfo *i = (TInfo *) malloc(sizeof(TInfo));
    strcpy( i->var.id , xId );
    i->var.value = xValue;
    i->var.offset = 0;
    new->info = i;
    return new;
}

/**
 * Inicialización de un nodo constante.
 */
Node *newConst(int xType, int xValue, int xLine){
	Node *new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = 1;
    new->noline = xLine;
    new->type = xType;
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
    TInfo *i = (TInfo *) malloc(sizeof(TInfo));
    i->cons.value = xValue;
    new->info = i;
    return new;
}

/**
 * Inicialización de un nodo operador.
 */
Node *newOp(char xId[], int xType, int xLine){
	Node *new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = 2;
    new->noline = xLine;
    new->type = xType;
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
    TInfo *i = (TInfo *) malloc(sizeof(TInfo));
    strcpy( i->op.id , xId );
    new->info = i;
    return new;
}

/**
 * Inicialización de un nodo función.
 */
Node *newFunc(char xId[], int xType, struct list *xParam, Node *xAST, int xLine){
	Node *new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = 3;
    new->noline = xLine;
    new->type = xType;
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
    TInfo *i;
    i = (TInfo *) malloc(sizeof(TInfo));
    strcpy( i->func.id , xId );
    i->func.param = xParam;
    i->func.offset = 0;
    i->func.AST = xAST;
    new->info = i;
	return new;
}

/**
 * Setea los hijos de un nodo. Como máximo cada nodo puede tener un total de
 * tres hijos.
 */
void insertTree(Node *root, Node *leafL, Node *leafM, Node *leafR){
    root->left = leafL;
    root->mid = leafM;
    root->right = leafR;
}

/**
 * Muestra por pantalla un nodo variable.
 */
void showVar(Node *a){
    if(a != NULL){
        printf("id: %s\n",a->info->var.id);
        printf("offset: %d\n", a->info->var.offset);
    } else {
        printf("Node NULL\n");
    }
    printf("\n");
}

/**
 * Muestra por pantalla un nodo constante.
 */
void showConst(Node *a){
    if(a != NULL){
        if (a->type == 0){
            printf("\nvalue:%i\n",a->info->cons.value);
        }else{
            if(a->info->cons.value == 0){
                printf("\nvalue: false\n");
            }else{
                printf("\nvalue: true\n");
            }
        }
    }else{
        printf("Node NULL\n");
    }
    printf("\n");
}

/**
 * Muestra por pantalla un nodo operador.
 */
void showOp(Node *a){
    if(a != NULL){
        printf("\nid: %s\n",a->info->op.id);
    }else{
        printf("Node NULL\n");
    }
    printf("\n");
}

/**
 * Muestra por pantalla un nodo función.
 */
void showFunc(Node *a){
    if(a != NULL){
        printf("id: %s\n",a->info->func.id);
    }else{
        printf("Node NULL\n");
    }
}

/**
 * Muestra un nodo por pantalla, ya sea variable, constante,
 * operador o función.
 */
void showNode(Node *p){
    if (p != NULL){
        switch ( p->tag ) {
        case 0:
            showVar(p);
            break;
        case 1:
            showConst(p);
            break;
        case 2:
            showOp(p);
            break;
        case 3:
            showFunc(p);
            break;
        case 4:
            showVar(p);
            break;
        default:
            showVar(p);
            break;
        }
    }
}

/**
 * Marca un nodo como visitado (mark=-1).
 */
void mark(Node *node){
    node->mark = -1;
}

/**
 * Implementación de un recorrido depth first search.
 */
void dfs(Node *root){
    if(root != NULL){
        showNode(root);
        mark(root);
        Node *adjacent[3];
        adjacent[0] = root->left;
        adjacent[1] = root->mid;
        adjacent[2] = root->right;
        for (int i = 0; i < 3; i++) {
            if(adjacent[i] != NULL && (adjacent[i]->mark != -1 || adjacent[i]->tag == 0)){
                dfs(adjacent[i]);
            }
        }
    }
}

/**
 * Setea el offset de una variable o de una función.
 */
void setOffset(Node *root, int xOffset){
    if (root != NULL){
        if(root->tag == 0 || root->tag == 4)
            root->info->var.offset = xOffset;
        if (root->tag == 3)
            root->info->func.offset = xOffset;
    }
}

#endif
