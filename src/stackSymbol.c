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
Stack *newStack(Stack *s);
Stack *pushNewLevel(Stack *s);
Stack *pushTop(Stack *s, Node *n);
Stack *popLevel(Stack *s);
Node *findTop(Stack *s, char _id[], int _tag);
Node *findAll(Stack *s, char _id[], int _tag);
void showStack(Stack *s);

/**
 * Inicializa un nuevo stack.
 */
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

/**
 * Inicializa un nuevo nivel.
 */
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

/**
 * Inserta un nodo en la lista que esta en el tope.
 */
Stack *pushTop(Stack *s, Node *n){
    s->currentLevel = insertLast(s->currentLevel,n);
    return s;
}

/**
 * Elimina un nivel de la pila.
 */
Stack *popLevel(Stack *s){
    s = s->next;
    return s;
}

/**
 * Busca el nodo con id=_id y tag=_tag en el nivel corriente.
 */
Node *findTop(Stack *s, char _id[], int _tag){
    Node *result = (Node *)malloc(sizeof(Node));
    result = findElem(s->currentLevel, _id, _tag);
    return result;
}

/**
 * Busca el nodo con id=_id y tag=_tag en todo el stack.
 */
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

/**
 * Muestra por pantalla el stack completo.
 */
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
