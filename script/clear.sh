#!/bin/bash
# -*- ENCODING: UTF-8 -*-

#Script utilizado para eliminar todos los archivos de compilacion
#Usage: Situado en la raiz del proyecto correr el comando..
# $ ./script/clear.sh

RESET='\033[0m'
GREEN='\033[0;32m'

cd src/
for i in $( ls )
do
    case $i in
        lex.yy.c)
            rm lex.yy.c
            ;;
        c-tds-sintaxis.output)
            rm  c-tds-sintaxis.output
            ;;
        c-tds-sintaxis.tab.h)
            rm c-tds-sintaxis.tab.h
            ;;
        c-tds-sintaxis.tab.c)
            rm c-tds-sintaxis.tab.c
            ;;
        *.out)
            rm $i
            ;;
        *)
            if [[ -x "$i" ]]; then
                rm $i
            fi
            ;;
        esac

done

echo -e "${GREEN}Clean directory ✔︎${RESET}"
