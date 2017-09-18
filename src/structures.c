#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.c"
#include "list.c"

typedef struct tokenLine{
    char id[20];
    int noLine;
}TokenLine;

//VARIABLE
typedef struct variable{
	char id[20];
	int type;   // 0:int, 1:boolean.
	int value;  // constante int o bool(0,1)
}TVar;


//FUNCION
typedef struct funcion{
	char id[20];
	int type;      // 0:int, 1:boolean.
	List *param;   // lista de parametros
	Node AST;
}TFunc;


//CONSTANTE
typedef struct constante{
	int type;	  // 0:int, 1:boolean.
	int value; 	  // constante int o bool(0,1)
}TConst;


//OPERADOR
typedef struct operador{
	char id[5];
	int type; 
}TOpe;


typedef struct info{
	TVar var;         // variable
	TFunc func;	 	  // funcion
	TConst cons;	  // constante
	TOpe op;		  // operador
}TInfo;

