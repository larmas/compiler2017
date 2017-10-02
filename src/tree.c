#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


struct node;
typedef struct node Node;

//VARIABLE
typedef struct variable{
  char id[20];
  int value;  // constante int o bool(0,1)
}TVar;

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

//NODO
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

void insertTree(Node *raiz, Node *leafL, Node *leafM, Node *leafR);
//List *getAdjacentUnvisited(ListN *l, Node *node);
void mark(Node *node);
void dfs(Node *root);


Node *newVar(char xId[], int xType, int xValue, int xLine){
	Node *new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = 0;
    new->type = xType;  // 0:int, 1:boolean
    new->noline = xLine;
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
    TInfo *i = (TInfo *) malloc(sizeof(TInfo));
    strcpy( i->var.id , xId );
    i->var.value = xValue;		// constante int o bool(0,1)
    new->info = i;
    return new;
}

Node *newConst(int xType, int xValue, int xLine){
	Node *new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = 1;
    new->noline = xLine;
    new->type = xType;  // 0:int, 1:boolean
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
    TInfo *i = (TInfo *) malloc(sizeof(TInfo));
    i->cons.value = xValue;   // constante int o bool(0,1)
    new->info = i;
    return new;
}

Node *newOp(char xId[], int xType, int xLine){
	Node *new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = 2;
    new->noline = xLine;
    new->type = xType;  // 0:int, 1:boolean
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
    new->type = xType;  // 0:int, 1:boolean
    new->left = NULL;
    new->mid = NULL;
    new->right = NULL;
    TInfo *i;
    i = (TInfo *) malloc(sizeof(TInfo));
    strcpy( i->func.id , xId );
    i->func.param = xParam;
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
    } else {
        printf("Node NULL\n");
    }
    printf("\n");
}

void showConst(Node *a){
    if(a != NULL){
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
    }else{
        printf("Node NULL\n");
    }
}

void showOp(Node *a){
    if(a != NULL){
        printf("id: %s\n",a->info->op.id);
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
        printf("Parametros: \n");
        showList(a->info->func.param);
    }else{
        printf("Node NULL\n");
    }
    
}

void showNode(Node *p){
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
    default:
        showFunc(p);
        break;
    }
}

void mark(Node *node){
    node->mark = -1;
}

/*List *getAdjacentUnvisited(List *l, Node *node){
    l = newList(l);
    if(node->left != NULL && node->left->mark != -1)
        l = insertLast(l,node->left);
    if(node->mid != NULL && node->mid->mark != -1)
        l = insertLast(l,node->mid);
    if(node->right != NULL && node->right->mark != -1)
        l = insertLast(l,node->right);
}*/

/*
void dfs(Node *root){
    if(root->value!=NULL)
  	  printf( "%i ", root->value );
    else
      printf("%s", root->id);
    mark(root);
    Node *adjacent[3];
    adjacent[0] = root->left;
    adjacent[1] = root->mid;
    adjacent[2] = root->right;
    for (int i = 0; i < 2; i++) {
        if(adjacent[i] != NULL && adjacent[i]->mark == 0){
            dfs(adjacent[i]);
        }
    }
}
*/

void dfs(Node *root){
    if(root->tag == 0)
        printf("%s\n", root->info->var.id);
    if(root->tag == 1)
        printf( "%i\n", root->info->cons.value );
    if(root->tag == 2)
        printf("%s\n", root->info->op.id);
    if(root->tag == 3)
        printf("%s\n", root->info->func.id);
    mark(root);
    Node *adjacent[3];
    adjacent[0] = root->left;
    adjacent[1] = root->mid;
    adjacent[2] = root->right;
    for (int i = 0; i < 3; i++) {
        if(adjacent[i] != NULL && adjacent[i]->mark == 0){
            dfs(adjacent[i]);
        }
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
}*/
