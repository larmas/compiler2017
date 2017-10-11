%{
#include <stdlib.h>
#include <stdio.h>
#include "structures.c"
#include "stackSymbol.c"
#include "ciList.c"

Stack *tds;
List *list;
CIList *ciList;
int typeRet;
int countReturn;
//int tempCount;
//int labelCount;

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
    } program
;

program:
    list_var_decl list_method_decl  {
                                        //printf("---STACK---\n");
                                        //showStack(tds);
                                        //printf("--------\n");
                                        Node *main = findAll(tds,"main",3);
                                        if(main == NULL){
                                            printf("%s\n","ERROR: Metodo main no existe.");
                                            exit(1);
                                        }
                                        ciList = newCIList(ciList);
                                        List *index = $2;

                                        while (index != NULL){
                                            generateIC(index->node);
                                            index = index->next;
                                        }
                                        showCIList(ciList);
                                    }

    | list_method_decl  {
                            //printf("---STACK---\n");
                            //showStack(tds);
                            //printf("--------\n");
                            Node *main = findAll(tds,"main",3);
                            if(main == NULL){
                                printf("%s\n","ERROR: Metodo main no existe.");
                                exit(1);
                            }
                            List *index = $1;
                            while (index != NULL){
                                generateIC(index->node);
                                index = index->next;
                            }
                            showCIList(ciList);
                        }

    | list_var_decl     {
                            //printf("---STACK---\n");
                            //showStack(tds);
                            //printf("--------\n");
                            printf("%s\n","ERROR: Metodo main no existe.");
                            exit(1);
                        }

    | /* LAMBDA */  {
                        //printf("---STACK---\n");
                        //showStack(tds);
                        //printf("--------\n");
                        printf("%s\n","ERROR: Metodo main no existe.");
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
                Node *new = newVar($1->id, -10, 0, $1->noLine);
                List *newL = newList(newL);
                newL = insertFirst(newL,new);
                $$ = newL;
            }

    | ID',' AuxId  {
                        Node *new = newVar($1->id, -10, 0, $1->noLine);
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
                                            //dfs($1->info->func.AST);
                                            typeRet = $1->type;
                                            countReturn = 0;
                                            checkType($1->info->func.AST);
                                            if(countReturn == 0){
                                                printf("%s%s%s%i\n","ERROR: Return de metodo '",$1->info->func.id,"' no encontrado. Linea: ",$1->noline);
                                                exit(1);
                                            }
                                        }

    | method_aux1 '(' method_aux2 ')' method_aux3   {
                                                        tds = popLevel(tds);
                                                        tds = popLevel(tds);
                                                        $1->info->func.param = $3;
                                                        $1->info->func.AST = $5;
                                                        //dfs($1->info->func.AST);
                                                        typeRet = $1->type;
                                                        countReturn = 0;
                                                        checkType($1->info->func.AST);
                                                        if(countReturn == 0){
                                                            printf("%s%s%s%i\n","ERROR: Return de metodo '",$1->info->func.id,"' no encontrado. Linea: ",$1->noline);
                                                            exit(1);
                                                        }
                                                    }
;

method_aux1: /* retorna el nodo func */
    type ID {
                Node *new = newFunc($2->id,$1,NULL,NULL,$2->noLine);
                tds = pushTop(tds,new);
                $$ = new;
            }

    | VOID ID   {
                    Node *new = newFunc($2->id,2,NULL,NULL,$2->noLine);
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
                                printf("%s%s%s%i\n","ERROR: Variable no declarada: ",$1->id,". Linea: ",$1->noLine);
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
                        Node *root = newOp("returnVoid", 2, $1->noLine);
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
                            Node *root = newOp("functionVoid", funcTDS->type, $1->noLine);
						    insertTree(root, funcTDS, NULL, NULL);
						    $$ = root;
					    }else{
						    printf("%s%s%s%i\n","ERROR: Metodo no declarado: '",$1->id,"'. Linea: ",$1->noLine);
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
							    printf("%s%s%s%i\n","ERROR: Metodo no declarado: '",$1->id,"'. Linea: ",$1->noLine);
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
		        printf("%s%s%s%i\n","ERROR: Variable no declarada: '",$1->id,"'. Linea: ",$1->noLine);
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
        printf("%s%i\n", "ERROR: Tipos incompatibles. Linea: ",root->noline);
        exit(1);
    }
}

