%{
#include <stdlib.h>
#include <stdio.h>
#include "structures.c"
#include "stackSymbol.c"

Stack *tds;
List *list;

extern char yytext;

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
%type<treeN> method_aux1;
%type<List> method_aux2;
%type<treeN> method_aux3;
%type<List> list_var_decl
%type<List> list_method_decl
%type<List> TypeID
%type<treeN> block
%type<i> type
%type<List> AuxId
%type<treeN> statament
%type<treeN> list_statament
%type<treeN> bool_literal
%type<treeN> literal
%type<treeN> expr
%type<treeN> method_call
%type<List> AuxExpr

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
                                        //printf("---STACK---\n");
                                        //showStack(tds);
                                        //printf("--------\n");
                                    }

    | list_method_decl  {
                            //printf("---STACK---\n");
                            //showStack(tds);
                            //printf("--------\n");
                        }

    | list_var_decl     {
                            //printf("---STACK---\n");
                            //showStack(tds);
                            //printf("--------\n");
                        }

    | /* LAMBDA */  {
                        //printf("---STACK---\n");
                        //showStack(tds);
                        //printf("--------\n");
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
                    Node *new = newVar($2->id, $1, 0, $2->noLine);
                    List *newL = newList(newL);
                    newL = insertLast(newL,new);
                    $$ = newL;
                }

    | type ID',' AuxId  {
                            Node *new = newVar($2->id, $1, 0, $2->noLine);
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
                Node *new = newVar($1->id, NULL, 0, $1->noLine);
                List *newL = newList(newL);
                newL = insertFirst(newL,new);
                $$ = newL;
            }

    | ID',' AuxId  {
                        Node *new = newVar($1->id, NULL, 0, $1->noLine);
                        List *newL = insertFirst($3,new);
                        $$ = newL;
                    }
;

method_decl:
    method_aux1 '(' ')' method_aux3     {
                                            tds = popLevel(tds);
                                            List *newL = newList(newL);
                                            $1->info->func.param = newL;
                                            $1->info->func.AST = $4;
                                            dfs($1->info->func.AST);
                                        }
    | method_aux1 '(' method_aux2 ')' method_aux3   {
                                                        tds = popLevel(tds);
                                                        tds = popLevel(tds);
                                                        $1->info->func.param = $3;
                                                        $1->info->func.AST = $5;
                                                        dfs($1->info->func.AST);
                                                    }
;

method_aux1: /* retorna el nodo func */
    type ID {
                Node *new = newFunc($2->id,$1,NULL,NULL,$2->noLine);
                tds = pushTop(tds,new);
                $$ = new;
            }
    | VOID ID   {
                    Node *new = newFunc($2->id,3,NULL,NULL,$2->noLine);
                    tds = pushTop(tds,new);
                    $$ = new;
                }
;

method_aux2: /* retorna la lista de parametros */
    {
        tds = pushNewLevel(tds);
    } TypeID    {
                    $$ = $2;
                }
;

method_aux3: /* retorna el AST del block */
    {
        tds = pushNewLevel(tds);
    } block {
                $$ = $2;
            }
;

TypeID:
    type ID     {
                    Node *new = newVar($2->id,$1,NULL,$2->noLine);
                    tds = pushTop(tds,new);
                    List *newL = newList(newL);
                    newL = insertFirst(newL,new);
                    $$ = newL;
                }

    | type ID','TypeID  {
                            Node *new = newVar($2->id,$1,NULL,$2->noLine);
                            tds = pushTop(tds,new);
                            List *newL;
                            newL = insertFirst($4,new);
                            $$ = newL;
                        }
;

block:
    '{'list_var_decl list_statament'}'  {
                                            $$ = $3;
                                        }
    | '{'list_statament'}'  {

                                $$ = $2;
                            }
    | '{'list_var_decl'}'   {
                                $$ = NULL;
                            }
    | '{''}'    {
                    $$ = NULL;
                }
;

list_statament:
    statament   {
                    $$ = $1;
                }
    | statament list_statament  {
                                   Node *root = newOp(";", 0, $1->noline);
                                   insertTree(root, $1, $2, NULL);
                                   $$ = root;
                                }
;

type:
    INT {
            $$=0;
        }
    | BOOLEAN   {
                    $$=1;
                }
