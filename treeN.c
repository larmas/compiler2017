#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node{
  int tag; // 0:variable, 1:int, 2:operador
  char id[20];
  int value;
  int mark; //0 no visitado, -1 visitado
  struct node *left;
  struct node *right;
}Node;


/*PROTOTIPOS*/
Node *newNode(int xTag, char xId[], int xValue);
void insertTree(Node *raiz, Node *leafL, Node *leafR);
void preorden(Node *raiz);
void mark(Node *node);
void dfs(Node *root);


// Crea un nuevo nodo con toda la informacion pasada
Node *newNode(int xTag, char xId[], int xValue){
    Node *new;
    new = (Node *) malloc(sizeof(Node));
    new->mark = 0;
    new->tag = xTag;
    if(xId!=NULL)
        for (int i = 0; i < sizeof(new->id); i++) {
            new->id[i] = xId[i];
        }
    if(xValue!=NULL)
        new->value = xValue;
    else
        new->value = NULL;
    return new;
}

// Le inserta al nodo 'root', sus hijos izquierdo y derecho
void insertTree(Node *root, Node *leafL, Node *leafR){
    root->left = leafL;
    root->right = leafR;
}


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
}

/*int main(int argc, char const *argv[]) {

  Node *root = newNode(0,"pepe",10);
  Node *left = newNode(0,"jose",20);
  Node *right = newNode(0,"juan",30);
  Node *aux1 = newNode(0,"x1",40);
  Node *aux2 = newNode(0,"x2",50);

  insertTree(root,left,right);
  insertTree(left,aux1, NULL);
  insertTree(right,aux2,NULL);
  dfs(root);

  return 0;
}*/
