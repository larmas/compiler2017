#ifndef TREE_C
#define TREE_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct node;
typedef struct node Node;

/**
 * Representacion de un nodo que contiene una variable.
 */
typedef struct variable{
  char id[20];
  int value;
  int offset;
}TVar;

/**
 * Representacion de un nodo que contiene una funcion.
 * El atributo param representa la lista de parametros que toma
 * una funcion.
 */
typedef struct funcion{
  char id[20];
  int offset;
  struct list *param;
  Node *AST;
}TFunc;

/**
 * Representacion de un nodo que contiene una constante.
 */
typedef struct constante{
  int value;    // constante int o bool(0,1)
}TConst;

/**
 * Representacion de un nodo que contiene un operador.
 */
typedef struct operador{
  char id[5];
}TOpe;

/**
 * Union que identifica a un nodo como variable, constante, operador o
 * funcion.
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
 *  un operador (tag=2), una funcion (tag=3) o un temporal (tag=4).
 *  Type indica el tipo del elemento, int = 0, boolean = 1 y void = 2.
 *  Mark es una marca utilizada en la funcion dfs para identificar nodos visitados (mark=-1)
 *  y no visitados (mark=0).
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
// Inicializacion de un nodo variable.
Node *newVar(char xId[], int xType, int xValue, int xLine);
// Inicializacion de un nodo temporal.
Node *newTemp(char xId[], int xType, int xValue, int xLine);
// Inicializacion de un nodo constante.
Node *newConst(int xType, int xValue, int xLine);
// Inicializacion de un nodo operador.
Node *newOp(char xId[], int xType, int xLine);
// Inicializacion de un nodo funcion.
Node *newFunc(char xId[], int xType, struct list *xParam, Node *xAST,int xLine);
// Muestra por pantalla un nodo variable.
void showVar(Node *a);
// Muestra por pantalla un nodo constante.
void showConst(Node *a);
// Muestra por pantalla un nodo operador.
void showOp(Node *a);
// Muestra por pantalla un nodo funcion.
void showFunc(Node *a);
// Inserta los hijos de un nodo.
void insertTree(Node *raiz, Node *leafL, Node *leafM, Node *leafR);
// Marca un nodo como visitado (mark=-1).
void mark(Node *node);
// Implementacion de un recorrido depth first search.
void dfs(Node *root);
// Reccore el arbol realizando el chequeo de tipos.
void checkType(Node *root);
// Setea el offset de una variable o de una funcion.
void setOffset(Node *root, int xOffset);


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

void insertTree(Node *root, Node *leafL, Node *leafM, Node *leafR){
    root->left = leafL;
    root->mid = leafM;
    root->right = leafR;
}

void showVar(Node *a){
    if(a != NULL){
        printf("id: %s\n",a->info->var.id);
        printf("offset: %d\n", a->info->var.offset);
        /*if (a->type == 0){
            printf("type: integer\n");
            //printf("\nvalue:%i\n",a->info->var.value);
        }else{
            printf("type: boolean\n");
            if(a->info->var.value == 0){
                printf("\nvalue: false\n");
            }else{
                printf("\nvalue: true\n");
            }
        }*/
    } else {
        printf("Node NULL\n");
    }
    printf("\n");
}

void showConst(Node *a){
    if(a != NULL){
        if (a->type == 0){
            //printf("type: integer\n");
            printf("\nvalue:%i\n",a->info->cons.value);
        }else{
            //printf("type: boolean\n");
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

void showOp(Node *a){
    if(a != NULL){
        printf("\nid: %s\n",a->info->op.id);
        /*if (a->type == 0){
            printf("type: integer\n");
        }else{
            printf("type: boolean\n");
        }*/
    }else{
        printf("Node NULL\n");
    }
    printf("\n");
}

void showFunc(Node *a){
    if(a != NULL){
        printf("id: %s\n",a->info->func.id);
        /*if (a->type == 0){
            printf("type: integer\n");
        }
        if(a->type == 1){
            printf("type: boolean\n");
        }
        if(a->type == 3){
            printf("type: void\n");
        }*/
        /*printf("Parametros: \n");
        showList(a->info->func.param);*/
    }else{
        printf("Node NULL\n");
    }
}

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
        default:
            showVar(p);
            break;
        }
    }
}

void mark(Node *node){
    node->mark = -1;
}

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

void setOffset(Node *root, int xOffset){
    if (root != NULL){
        if(root->tag == 0 || root->tag == 4)
            root->info->var.offset = xOffset;
        if (root->tag == 3)
            root->info->func.offset = xOffset;
    }
}

/*
int main(int argc, char const *argv[]) {
    Node *var1 = newVar("x",0,10,50);  // x:10
    Node *var2 = newVar("y",1,0,50);   // y:false
    //showVar(var1);
    //showVar(var2);

    Node *cons1 = newConst(1,1,50);    // true
    Node *cons2 = newConst(0,88,50);   // 88
    //showConst(cons1);
    //showConst(cons2);

    Node *op1 = newOp("*",0,50);       // *
    Node *op2 = newOp("&&",1,50);      // &&
    //showOp(op1);
    //showOp(op2);

    Node *func1 = newFunc("suma",0,NULL,NULL,50);
    Node *func2 = newFunc("equals",1,NULL,NULL,50);
    //showFunc(func1);
    //showFunc(func2);

    insertTree(var1,var2,cons1,cons2);
    insertTree(var2,op1,NULL,op2);
    insertTree(op2,func1,NULL,func2);

    dfs(var1);

  return 0;

}
*/

#endif
