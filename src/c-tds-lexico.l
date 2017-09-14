%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "c-tds-sintaxis.tab.h"
#include "structures.c"

%}

%option noyywrap
%option yylineno
%x comment

alpha [a-zA-z]
digit [0-9]

%%

"boolean"   {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "BOOLEAN");
                yylval.tokenLine->noLine = yylineno;
                return BOOLEAN;
            }

"else"      {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "ELSE");
                yylval.tokenLine->noLine = yylineno;
                return ELSE;
            }

"false"     {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "FALSE");
                yylval.tokenLine->noLine = yylineno;
                return FALSE;
            }

"if"        {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "IF");
                yylval.tokenLine->noLine = yylineno;
                return IF;
            }

"int"       {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "INT");
                yylval.tokenLine->noLine = yylineno;
                return INT;
            }

"return"    {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "RETURN");
                yylval.tokenLine->noLine = yylineno;
                return RETURN;
            }

"true"      {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "TRUE");
                yylval.tokenLine->noLine = yylineno;
                return TRUE;
            }

"void"      {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "VOID");
                yylval.tokenLine->noLine = yylineno;
                return VOID;
            }

"while"     {//Prueba de token con numero de linea
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "WHILE");
                yylval.tokenLine->noLine = yylineno;
                return WHILE;
            }

"+"         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "MAS");
                yylval.tokenLine->noLine = yylineno;
                return MAS;
            }

"-"         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "MENOS");
                yylval.tokenLine->noLine = yylineno;
                return MENOS;
            }

"*"         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "MULT");
                yylval.tokenLine->noLine = yylineno;
                return MULT;
            }

"/"         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "DIV");
                yylval.tokenLine->noLine = yylineno;
                return DIV;
            }

"%"         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "MOD");
                yylval.tokenLine->noLine = yylineno;
                return MOD;
            }

"<"         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "MENOR");
                yylval.tokenLine->noLine = yylineno;
                return MENOR;
            }

">"         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "MAYOR");
                yylval.tokenLine->noLine = yylineno;
                return MAYOR;
            }

"="         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "ASIGN");
                yylval.tokenLine->noLine = yylineno;
                return ASIGN;
            }

"=="        {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "IGUAL");
                yylval.tokenLine->noLine = yylineno;
                return IGUAL;
            }

"&&"        {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "AND");
                yylval.tokenLine->noLine = yylineno;
                return AND;
            }

"||"        {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "OR");
                yylval.tokenLine->noLine = yylineno;
                return OR;
            }

"!"         {
                yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                strcpy(yylval.tokenLine->id, "NOT");
                yylval.tokenLine->noLine = yylineno;
                return NOT;
            }




{digit}{digit}*         {
                            yylval.i = atoi(yytext);
                            return INT_LITERAL;
                        }



{alpha}({alpha}|{digit}|'_')*     {
                                        yylval.tokenLine = (TokenLine *)malloc(sizeof(TokenLine));
                                        strcpy(yylval.tokenLine->id,yytext);
                                        yylval.tokenLine->noLine = yylineno;
                                        return ID;
                                    }

[;,(){}]                   {
                                //printf("%s\n",yytext);
                                return *yytext;
                            }
(\n|\t|" ")*                {

                            }


"/*"                BEGIN(comment);
<comment>[^*\n]*
<comment>"*"+[^*/\n]*
<comment>\n
<comment>"*"+"/"    BEGIN(INITIAL);

"//".*\n  ; /* Ignore rest of line */

.       {
            printf("%s%s%s%i\n","Simbolo no valido: ",yytext," en la linea ",yylineno);
            exit(1);
        }

%%

void yyerror(){
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;

	yyparse();

}
