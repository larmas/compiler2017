#!/bin/bash

#Usage $ ./compile.sh

#Cambio al directorio src
cd ../src/

printf "\n-----COMPILE FLEX-----"
flex c-tds-lexico.l
printf "\n\n-----COMPILE BISON-----"
bison -v -d c-tds-sintaxis.y
printf "\n\n-----COMPILE GCC-----\n"
gcc -o run lex.yy.c c-tds-sintaxis.tab.c
