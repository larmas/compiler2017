%{
#include <stdlib.h>
#include <stdio.h>
#include "structures.c"
#include "stackSymbol.c"
#include "list.c"
#include "ciList.c"
#include "intermediateCode.c"
#include "assembly.c"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

int yylex();
void yyerror();

Stack *tds;
List *list;
CIList *ciList;
int typeRet;
int countReturn;
int offsetCount;

extern char yytext;
extern char **argv;

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
%type<treeN> block_aux
%type<i> type
%type<List> AuxId
%type<treeN> statament
%type<treeN> list_statament
%type<treeN> bool_literal
%type<treeN> literal
%type<treeN> expr
%type<treeN> method_call
%type<List> AuxExpr

/* PRECEDENCIAS */
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
        ciList = newCIList(ciList);
        offsetCount = 0;

    } program
;

program:
    list_var_decl list_method_decl  {
                                        Node *main = findAll(tds,"main",3);
                                        if(main == NULL){
                                            printf("%s\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Metodo main no existe.");
                                            exit(1);
                                        }
                                        showCIList(ciList);

                                        generateAsm(ciList, argv[0]);
                                    }

    | list_method_decl  {
                            Node *main = findAll(tds,"main",3);
                            if(main == NULL){
                                printf("%s\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Metodo main no existe.");
                                exit(1);
                            }
                            showCIList(ciList);
                            generateAsm(ciList, argv[0]);
                        }

    | list_var_decl     {
                            printf("%s\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Metodo main no existe.");
                            exit(1);
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
    method_decl {}
    | method_decl list_method_decl  {}
;

var_decl:
    type ID';'  {
                    Node *new = newVar($2->id, $1, 0, $2->noLine);
                    setOffset(new,(offsetCount-8));
                    offsetCount -= 8;
                    List *newL = newList(newL);
                    newL = insertLast(newL,new);
                    $$ = newL;
                }
    | type ID',' AuxId  {
                            Node *new = newVar($2->id, $1, 0, $2->noLine);
                            setOffset(new,(offsetCount-8));
                            offsetCount -= 8;
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
                Node *new = newVar($1->id, -10, 0, $1->noLine);
                setOffset(new,(offsetCount-8));
                offsetCount -= 8;
                List *newL = newList(newL);
                newL = insertFirst(newL,new);
                $$ = newL;
            }
    | ID',' AuxId  {
                        Node *new = newVar($1->id, -10, 0, $1->noLine);
                        setOffset(new,(offsetCount-8));
                        offsetCount -= 8;
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
                                            typeRet = $1->type;
                                            countReturn = 0;
                                            checkType($1->info->func.AST);
                                            if(countReturn == 0){
                                                printf("%s%s%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Return de metodo '",$1->info->func.id,"' no encontrado. Linea: ",$1->noline);
                                                exit(1);
                                            }
                                            setOffset($1,offsetCount);
                                            //offsetCount -= 8;
                                            insertInitIC($1);
                                            generateIC($1->info->func.AST);
                                            insertEndIC($1);
                                        }
    | method_aux1 '(' method_aux2 ')' method_aux3   {
                                                        tds = popLevel(tds);
                                                        tds = popLevel(tds);
                                                        $1->info->func.param = $3;
                                                        $1->info->func.AST = $5;
                                                        typeRet = $1->type;
                                                        countReturn = 0;
                                                        checkType($1->info->func.AST);
                                                        if(countReturn == 0){
                                                            printf("%s%s%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Return de metodo '",$1->info->func.id,"' no encontrado. Linea: ",$1->noline);
                                                            exit(1);
                                                        }
                                                        setOffset($1,offsetCount);
                                                        //offsetCount -= 8;
                                                        insertInitIC($1);
                                                        generateIC($1->info->func.AST);
                                                        insertEndIC($1);
                                                    }
;

method_aux1:
    type ID {
                offsetCount = 0;
                Node *new = newFunc($2->id,$1,NULL,NULL,$2->noLine);
                tds = pushTop(tds,new);
                $$ = new;
            }
    | VOID ID   {
                    offsetCount = 0;
                    Node *new = newFunc($2->id,2,NULL,NULL,$2->noLine);
                    tds = pushTop(tds,new);
                    $$ = new;
                }
;

method_aux2:
    {
        tds = pushNewLevel(tds);
    } TypeID    {
                    $$ = $2;
                }
;

method_aux3:
    block_aux   {
                    $$ = $1;
                }
;

TypeID:
    type ID     {
                    Node *new = newVar($2->id,$1,0,$2->noLine);
                    tds = pushTop(tds,new);
                    List *newL = newList(newL);
                    newL = insertFirst(newL,new);
                    $$ = newL;
                }
    | type ID','TypeID  {
                            Node *new = newVar($2->id,$1,0,$2->noLine);
                            tds = pushTop(tds,new);
                            List *newL;
                            newL = insertFirst($4,new);
                            $$ = newL;
                        }
;

block_aux:
    {
        tds = pushNewLevel(tds);
    } block {
                $$ = $2;
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
                                printf("%s%s%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Variable no declarada: ",$1->id,". Linea: ",$1->noLine);
                                exit(1);
	                        }
	                    }
    | method_call';'    {
                            $$ =$1;
                        }
    | IF '(' expr ')' block_aux ELSE block_aux  {
                                                    tds = popLevel(tds);
                                                    tds = popLevel(tds);
                                                    Node *root = newOp("ifElse", 1, $1->noLine);
                                                    insertTree(root, $3, $5, $7);
                                                    $$ = root;
                                                }
    | IF '(' expr ')' block_aux     {
                                        tds = popLevel(tds);
                                        Node *root = newOp("if", 1, $1->noLine);
                                        insertTree(root, $3, $5, NULL);
                                        $$ = root;
                                    }
    | WHILE '(' expr ')' block_aux  {
                                        tds = popLevel(tds);
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
                        Node *root = newOp("returnVoid", 2, $1->noLine);
                        insertTree(root, NULL, NULL, NULL);
                        $$ = root;
                    }
    | ';'   {
                Node *root = newOp("skip", 3, -1);
                insertTree(root, NULL, NULL, NULL);
                $$ = root;
            }
    | block_aux {
                    tds = popLevel(tds);
                    $$ = $1;
                }
