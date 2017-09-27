%{
#include <stdlib.h>
#include <stdio.h>
#include "structures.c"
#include "stackSymbol.c"

Stack *tds;
List *list;

%}

%union { int i; struct tokenLine *tokenLine; struct node *treeN; struct list *List; }

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

%type<List> var_decl
%type<treeN> method_decl
%type<List> list_var_decl
%type<List> list_method_decl
%type<List> TypeID
%type<treeN> block
%type<i> type
%type<List> AuxId

%left OR
%left AND
%nonassoc IGUAL
%nonassoc MENOR MAYOR
%left MAS MENOS
%left MULT DIV MOD
%right UMINUS

%%
initial:
    {
        tds = newStack(tds);
    } program
;
program:
    list_var_decl list_method_decl  {
                                        showStack(tds);
                                    }
    | list_method_decl  {
                            showStack(tds);
                        }
    | list_var_decl     {
                            showStack(tds);
                        }
    | /* LAMBDA */  {
                        showStack(tds);
                    }
;

list_var_decl:
    var_decl    {
                    List *index = $1;
                    while(index != NULL){
                        tds = pushTop(tds,index->node);
                        index = index->next;
                    }
                }
    |  list_var_decl var_decl   {
                                    List *index = $2;
                                    while(index != NULL){
                                        tds = pushTop(tds,index->node);
                                        index = index->next;
                                    }
                                }
;

list_method_decl:
    method_decl     {

                    }
    | method_decl list_method_decl  {

                                    }
;

var_decl:
    type ID';'  {
                    Node *new = newVar($2->id, $1, NULL, $2->noLine);
                    List *newL = newList(newL);
                    newL = insertLast(newL,new);
                    $$ = newL;
                }
    | type ID',' AuxId  {
                            Node *new = newVar($2->id, $1, NULL, $2->noLine);
                            List *newL = newList(newL);
                            newL = insertLast(newL,new);
                            List *index = $4;
                            while (index != NULL){
                                index->node->type = $1;
                                newL = insertLast(newL,index->node);
                                index = index->next;
                            }
                            $$ = newL;
                        }
;

AuxId:
    ID';'   {
                Node *new = newVar($1->id, NULL, NULL, $1->noLine);
                List *newL = newList(newL);
                newL = insertFirst(newL,new);
                $$ = newL;
            }
    | ID',' AuxId  {
                        Node *new = newVar($1->id, NULL, NULL, $1->noLine);
                        List *newL = insertFirst($3,new);
                        $$ = newL;
                    }
;

method_decl:
    method_aux1 '(' ')' method_aux3     {

                                            tds = popLevel(tds);

                                            //Aca debo completar el nodo funcion.
                                        }
    | method_aux1 '(' method_aux2 ')' method_aux3   {

                                                        tds = popLevel(tds);
                                                        tds = popLevel(tds);
                                                        // Aca debo completar el nodo funcion.
                                                    }
;

method_aux1:
    type ID {
                Node *new = newFunc($2->id,$1,NULL,NULL,$2->noLine);
                tds = pushTop(tds,new);
            }
    | VOID ID   {
                    Node *new = newFunc($2->id,3,NULL,NULL,$2->noLine);
                    tds = pushTop(tds,new);
                }
;

method_aux2:

    {tds = pushNewLevel(tds);} TypeID

;

method_aux3:
    {tds = pushNewLevel(tds);} block

;

TypeID:
    type ID     {
                    Node *new = newVar($2->id,$1,NULL,$2->noLine);
                    tds = pushTop(tds,new);
                }
    | type ID','TypeID  {
                            Node *new = newVar($2->id,$1,NULL,$2->noLine);
                            tds = pushTop(tds,new);
                        }
;

block:
    '{'list_var_decl list_statament'}'  {

                                        }
    | '{'list_statament'}'  {

                            }
    | '{'list_var_decl'}'   {

                            }
    | '{''}'    {

                }
;

list_statament:
    statament
    | statament list_statament
;

type:
    INT {
            $$ = 0;
        }
    | BOOLEAN   {
                    $$ = 1;
                }

;

statament:
    ID ASIGN expr';'
    | method_call';'
    | IF '(' expr ')' block ELSE block
    | IF '(' expr ')' block
    | WHILE '(' expr ')' block
    | RETURN expr';'
    | RETURN';'
    | ';'
    | block
;

method_call:
    ID '(' ')'
    | ID '('AuxExpr')'
;

AuxExpr:
    expr
    | expr',' AuxExpr
;

expr:
    ID
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

literal:
    INT_LITERAL
    | bool_literal
;

bool_literal:
    TRUE
    | FALSE
;

%%
