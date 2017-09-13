%{
#include <stdlib.h>
#include <stdio.h>
#include "list.c"
#include "structures.c"

//extern int yylineno;

List *list;
%}

%union { int i; struct tokenLine *tokenLine; char *s; struct node *treeN; }

%token<tokenLine> ID
%token<tokenLine> INT_LITERAL
%token<tokenLine> BOOLEAN
%token<tokenLine> ELSE
%token<tokenLine> FALSE
%token<tokenLine> IF
%token<tokenLine> INT
%token<tokenLine> RETURN
%token<tokenLine> TRUE
%token<tokenLine> VOID
%token<tokenLine> WHILE
%token<tokenLine> MAS
%token<tokenLine> MENOS
%token<tokenLine> MULT
%token<tokenLine> DIV
%token<tokenLine> MOD
%token<tokenLine> MAYOR
%token<tokenLine> MENOR
%token<tokenLine> ASIGN
%token<tokenLine> IGUAL
%token<tokenLine> AND
%token<tokenLine> OR
%token<tokenLine> NOT

%left OR
%left AND
%nonassoc IGUAL
%nonassoc MENOR MAYOR
%left MAS MENOS
%left MULT DIV MOD
%right UMINUS

%%

program: list_var_decl list_method_decl
       | list_method_decl
       | list_var_decl
       |
;

list_var_decl: var_decl
            |  list_var_decl var_decl
;

list_method_decl: method_decl
                | method_decl list_method_decl
;

var_decl: type ID';'
        | type ID',' AuxId
;

AuxId: ID';'
     | ID',' AuxId
;

method_decl: type ID '(' ')' block
           | type ID '('TypeID')' block
           | VOID ID '(' ')' block
           | VOID ID '('TypeID')' block
;

TypeID: type ID
      | type ID','TypeID
;

block: '{'list_var_decl list_statament'}'
     | '{'list_statament'}'
     | '{'list_var_decl'}'
     | '{''}'
;

list_statament: statament
            | statament list_statament
;

type: INT
    | BOOLEAN

;

statament: ID ASIGN expr';'
         | method_call';'
         | IF '(' expr ')' block ELSE block
         | IF '(' expr ')' block
         | WHILE '(' expr ')' block         {printf("Numero linea: %i\n",$1->noLine);}
         | RETURN expr';'
         | RETURN';'
         | ';'
         | block
;

method_call: ID '(' ')'
           | ID '('AuxExpr')'
;

AuxExpr:  expr
        | expr',' AuxExpr
;

expr: ID
    | method_call
    | literal
    | expr MAS expr
    | expr MULT expr
    | expr MENOS expr
    | expr DIV expr
    | expr MOD expr
    | expr MENOR expr
    | expr MAYOR expr
    | expr IGUAL expr
    | expr AND expr
    | expr OR expr
    | MENOS expr %prec UMINUS
    | NOT expr %prec UMINUS
    | '('expr')'
;

literal: INT_LITERAL
       | bool_literal
;

bool_literal: TRUE
            | FALSE
;

%%