void checkType(Node * root){
	if(root->tag == 2){ // es operador
		if(strcmp(root->info->op.id, "=") == 0){
			if(root->left->type != root->mid->type){
				printf("%s%i\n","ERROR: Tipos incompatibles. Linea: ",root->noline);
	        	exit(1);
			}
			checkType(root->mid);
		}

		if(strcmp(root->info->op.id, "function") == 0){
            if( (root->mid != NULL)&&(root->left->info->func.param != NULL) ){ // ninguno es null

                if ( longList(root->left->info->func.param) == longList(root->mid->info->func.param) ){    // tienen la misma cantidad de param
    				List *a_param = root->left->info->func.param;
    				List *b_param = root->mid->info->func.param;
    				for (int cont=0; cont <=  longList(b_param); cont++){ // controlo q cada parametro pasado tenga el mismo tipo
    					if(a_param->node->type != b_param->node->type){
    						printf("%s%i\n","ERROR: tipos de parametros incompatibles. Linea: ",root->noline);
    	        			exit(1);
    					}else{
    						a_param = a_param->next;
    						b_param = b_param->next;
    					}
    				}

    			}else{
    				printf("%s%i\n","ERROR: cantidad de parametros incorrecta. Linea: ",root->noline);
    	        	exit(1);
    			}

    		}else{
    				printf("%s%i\n","ERROR: cantidad de parametros incorrecta. Linea: ",root->noline);
    	        	exit(1);
    		}
		}

		if(strcmp(root->info->op.id, "functionVoid") == 0){
			if( (root->mid != NULL)||(root->left->info->func.param != NULL) ){  // alguno no es null
            		printf("%s%i\n","ERROR: tipos de parametros incompatibles. Linea: ",root->noline);
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
                printf("%s%i\n", "ERROR: Valor de retorno incorrecto. Linea: ",root->noline);
                exit(1);
            }
		}

		if(strcmp(root->info->op.id, "returnVoid") == 0){
            countReturn++;
            if(root->type != typeRet){
                printf("%s%i\n", "ERROR: Valor de retorno incorrecto. Linea: ",root->noline);
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
				printf("%s%i\n","ERROR: tipos incompatibles. Linea: ",root->noline);
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
				printf("%s%i\n","ERROR: tipos incompatibles. Linea: ",root->noline);
	            exit(1);
			}
        }

		if(strcmp(root->info->op.id, ">") == 0 || strcmp(root->info->op.id,"<") == 0){
			if(root->left->type == 0 && root->mid->type == 0){
				checkType(root->left);
				checkType(root->mid);
			}else{
				printf("%s%i\n","ERROR: tipos incompatibles. Linea: ",root->noline);
	            exit(1);
			}
		}

		if(strcmp(root->info->op.id, "!") == 0 || strcmp(root->info->op.id,"negativo") == 0){
			if(root->type == root->left->type){
				checkType(root->left);
			}else{
				printf("%s%i\n","ERROR: tipos incompatibles. Linea: ",root->noline);
	            exit(1);
			}
		}
        if(strcmp(root->info->op.id, ";") == 0){
            checkType(root->left);
            checkType(root->mid);
        }

	}
}

/*
    Antes de llamar a generateIC se debe inicializar tempCount, labelCount y newCIList
 */
