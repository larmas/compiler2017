#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Script utilizado para correr los test de generacion de assembler.
# Usage: Situado en la raiz del proyecto correr el comando..
#  $ ./script/runTestAssembly.sh

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
UNAME="$( uname -s )"
MACHINE=""

cd src/

DIR=../test/test_assembly

printf "Si al correr los test usted puede ver un mensaje de color ${RED}${BOLD}rojo${RESET} "
printf "significa que alguno de sus test falló,\nsi no ve ningun mensaje sus test "
printf "pasaron ${GREEN}correctamente${RESET}.\n"
printf "${CYAN}Sus test iniciaran en 3 segundos...${RESET}"
sleep 1
printf "${CYAN}2 segundos...${RESET}"
sleep 1
printf "${CYAN}1 segundo...${RESET}\n\n"
sleep 1

case "$UNAME" in
    Linux*)     MACHINE=Linux;;
    Darwin*)    MACHINE=Mac;;
    *)          MACHINE="UNKNOWN:$UNAME"
esac

for i in $(ls $DIR)
do
    printf "\n"
    echo -e "${GREEN}${BOLD}In file: ${RESET}${GREEN}'"$i"'${RESET}"
    ./run.out $DIR/$i $MACHINE
done
