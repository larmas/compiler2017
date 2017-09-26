#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.c"

typedef struct stack{
    List *currentLevel;
    int length; //Arranca desde 1
    struct stack *next;
}Stack;

/*PROTOTIPOS*/

Stack *newStack(Stack *s); //Inicializa un nuevo stack
Stack *pushNewLevel(Stack *s); //Abre un nuevo nivel
Stack *pushTop(Stack *s, Node *n); //Inserta un nodo en la lista que esta en el tope
Stack *popLevel(Stack *s); //Cierra un nivel
Node *findTop(Stack *s, char _id[], int _tag); //Busca el nodo con id=_id y tag=_tag en el nivel corriente
Node *findAll(Stack *s, char _id[], int _tag); //Busca el nodo con id=_id y tag=_tag en todo el stack
void showStack(Stack *s); //Muestra por pantalla el stack completo

Stack *newStack(Stack *s){
    s = (Stack *) malloc (sizeof(Stack));
    List *l;
    l = (List *) malloc (sizeof(List));
    l = newList(l);
    s->currentLevel = l;
    s->next = NULL;
    s->length = 1;
    return s;
}

Stack *pushNewLevel(Stack *s){
    if (s == NULL){
        s = newStack(s);
    }else{
        Stack *new = (Stack *) malloc (sizeof(Stack));
        List *l = (List *) malloc (sizeof(List));
        l = newList(l);
        new->currentLevel = l;
        new->next = s;
        new->length = s->length;
        s = new;
        s->length++;
    }
    return s;
}

Stack *pushTop(Stack *s, Node *n){
    s->currentLevel = insertLast(s->currentLevel,n);
    return s;
}

Stack *popLevel(Stack *s){
    s = s->next;
    s->length--;
    return s;
}

Node *findTop(Stack *s, char _id[], int _tag){
    Node *result = (Node *)malloc(sizeof(Node));
    result = findElem(s->currentLevel, _id, _tag);
    return result;
}

Node *findAll(Stack *s, char _id[], int _tag){
    Stack *p = s;
    Node *result = (Node *)malloc(sizeof(Node));
    while(p != NULL){
        result = findElem(p->currentLevel,_id,_tag);
        if(result != NULL)
            break;
        p = p->next;
    }
    return result; //Puede retornar un nodo NULL
}

void showStack(Stack *s){
    Stack *index;
    index = s;
    int level = index->length;
    while(s != NULL){
        printf("%s%i\n","Level: ",level);
        showList(s->currentLevel);
        s = s->next;
        level--;
    }
}

/*
int main(int argc, char const *argv[]) {
    Stack *test = (Stack *) malloc (sizeof(Stack));

    Node *n1 = (Node *) malloc (sizeof(Node));
    n1 = newVar("x1",0,10,1);
    test = newStack(test);
    test = pushTop(test,n1);
    showStack(test);

    Node *n1 = (Node *) malloc (sizeof(Node));
    n1 = newVar("x1",0,10,1);
    test = newStack(test);

    Node *n2 = (Node *) malloc (sizeof(Node));
    n2 = newVar("x2",0,0,2);
    test = pushNewLevel(test);
    test = pushTop(test,n2);
    test = pushTop(test,n1);
    showStack(test);

    Node *n1 = (Node *) malloc (sizeof(Node));
    n1 = newVar("x1",0,10,1);
    test = newStack(test,n1);
    Node *n2 = (Node *) malloc (sizeof(Node));
    n2 = newVar("x2",0,0,2);
    pushTop(test,n2);
    showStack(test);

    Node *n1 = (Node *) malloc (sizeof(Node));
    n1 = newVar("x1",0,10,1);
    test = newStack(test,n1);
    Node *n2 = (Node *) malloc (sizeof(Node));
    n2 = newVar("x2",0,0,2);
    test = pushNewLevel(test,n2);
    showStack(test);
    test = popLevel(test);
    printf("\n%s\n","Level eliminado...\n" );
    showStack(test);

    Node *n1 = (Node *) malloc (sizeof(Node));
    n1 = newVar("x1",0,10,1);
    test = newStack(test,n1);
    Node *n2 = (Node *) malloc (sizeof(Node));
    n2 = newVar("x2",0,0,2);
    test = pushNewLevel(test,n2);
    showStack(test);
    printf("%s\n","BUSQUEDA EN EL TOPE: \n" );
    Node *result1 = findTop(test,"x2",0);
    showVar(result1);
    printf("%s\n","BUSQUEDA EN TODO EL STACK: \n" );
    Node *result2 = findAll(test,"x2",0);
    showNode(result2);
    return 0;
}
*/