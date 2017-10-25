#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Script utilizado para compilar y correr un archivo particular.
# El script permite compilacion total, compilacion parcial y ejecucion del proyecto.
# Usage: Situado en la raiz del proyecto correr el comando...
# $ ./script/ctds.sh <opcion> <archivo.ctds>
# El archivo a correr debe estar en la carpeta test/

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'

cd test/
DIRSRC=../src
DIRTEST=../test
FLAG=false
DIR=''

#case $1 in
#    '-o')
#        if [ ! -f $2 ]; then
#            for i in $( ls )
#            do
#                if [ $i = $3 ]; then
#                    cd $DIRSRC
#                    FLAG=true
#                    printf  "\n\n${CYAN}-----COMPILE FLEX-----${RESET}"
#                    flex c-tds-lexico.l
#                    printf "\n\n${CYAN}-----COMPILE BISON-----${RESET}"
#                    bison -v -d c-tds-sintaxis.y
#                    printf "\n\n${CYAN}-----COMPILE GCC-----${RESET}\n"
#                    gcc -o $2 lex.yy.c c-tds-sintaxis.tab.c
#                    printf "\n\n${CYAN}-----EXECUTE-----${RESET}\n"
#                    ./$2 $DIRTEST/$3
#                fi
#            done
#            if [ $FLAG = false ]; then
#                printf "${RED} Invalid file: $3${RESET}"
#            fi
#        else
#            if [ -f $2 ]; then
#                printf "${RED}\n [ERROR] Debe proporcionar un identificador para el ejecutable.${RESET}\n"
#            fi
#        fi
#        ;;
#    '-target')
#        case $2 in
#            'scan')
#
#                ;;
#            'parse')
#
#                ;;
#            'codinter')
#
#                ;;
#            'assembly')
#
#                ;;
#        esac
#        ;;
#    '-opt')
#        ;;
#    *)
#        echo -e "${RED} Invalid argument $1${RESET}"
#        echo -e "${CYAN} Possible arguments: ${RESET}"
#        echo -e "${CYAN} -o <name>                    : for rename the executable <name>. ${RESET}"
#        echo -e "${CYAN} -target <stage>              : <stage> is a scan, parse, codinter, or assembly.\
# The compilation proceeds to the given stage. ${RESET}"
#        echo -e "${CYAN} -opt [list of optimizations] : Make the list of optimizations.\
# The all option performs all the supported optimizations. ${RESET}\n"
#        ;;
#    esac

for i in $( ls )
do
    if [ $i = $1 ]; then
        cd $DIRSRC
        echo -e "${CYAN} RUNNING...${RESET}"
        ./run.out $DIRTEST/$i
    fi
done
