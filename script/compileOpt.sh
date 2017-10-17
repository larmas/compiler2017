#!/bin/bash
# -*- ENCODING: UTF-8 -*-

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36'

cd test/
DIR=../src
FLAG=false

if [ $1 = '-c' ]; then
    for i in $( ls )
    do
        if [ $i = $2 ]; then
            cd $DIR
            FLAG=true
            printf "${CYAN}\n-----COMPILE GCC-----${RESET}\n"
            gcc -o run.out lex.yy.c c-tds-sintaxis.tab.c
        fi
    done
    if [ $FLAG = false ]; then
        printf "${RED} Invalid file:  $2${RESET}"
    fi
else
    if [ $1 = '-a' ]; then
        for i in $( ls )
        do
            if [ $i = $2 ]; then
                cd $DIR
                FLAG=true
                printf "${CYAN}\n-----COMPILE FLEX-----${RESET}\n"
                flex c-tds-lexico.l
                printf "${CYAN}\n\n-----COMPILE BISON-----${RESET}\n"
                bison -v -d c-tds-sintaxis.y
                printf "${CYAN}\n-----COMPILE GCC-----${RESET}\n"
                gcc -o run.out lex.yy.c c-tds-sintaxis.tab.c
            fi
        done
        if [ $FLAG = false ]; then
            printf "${RED} Invalid file:  $2${RESET}\n"
        fi
    else
        if [ $1 = '-f' ]; then
            for i in $( ls )
            do
                if [ $i = $2 ]; then
                    cd $DIR
                    FLAG=true
                    printf "${CYAN}\n-----COMPILE FLEX-----${RESET}\n"
                    flex c-tds-lexico.l
                fi
            done
            if [ $FLAG = false ]; then
                printf "${RED} Invalid file:  $2${RESET}\n"
            fi
        else
            if [ $1 = '-b' ]; then
                for i in $( ls )
                do
                    if [ $i = $2 ]; then
                        cd $DIR
                        FLAG=true
                        printf "${CYAN}\n\n-----COMPILE BISON-----${RESET}\n"
                        bison -v -d c-tds-sintaxis.y
                    fi
                done
                if [ $FLAG = false ]; then
                    printf "${RED} Invalid file:  $2${RESET}\n"
                fi
            else
                printf "${RED} Invalid argument $1${RESET}\n"
                printf "${CYAN} Possible arguments: \n"
                printf "${CYAN}  -a : for complete compilation. \n"
                printf "${CYAN}  -f : to compile flex. \n"
                printf "${CYAN}  -b : to compile bison. \n"
                printf "${CYAN}  -c : to compile C. \n"
            fi
        fi
    fi
fi
