#!/bin/bash
# -*- ENCODING: UTF-8 -*-

#Script utilizado para compilar el proyecto
#Usage: Situado en la raiz del proyecto correr el comando..
# $ ./script/compile.sh

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'

#Cambio al directorio src
cd src/

printf "${CYAN}\n-----COMPILE FLEX-----${RESET}"

flex c-tds-lexico.l

printf "\n\n${CYAN}-----COMPILE BISON-----${RESET}"

bison -d c-tds-sintaxis.y

printf "\n\n${CYAN}-----COMPILE GCC-----${RESET}\n"

gcc -o run.out lex.yy.c c-tds-sintaxis.tab.c
