%{
#include <stdlib.h>
#include <stdio.h>
#include "structures.c"
#include "stackSymbol.c"

//extern int yylineno;
Stack *tds;
List *list;
%}

%union { int i; struct tokenLine *tokenLine; struct node *treeN; struct list *varList; }

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

%type<varList> var_decl
%type<i> type

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

list_var_decl: var_decl {
                            printf("%s\n", "---1---");
                            List *index = $1;
                            while(index != NULL){
                                if(tds==NULL){
                                    tds = newStack(tds, index->node);
                                }else{
                                    tds = pushTop(tds, index->node);
                                }
                                showStack(tds);
                            }
                        }
            |  list_var_decl var_decl   {
                                            printf("%s\n", "---2---");
                                            List *index = $2;
                                            while(index != NULL){
                                                if(tds==NULL){
                                                    tds = newStack(tds,index->node);
                                                }else{
                                                    tds = pushTop(tds, index->node);
                                                }
                                                showStack(tds);
                                            }
                                        }
;

list_method_decl: method_decl
                | method_decl list_method_decl
;
/*var_decl deberia ser una lista de un struct que tenga tipo y id*/
var_decl: type ID';'        {

                                Node *new = newVar($2,$1,NULL,$2->noLine);

                                if($$ == NULL){
                                    printf("%s\n","-----4----");
                                    $$ = newList($$,new);
                                }else{
                                    printf("%s\n","-----3----");
                                    $$ = insertLast($$,new);

                                }
                            }
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

type: INT {$$ = 0;}
    | BOOLEAN {$$ = 1;}

;

statament: ID ASIGN expr';'
         | method_call';'
         | IF '(' expr ')' block ELSE block
         | IF '(' expr ')' block
         | WHILE '(' expr ')' block
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
