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

FLEXRES=$( flex c-tds-lexico.l )
BISONRES=$( bison -v -d c-tds-sintaxis.y )
GCCRES=$( gcc -o run.out lex.yy.c c-tds-sintaxis.tab.c )

echo -e "${CYAN}${BOLD}\n-----COMPILE FLEX-----${RESET}"
if [ "$FLEXRES" == '' ]; then
    echo -e "${GREEN}${ITALIC} Success ✔ ${RESET}\n"
else
    echo -e "${RED} $FLEXRES ${RESET}\n"
fi

echo -e "${CYAN}-----COMPILE BISON-----${RESET}"
if [ "$BISONRES" == '' ]; then
    echo -e "${GREEN} Success ✔ ${RESET}\n"
else
    echo -e "${RED} $BISONRES ${RESET}\n"
fi

echo -e "${CYAN}-----COMPILE GCC-----${RESET}"
if [ "$GCCRES" == '' ]; then
    echo -e "${GREEN} Success ✔ ${RESET}\n"
else
    echo -e "${RED} $GCCRES ${RESET}\n"
fi
