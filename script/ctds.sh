#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Script utilizado para correr un archivo particular.
# Usage: Situado en la raiz del proyecto correr el comando...
# $ ./script/ctds.sh <archivo.ctds>
# El archivo a correr debe estar en la carpeta test/codeinter

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'

cd test/test_codeinter/
DIRSRC=../../src
DIRTEST=../test/test_codeinter/


for i in $( ls )
do
    if [ $i = $1 ]; then
        cd $DIRSRC
        echo -e "${CYAN} RUNNING FILE: $i ${RESET}"
        ./run.out $DIRTEST$i
    fi
done
