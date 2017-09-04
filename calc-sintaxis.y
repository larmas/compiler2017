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



%left '+'
%left '*'

%%

program: var_decl method_decl
       | method_decl
       | var_decl
       |



var_decl: type ID';'
        | type ID',' AuxId

AuxId: ID';'
     | ID',' AuxId



method_decl: type ID '(' ')' block
           | type ID '('TypeID')' block
           | VOID ID '(' ')' block
           | VOID ID '('TypeID')' block

TypeID: type ID
      | type ID','TypeID



block: '{'var_decl statament'}'
     | '{'statament'}'
     | '{'var_decl'}'
     | '{''}'



type: INT
    | BOOLEAN




statament: ID ASIGN expr';'
         | method_call';'
         | IF'('expr')' block ELSE block
         | IF'('expr')' block
         | WHILE expr block
         | RETURN expr';'
         | RETURN
         | ';'
         | block




method_call: ID '(' ')'
           | ID '('expr')'
           | ID '('expr AuxExpr')'

AuxExpr: ','expr
       | ','expr AuxExpr



expr: ID
    | method_call
    | literal
    | expr bin_op expr
    | MENOS expr
    | '!' expr
    | '('expr')'



bin_op: arith_op
      | rel_op
      | cond_op



arith_op: MAS
        | MENOS
        | MULT
        | DIV
        | MOD



rel_op: MENOR
      | MAYOR
      | IGUAL



cond_op: AND
       | OR



literal: INT_LITERAL
       | bool_literal



bool_literal: TRUE
            | FALSE




%%
