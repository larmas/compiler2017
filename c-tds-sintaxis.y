%{
#include <stdlib.h>
#include <stdio.h>
#include "list.c"

List *list;
%}

%union { int i; char *s; struct Node *treeN; }

%token<s> ID
%token<i> INT_LITERAL
%token<i> MAIN
%token<i> BOOLEAN
%token<i> ELSE
%token<i> FALSE
%token<i> IF
%token<i> INT
%token<i> RETURN
%token<i> TRUE
%token<i> VOID
%token<i> WHILE
%token<i> MAS
%token<i> MENOS
%token<i> MULT
%token<i> DIV
%token<i> MOD
%token<i> MAYOR
%token<i> MENOR
%token<i> ASIGN
%token<i> IGUAL
%token<i> AND
%token<i> OR
%token<i> NOT

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
             |  var_decl list_var_decl
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

block: '{'var_decl statament'}'
     | '{'statament'}'
     | '{'var_decl'}'
     | '{''}'
;

type: INT
    | BOOLEAN

;

statament: ID ASIGN expr';'
         | method_call';'
         | IF'('expr')' block ELSE block
         | IF'('expr')' block
         | WHILE '('expr')' block
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