;

method_call:
    ID '(' ')'  {
				    Node *funcTDS = findAll(tds, $1->id, 3);
				    if(funcTDS != NULL){
                        Node *root = newOp("functionVoid", funcTDS->type, $1->noLine);
					    insertTree(root, funcTDS, NULL, NULL);
					    $$ = root;
				    }else{
					    printf("%s%s%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Metodo no declarado: '",$1->id,"'. Linea: ",$1->noLine);
                        exit(1);
				    }
				}
    | ID '('AuxExpr')'  {
	                        Node *funcTDS = findAll(tds, $1->id, 3);
						    if(funcTDS != NULL){
                                Node *root = newOp("function", funcTDS->type, $1->noLine);
                                Node *auxFunc = newFunc("params",0,$3,NULL,$1->noLine);
							    insertTree(root, funcTDS, auxFunc, NULL);
							    $$ = root;
						    }else{
							    printf("%s%s%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Metodo no declarado: '",$1->id,"'. Linea: ",$1->noLine);
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
		        printf("%s%s%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Variable no declarada: '",$1->id,"'. Linea: ",$1->noLine);
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
void checkBoolCondition(Node *root){
    if ( root->type != 1){
        printf("%s%i\n", COLOR_RED"[ERROR]"COLOR_MAGENTA" Tipos incompatibles. Linea: ",root->noline);
        exit(1);
    }
}

void checkType(Node * root){
    if(root != NULL){
    	if(root->tag == 2){ // es operador
    		if(strcmp(root->info->op.id, "=") == 0){
    			if(root->left->type != root->mid->type){
    				printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Tipos incompatibles. Linea: ",root->noline);
    	        	exit(1);
    			}
    			checkType(root->mid);
    		}
    		if(strcmp(root->info->op.id, "function") == 0){
                if( (root->mid != NULL)&&(root->left->info->func.param != NULL) ){ // ninguno es null
                    if ( longList(root->left->info->func.param) == longList(root->mid->info->func.param) ){    // tienen la misma cantidad de param
        				List *a_param = root->left->info->func.param;
        				List *b_param = root->mid->info->func.param;
        				for (int cont=0; cont <=  longList(b_param); cont++){
        					if(a_param->node->type != b_param->node->type){
        						printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Tipos de parametros incompatibles. Linea: ",root->noline);
        	        			exit(1);
        					}else{
        						a_param = a_param->next;
        						b_param = b_param->next;
        					}
        				}
        			}else{
        				printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Cantidad de parametros incorrecta. Linea: ",root->noline);
        	        	exit(1);
        			}
        		}else{
        				printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Cantidad de parametros incorrecta. Linea: ",root->noline);
        	        	exit(1);
        		}
    		}
    		if(strcmp(root->info->op.id, "functionVoid") == 0){
    			if( (root->mid != NULL)||(root->left->info->func.param != NULL) ){  // alguno no es null
                		printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Tipos de parametros incompatibles. Linea: ",root->noline);
        	        	exit(1);
                }
            }
    		if(strcmp(root->info->op.id, "if") == 0){
                checkBoolCondition(root->left);
    			checkType(root->left);
    			checkType(root->mid);
    		}
    		if(strcmp(root->info->op.id, "ifElse") == 0){
                checkBoolCondition(root->left);
    			checkType(root->left);
    			checkType(root->mid);
    			checkType(root->right);
    		}
    		if(strcmp(root->info->op.id, "while") == 0){
                checkBoolCondition(root->left);
    			checkType(root->left);
    			checkType(root->mid);
    		}
    		if(strcmp(root->info->op.id, "return") == 0){
                countReturn++;
                if(root->type == typeRet){
                    checkType(root->left);
                }else{
                    printf("%s%i\n", COLOR_RED"[ERROR]"COLOR_MAGENTA" Valor de retorno incorrecto. Linea: ",root->noline);
                    exit(1);
                }
    		}
    		if(strcmp(root->info->op.id, "returnVoid") == 0){
                countReturn++;
                if(root->type != typeRet){
                    printf("%s%i\n", COLOR_RED"[ERROR]"COLOR_MAGENTA" Valor de retorno incorrecto. Linea: ",root->noline);
                    exit(1);
                }
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
    				printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Tipos incompatibles. Linea: ",root->noline);
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
    				printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Tipos incompatibles. Linea: ",root->noline);
    	            exit(1);
    			}
            }
    		if(strcmp(root->info->op.id, ">") == 0 || strcmp(root->info->op.id,"<") == 0){
    			if(root->left->type == 0 && root->mid->type == 0){
    				checkType(root->left);
    				checkType(root->mid);
    			}else{
    				printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Tipos incompatibles. Linea: ",root->noline);
    	            exit(1);
    			}
    		}
    		if(strcmp(root->info->op.id, "!") == 0 || strcmp(root->info->op.id,"negativo") == 0){
    			if(root->type == root->left->type){
    				checkType(root->left);
    			}else{
    				printf("%s%i\n",COLOR_RED"[ERROR]"COLOR_MAGENTA" Tipos incompatibles. Linea: ",root->noline);
    	            exit(1);
    			}
    		}
            if(strcmp(root->info->op.id, ";") == 0){
                checkType(root->left);
                checkType(root->mid);
            }
    	}
    }
}
