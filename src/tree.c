#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct node;
typedef struct node Node;

typedef struct tokenLine{
    char id[20];
    int noLine;
}TokenLine;

//VARIABLE
typedef struct variable{
  char id[20];
  int value;  // constante int o bool(0,1)
}TVar;


struct node;
typedef struct node Node;
//FUNCION
typedef struct funcion{
  char id[20];
  struct list *param;   // lista de parametros
  Node *AST;
}TFunc;

//CONSTANTE
typedef struct constante{
  int value;    // constante int o bool(0,1)
}TConst;

//OPERADOR
typedef struct operador{
  char id[5];
}TOpe;

typedef union info{
  TVar var;         // variable
  TFunc func;       // funcion
  TConst cons;      // constante
  TOpe op;          // operador
}TInfo;

struct node{
  int tag;            // 0:variable, 1:const, 2:operador, 3:funcion.
  int noline;
  int type;           // 0:int, 1:boolean.
  TInfo *info;
  struct node *left;
  struct node *mid;
  struct node *right;
  int mark;           //0 no visitado, -1 visitado
};


/*PROTOTIPOS*/
Node *newVar(char xId[], int xType, int xValue, int xLine);
Node *newConst(int xType, int xValue, int xLine);
Node *newOp(char xId[], int xType, int xLine);
Node *newFunc(char xId[], int xType, struct list *xParam, Node *xAST,int xLine);

void showVar(Node *a);
void showConst(Node *a);
void showOp(Node *a);
void showConst(Node *a);

void insertTree(Node *raiz, Node *leafL, Node *leafR);
//void preorden(Node *raiz);
//void mark(Node *node);
//void dfs(Node *root);


// Crea un nodo tipo variable (0).
Node *newVar(char xId[], int xType, int xValue, int xLine){
	  Node *new;
    new = (Node *) malloc(sizeof(Node));

    new->mark = 0;
    new->tag = 0;
    new->type = xType;  // 0:int, 1:boolean
    new->noline = xLine;

    TInfo *i;
    i =(TInfo *) malloc(sizeof(TInfo));

    strcpy( i->var.id , xId );
    i->var.value = xValue;		// constante int o bool(0,1)

    new->info = i;
    return new;
}


// Crea un nodo tipo constante (1).
Node *newConst(int xType, int xValue, int xLine){
	  Node *new;
    new = (Node *) malloc(sizeof(Node));

    new->mark = 0;
    new->tag = 1;
    new->noline = xLine;
    new->type = xType;  // 0:int, 1:boolean

    TInfo *i;
    i =(TInfo *) malloc(sizeof(TInfo));

    i->cons.value = xValue;   // constante int o bool(0,1)

    new->info = i;
    return new;
}


// Crea un nodo tipo operador (2).
Node *newOp(char xId[], int xType, int xLine){
	Node *new;
    new = (Node *) malloc(sizeof(Node));

    new->mark = 0;
    new->tag = 2;
    new->noline = xLine;
    new->type = xType;  // 0:int, 1:boolean

    TInfo *i;
    i =(TInfo *) malloc(sizeof(TInfo));

    strcpy( i->op.id , xId );

    new->info = i;
    return new;
}

// Crea un nodo tipo funcion (3).
Node *newFunc(char xId[], int xType, struct list *xParam, Node *xAST, int xLine){
	Node *new;
    new = (Node *) malloc(sizeof(Node));

    new->mark = 0;
    new->tag = 3;
    new->noline = xLine;
    new->type = xType;  // 0:int, 1:boolean

    TInfo *i;
    i =(TInfo *) malloc(sizeof(TInfo));

    strcpy( i->func.id , xId );
    i->func.param = xParam;
    i->func.AST = xAST;
    new->info = i;
	  return new;
}


void showVar(Node *a){
    printf("id: %s\n",a->info->var.id);
    if (a->type == 0){
        printf("type: integer\n");
        printf("value:%i\n",a->info->var.value);
    }else{
        printf("type: boolean\n");
        if(a->info->var.value == 0){
          printf("value: false\n");
        }else{
          printf("value: true\n");
        }
    }
}

void showConst(Node *a){
    if (a->type == 0){
        printf("type: integer\n");
        printf("value:%i\n",a->info->cons.value);
    }else{
        printf("type: boolean\n");
        if(a->info->cons.value == 0){
          printf("value: false\n");
        }else{
          printf("value: true\n");
        }
    }
}

void showOp(Node *a){
    printf("id: %s\n",a->info->op.id);
    if (a->type == 0){
        printf("type: integer\n");
    }else{
        printf("type: boolean\n");
    }
}


void showFunc(Node *a){
     printf("id: %s\n",a->info->func.id);
    if (a->type == 0){
        printf("type: integer\n");
    }else{
        printf("type: boolean\n");
    }
}

// Le inserta al nodo 'root', sus hijos izquierdo y derecho
void insertTree(Node *root, Node *leafL, Node *leafR){
    root->left = leafL;
    root->right = leafR;
}

/*
// Recorrido preorden
void preorden(Node *raiz){
	if( !raiz ) return;
    if(raiz->value!=NULL)
	   printf( "%i ", raiz->value );
    else
        printf("%s", raiz->id);
	preorden(raiz->left);
	preorden(raiz->right);
}

void mark(Node *node){
    node->mark = -1;
}

void dfs(Node *root){
    if(root->value!=NULL)
  	  printf( "%i ", root->value );
    else
      printf("%s", root->id);
    mark(root);
    Node *adjacent[2];
    adjacent[0] = root->left;
    adjacent[1] = root->right;
    for (int i = 0; i < 2; i++) {
        if(adjacent[i] != NULL && adjacent[i]->mark == 0){
            dfs(adjacent[i]);
        }
    }
}*/

int main(int argc, char const *argv[]) {
 /* Node *var1 = newVar("x",0,10,50);  // x:10
  Node *var2 = newVar("y",1,0,50);   // y:false
  showVar(var1);
  showVar(var2);

  Node *cons1 = newConst(1,1,50);    // true
  Node *cons2 = newConst(0,88,50);   // 88
  showConst(cons1);
  showConst(cons2);

  Node *op1 = newOp("*",0,50);       // *
  Node *op2 = newOp("&&",1,50);      // &&
  showOp(op1);
  showOp(op2);

  Node *func1 = newFunc("suma",0,50);
  Node *func2 = newFunc("equals",1,50);

  showFunc(func1);
  showFunc(func2);
*/
  return 0;
}
