%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "c-tds-sintaxis.tab.h"

%}

%option noyywrap
%option yylineno
%x comment

alpha [a-zA-z]
digit [0-9]

%%

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
"=="  {return IGUAL;}
"&&"  {return AND;}
"||"  {return OR;}
"!"  {return NOT;}



{digit}{digit}*         {
                            yylval.i = atoi(yytext);
                            //printf("INT : %d\n",atoi(yytext));
                            return INT_LITERAL;
                        }



{alpha}({alpha}|{digit}|'_')*     {
                                        yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                        strcpy(yylval.s,yytext);
                                        //printf("ID : %s\n",yytext);
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
<<EOF>>     {
                printf("%s%i%s\n", "Compiled successfully, ",yylineno," compiled lines.");
                return EOF;
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
