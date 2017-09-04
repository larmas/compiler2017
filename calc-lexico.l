%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "calc-sintaxis.tab.h"

%}

%option noyywrap
%option yylineno

alpha [a-zA-z]
digit [0-9]

%%
"main"    {return MAIN;}
"boolean" {return BOOLEAN;}
"else"    {return ELSE;}
"false"   {return FALSE;}
"if"      {return IF;}
"int"     {return INT;}
"return"  {return RETURN;}
"true"    {return TRUE;}
"void"    {return VOID;}
"while"   {return WHILE;}
"+"   {return MAS;}
"-"   {return MENOS;}
"*"   {return MULT;}
"/"   {return DIV;}
"%"   {return MOD;}
"<"   {return MENOR;}
">"   {return MAYOR;}
"="   {return ASIGN;}
"=="   {return IGUAL;}
"&&"   {return AND;}
"||"   {return OR;}



{digit}{digit}*         {             
                            yylval.i = atoi(yytext);
                            //printf("INT : %d\n",atoi(yytext));
                            return INT_LITERAL;
                        }



{alpha}({alpha}|{digit}|MENOS)*     {
                                        yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                        strcpy(yylval.s,yytext);
                                        printf("ID : %s\n",yytext);
                                        return ID;
                                    }

[;,()!{}]                   {
                                printf("%s\n",yytext);
                                return *yytext;
                            }

.|\n                          ; /* ignore all the rest */

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