;

statament:
    ID ASIGN expr';'    {
						    Node *xId = findAll(tds, $1->id, 0);
						    if(xId != NULL){
							    Node *root = newOp("=", $3->type, $2->noLine);
                                insertTree(root, xId, $3, NULL);
                                $$ = root;
                            }else{
                                printf("%s%s%s%i\n","Variable no declarada: ",$1->id," en la linea ",$1->noLine);
                                exit(1);
	                        }
	                    }
    | method_call';'    {
                            $$ =$1;
                        }
    | IF '(' expr ')' block ELSE block  {
                                            Node *root = newOp("ifElse", 1, $1->noLine);
                                            insertTree(root, $3, $5, $7);
                                            $$ = root;
                                        }
    | IF '(' expr ')' block     {
                                    Node *root = newOp("if", 1, $1->noLine);
                                    insertTree(root, $3, $5, NULL);
                                    $$ = root;
                                }
    | WHILE '(' expr ')' block   {
                                    Node *root = newOp("while", 1, $1->noLine);
                                    insertTree(root, $3, $5, NULL);
                                    $$ = root;
                                }
    | RETURN expr';'    {
                            Node *root = newOp("return", $2->type, $1->noLine);
                            insertTree(root, $2, NULL, NULL);
                            $$ = root;
                        }
    | RETURN';'     {
                        Node *root = newOp("returnVoid", 3, $1->noLine);
                        insertTree(root, NULL, NULL, NULL);
                        $$ = root;
                    }
    | ';'   {
                Node *root = newOp("skip", 3, -1);  //*** hacer que ; retorne noLine
                insertTree(root, NULL, NULL, NULL);
                $$ = root;
            }
    | block     {
                    $$ = $1;
                }
;

method_call:
    ID '(' ')'     {
					    Node *funcTDS = findAll(tds, $1->id, 3);
					    if(funcTDS != NULL){
                            Node *root = newOp("function", funcTDS->type, $1->noLine);
						    insertTree(root, funcTDS, NULL, NULL);
						    $$ = root;
					    }else{
						    printf("%s%s%s%i\n","Metodo no declarado: ",$1->id," en la linea ",$1->noLine);
                            exit(1);
					    }
					}
    | ID '('AuxExpr')'  {
	                        Node *funcTDS = findAll(tds, $1->id, 3);
						    if(funcTDS != NULL){
                                Node *root = newOp("function", funcTDS->type, $1->noLine);
							    insertTree(root, funcTDS, $3, NULL);
							    $$ = root;
						    }else{
							    printf("%s%s%s%i\n","Metodo no declarado: ",$1->id," en la linea ",$1->noLine);
                                exit(1);
						    }
						}
;

AuxExpr:
    expr    {
                List *newL = newList(newL);
                newL = insertParam(newL,$1);
                $$ = newL;
            }
    | expr',' AuxExpr   {
                            List *newL = insertParam($3,$1);
                            $$ = newL;
                        }
;

