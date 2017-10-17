#!/bin/bash
# -*- ENCODING: UTF-8 -*-

#Script utilizado para eliminar todos los archivos de compilacion
#Usage: Situado en la raiz del proyecto correr el comando..
# $ ./script/clear.sh

RESET='\033[0m'
GREEN='\033[0;32m'

cd src/

if [[ -f lex.yy.c ]]; then
    rm lex.yy.c
fi
if [[ -f c-tds-sintaxis.output ]]; then
    rm  c-tds-sintaxis.output
fi

if [[ -f c-tds-sintaxis.tab.h ]]; then
    rm c-tds-sintaxis.tab.h
fi

if [[ -f c-tds-sintaxis.tab.c ]]; then
    rm c-tds-sintaxis.tab.c
fi

if [[ -f run.out ]]; then
    rm run.out
fi

echo -e "${GREEN}Clean directory${RESET}"
