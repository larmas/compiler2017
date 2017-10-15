#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Script utilizado para correr los test del proyecto
# Usage: Situado en la raiz del proyecto correr el comando..
#  $ ./script/runTestSintactico.sh

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'

cd src/

DIR=../test/test_sintactico
SRC=src/

for i in $(ls $DIR)
do
    printf "\n"
    echo -e "${GREEN}${BOLD}In file: ${RESET}${GREEN}'"$i"'${RESET}"
    printf "${RED}"
    ./run.out $DIR/$i
    printf "${RESET}"
done