expr:
    ID  {
		    Node *root = findAll(tds, $1->id, 0);
		    if(root != NULL){
		        $$ = root;
		    }else{
		        printf("%s%s%s%i\n","Error: Variable no declarada: ",$1->id," en la linea ",$1->noLine);
                exit(1);
		    }
		}
    | method_call   {
                        $$ = $1;
                    }
    | literal   {
                    $$ = $1;
                }
    | expr MAS expr     {
                            Node *root = newOp("+", 0, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr MULT expr    {
                            Node *root = newOp("*", 0, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr MENOS expr   {
                            Node *root = newOp("-", 0, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr DIV expr     {
                            Node *root = newOp("/", 0, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr MOD expr     {
                            Node *root = newOp("%", 0, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr MENOR expr   {
                            Node *root = newOp("<", 1, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr MAYOR expr   {
                            Node *root = newOp(">", 1, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr OR expr      {
                            Node *root = newOp("||", 1, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr AND expr     {
                            Node *root = newOp("&&", 1, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | expr IGUAL expr   {
                            Node *root = newOp("==", 1, $2->noLine);
                            insertTree(root, $1, $3, NULL);
                            $$ = root;
                        }
    | MENOS expr %prec UMINUS   {
                                    Node *root = newOp("negativo", 0, $1->noLine);
                                    insertTree(root, $2, NULL, NULL);
                                    $$ = root;
                                }
    | NOT expr %prec UMINUS     {
                                    Node *root = newOp("!", 1, $1->noLine);
                                    insertTree(root, $2, NULL, NULL);
                                    $$ = root;
                                }
    | '('expr')'    {
                        $$ = $2;
                    }
;

literal:
    INT_LITERAL     {
                        int value = atoi($1->id);
                        Node *root = newConst(0, value, $1->noLine);
                        $$ = root;
                    }
    | bool_literal  {
                        $$ = $1;
                    }
;

bool_literal:
    TRUE    {
                Node *root = newConst(1, 1, $1->noLine);
                $$ = root;
            }
    | FALSE     {
                    Node *root = newConst(1, 0, $1->noLine);
                    $$ = root;
                }
;


%%

void checkType(Node * root){
	if(root->tag == 2){ // es operador

		if(strcmp(root->info->op.id, "=") == 0){
			if(root->left->type != root->mid->type){
				printf("%s%i\n","Error: tipos incompatibles en la linea ",root->noline);
	        	exit(1);
			}
			checkType(root->mid);
		}

		if(strcmp(root->info->op.id, "function") == 0){
			if ( longList(root->left->info->func.param) == longList(root->mid) ){    // tienen la misma cantidad de param
				int cont = 0;
				List *a_param = root->left->info->func.param;
				List *b_param = root->mid;
				for (cont; cont <  longList(b_param); cont++){ // controlo q cada parametro pasado tenga el mismo tipo
					if(a_param->node->type != b_param->node->type){
						printf("%s%i\n","Error: tipos incompatibles en la linea ",root->noline);
	        			exit(1);
					}else{
						a_param = a_param->next;
						a_param = b_param->next;
					}
				}

			}else{
				printf("%s%i\n","Error: la cantidad de parametros no es correcta ",root->noline);
	        	exit(1);
			}
		}

		if(strcmp(root->info->op.id, "if") == 0){
			checkType(root->left);
			checkType(root->mid);
		}

		if(strcmp(root->info->op.id, "ifElse") == 0){
			checkType(root->left);
			checkType(root->mid);
			checkType(root->right);
		}

		if(strcmp(root->info->op.id, "while") == 0){
			checkType(root->left);
			checkType(root->mid);
		}

		if(strcmp(root->info->op.id, "return") == 0){
			checkType(root->left);
			//controlar que return retorne lo mismo que la funcion
		}

		if(strcmp(root->info->op.id, "returnVoid") == 0){
			//controlar que return retorne lo mismo que la funcion
		}

        if(strcmp(root->info->op.id, "+") == 0 ||
           strcmp(root->info->op.id, "*") == 0 ||
           strcmp(root->info->op.id, "-") == 0 ||
           strcmp(root->info->op.id, "/") == 0 ||
           strcmp(root->info->op.id, "%") == 0 )
        {
			if(root->left->type == root->mid->type){
				checkType(root->left);
				checkType(root->mid);
			}else{
				printf("%s%i\n","Error: tipos incompatibles en la linea ",root->noline);
	            exit(1);
			}
        }

        if(strcmp(root->info->op.id, "&&") == 0 ||
           strcmp(root->info->op.id, "||") == 0 ||
           strcmp(root->info->op.id, "==") == 0 )
        {
        	if(root->left->type == root->mid->type){
				checkType(root->left);
				checkType(root->mid);
			}else{
				printf("%s%i\n","Error: tipos incompatibles en la linea ",root->noline);
	            exit(1);
			}
        }

		if(strcmp(root->info->op.id, ">") == 0 || strcmp(root->info->op.id,"<") == 0){
			if(root->left->type == 0 && root->mid->type == 0){
				checkType(root->left);
				checkType(root->mid);
			}else{
				printf("%s%i\n","Error: tipos incompatibles en la linea ",root->noline);
	            exit(1);
			}
		}

		if(strcmp(root->info->op.id, "!") == 0 || strcmp(root->info->op.id,"negativo") == 0){
			if(root->type == root->left->type){
				checkType(root->left);
			}else{
				printf("%s%i\n","Error: tipos incompatibles en la linea ",root->noline);
	            exit(1);
			}
		}
	}

}
