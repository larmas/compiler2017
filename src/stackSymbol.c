#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.c"

/**
 * Estructura que representa una lista que se comporta como una pila.
 */
typedef struct stack{
    List *currentLevel;
    int length;
    struct stack *next;
}Stack;

/*PROTOTIPOS*/
//Inicializa un nuevo stack.
Stack *newStack(Stack *s);
//Inicializa un nuevo nivel.
Stack *pushNewLevel(Stack *s);
//Inserta un nodo en la lista que esta en el tope.
Stack *pushTop(Stack *s, Node *n);
//Elimina un nivel de la pila.
Stack *popLevel(Stack *s);
//Busca el nodo con id=_id y tag=_tag en el nivel corriente.
Node *findTop(Stack *s, char _id[], int _tag);
//Busca el nodo con id=_id y tag=_tag en todo el stack.
Node *findAll(Stack *s, char _id[], int _tag);
//Muestra por pantalla el stack completo.
void showStack(Stack *s);

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
    return result;
}

void showStack(Stack *s){
    Stack *index;
    index = s;
    int level = index->length;
    while(s != NULL){
        printf("\n%s%i\n","Level: ",level);
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
    showNode(findAll(test,"x2",0));

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
}*/
