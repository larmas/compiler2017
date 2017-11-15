#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Script utilizado para correr un archivo particular.
# Uso:
#   Situado en la raiz del proyecto correr el comando...
#   $ ./script/ctds.sh <archivo.ctds>
#   El archivo a correr debe estar en la carpeta test/test_assembly y, tanto
#   el archivo que contiene el codigo assembler correspondiente al caso de test
#   como su ejecutable, estará ubicado en el directorio /assembly.

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'

cd test/test_assembly
DIR_SRC=../../src
DIR_TEST=../test/test_assembly
DIR_ASM=../assembly
FLAG=false

case "$( uname -s )" in
    Linux*)     MACHINE=Linux;;
    Darwin*)    MACHINE=Mac;;
    *)          MACHINE="UNKNOWN:$UNAME"
esac

for i in $( ls )
do
    if [ $i = $1 ]; then
        cd $DIR_SRC
        RUN_ASM=$( ./run.out $DIR_TEST/$i $MACHINE )
        NAME=${i%.*}
        if [ "$RUN_ASM" != "" ]; then
            printf "\n"
            echo -e "${CYAN}IN FILE: $i ${RESET}"
            echo -e "${RED} $RUN_ASM ${RESET}"
        else
            RUN_EXE= $( gcc -o $NAME $NAME".s" )
            if [ "$RUN_EXE" != "" ]; then
                echo -e "${CYAN}IN FILE: $NAME".s" ${RESET}"
                echo -e "${RED} $RUN_EXE ${RESET}"
            else
                echo -e "${GREEN}OK!! ${RESET}"
            fi
        fi
        mv $NAME".s" $DIR_ASM
        mv $NAME $DIR_ASM
        FLAG=true
    fi
done
if [ "$FLAG" = false ]; then
    echo -e "${RED}[ERROR] ${MAGENTA} No such file $1 ${RESET}"
fi
