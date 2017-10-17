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
CYAN='\033[0;36m'

cd src/

DIR=../test/test_sintactico

printf "Si al correr los test usted puede ver un mensaje de color ${RED}${BOLD}rojo${RESET} "
printf "significa que alguno de sus test falló,\nsi no ve ningun mensaje sus test "
printf "pasaron ${GREEN}correctamente${RESET}.\n"
printf "${CYAN}Sus test iniciaran en 3 segundos${RESET}\n\n"
sleep 3

for i in $(ls $DIR)
do
    printf "\n"
    echo -e "${GREEN}${BOLD}In file: ${RESET}${GREEN}'"$i"'${RESET}"
    ./run.out $DIR/$i
done