Node *generateIC(Node *root){
    static int tempCount = 0;
    static int labelCount = 0;
    if((root->tag == 0)||(root->tag == 1)){  //es una constante o variable
        return root;
    }

    if(root->tag == 2){  //es un operador
        if (strcmp(root->info->op.id, "=") == 0){
            NodeCI *new = newNodeCI("MOV",generateIC(root->left),generateIC(root->mid), NULL);
            ciList = insertLastCI(ciList,new);

        }
        if (strcmp(root->info->op.id, "+") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("ADD",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "-") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("SUB",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "*") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("MULT",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "/") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("DIV",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "%") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("MOD",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, ">") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("MAY",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "<") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("MIN",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "&&") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("AND",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "||") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("OR",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "==") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,root->type,0,0);
            NodeCI *new = newNodeCI("EQUAL",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "!") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("NEGB",generateIC(root->left),NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "negativo") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("NEGI",generateIC(root->left),NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "return") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,root->type,0,0);
            NodeCI *new = newNodeCI("RETURN",NULL,NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "returnVoid") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,root->type,0,0);
            NodeCI *new = newNodeCI("RETURNV",NULL,NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }

        if (strcmp(root->info->op.id, "if") == 0){
            char labelId[20];
            char aux[20];
            //itoa(labelCount, aux, 10);
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,strcat("L",aux));
            Node *label0 = newVar(labelId,0,0,0);
            labelCount++;
            // condicion if
            Node *cond = generateIC(root->left);

            // IF temp L0 (si temp es false, salto hacia L0)
            NodeCI *nIF = newNodeCI("IFF",cond,label0, NULL);
            ciList = insertLastCI(ciList,nIF);

            // cuerpo if
            generateIC(root->mid);

            // LABEL L0
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);

            return NULL;
        }


        if (strcmp(root->info->op.id, "ifElse") == 0){
            char labelId[20];
            char aux[20];
            //itoa(labelCount, aux, 10);
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,strcat("L",aux));
            Node *label0 = newVar(labelId,0,0,0);
            labelCount++;


            //itoa(labelCount, aux, 10);
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,strcat("L",aux));
            Node *label1 = newVar(labelId,0,0,0);
            labelCount++;

            // condicion del ifElse
            Node *cond = generateIC(root->left);

            // IF temp L0 (si temp es false, salto hacia L0)
            NodeCI *nIF = newNodeCI("IFF",cond,label0, NULL);
            ciList = insertLastCI(ciList,nIF);

            // cuerpo del if
            generateIC(root->mid);

            // JMP L1
            NodeCI *jmp = newNodeCI("JMP",NULL,NULL, label1);
            ciList = insertLastCI(ciList,jmp);

            // LABEL L0
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);

            // cuerpo del Else
            generateIC(root->right);

            // LABEL L1
            NodeCI *l1 = newNodeCI("LABEL",NULL,NULL, label1);
            ciList = insertLastCI(ciList,l1);

            return NULL;
        }


        if (strcmp(root->info->op.id, "while") == 0){

            char labelId[20];
            char aux[20];
            //itoa(labelCount, aux, 10);
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,strcat("L",aux));
            Node *label0 = newVar(labelId,0,0,0);
            labelCount++;



            //itoa(labelCount, aux, 10);
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,strcat("L",aux));
            Node *label1 = newVar(labelId,0,0,0);
            labelCount++;

            // LABEL L0
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);

            // condicion del while
            Node *cond = generateIC(root->left);

            // IF temp L1 (si temp es false, salto hacia L1)
            NodeCI *nIF = newNodeCI("IFF",cond,label1, NULL);
            ciList = insertLastCI(ciList,nIF);

            // cuerpo del while
            generateIC(root->mid);

            // JMP L0
            NodeCI *jmp = newNodeCI("JMP",NULL,NULL, label0);
            ciList = insertLastCI(ciList,jmp);

            // LABEL L1
            NodeCI *l1 = newNodeCI("LABEL",NULL,NULL, label1);
            ciList = insertLastCI(ciList,l1);

            return NULL;
        }

        if (strcmp(root->info->op.id, "function") == 0){
            List *par = root->mid->info->func.param;
            while(par != NULL){
                NodeCI *load = newNodeCI("LOAD",par->node,NULL,NULL);
                ciList = insertLastCI(ciList,load);
                par = par->next;
            }

            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,root->left->type,0,0);
            NodeCI *callF = newNodeCI("CALL",root->left,newTemporal,NULL);
            ciList = insertLastCI(ciList,callF);
            tempCount++;

            return newTemporal;
        }

        if (strcmp(root->info->op.id, "functionVoid") == 0){
            char tempId[20];
            char aux = tempCount + '0';
            strcpy(tempId, strcat("T",&aux));
            Node *newTemporal = newVar(tempId,root->left->type,0,0);
            NodeCI *callF = newNodeCI("CALL",root->left,newTemporal,NULL);
            ciList = insertLastCI(ciList,callF);
            tempCount++;

            return newTemporal;
        }

        if (strcmp(root->info->op.id, ";") == 0){
            generateIC(root->left);
            generateIC(root->mid);
            return NULL;
        }
    }
    if(root->tag == 3){  // es funcion
        char nameFunc[20];

        strcpy(nameFunc,strcat("BEGIN ",root->info->func.id));
        NodeCI *beginF = newNodeCI(nameFunc,NULL,NULL,NULL);
        ciList = insertLastCI(ciList,beginF);

        generateIC(root->info->func.AST);

        strcpy(nameFunc,strcat("END ",root->info->func.id));
        NodeCI *endF = newNodeCI(nameFunc,NULL,NULL,NULL);
        ciList = insertLastCI(ciList,endF);
        return NULL;
    }   
    return NULL;
}
