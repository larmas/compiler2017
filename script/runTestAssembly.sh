#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Script utilizado para correr los test de generación de assembler.
# Uso: Situado en la raiz del proyecto correr el comando..
#  $ ./script/runTestAssembly.sh
#  El archivo que contiene el código assembler generado para cada caso de test
#  se ubicará en el directorio /assembly.

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
UNAME="$( uname -s )"
MACHINE=""

cd src/

DIR_TEST=../test/test_assembly
DIR_ASM=../assembly
RUN=" "
TOTAL_TEST=0
TEST_FAILURE=0

printf "Si al correr los test usted puede ver un mensaje de color ${RED}${BOLD}rojo${RESET} "
printf "significa que alguno de sus test falló,\nsi no ve ningun mensaje sus test "
printf "pasaron ${GREEN}correctamente${RESET}.\n"

case "$UNAME" in
    Linux*)     MACHINE=Linux;;
    Darwin*)    MACHINE=Mac;;
    *)          MACHINE="UNKNOWN:$UNAME"
esac

for i in $(ls $DIR_TEST)
do
    TOTAL_TEST=$((TOTAL_TEST+1))
    RUN=`./run.out $DIR_TEST/$i $MACHINE`
    if [[ $RUN != "" ]]; then
        printf "\n"
        echo -e "${GREEN}${BOLD}In file: ${RESET}${GREEN}'"$i"'${RESET}"
        echo -e "${RED} $RUN ${RESET}"
        TEST_FAILURE=$((TEST_FAILURE+1))
    fi
done
printf "\n"
echo -e "${CYAN}${BOLD}Resultados: ${RESET}"
echo -e "${MAGENTA}   Total:${RESET} $TOTAL_TEST "
echo -e "${GREEN}   Correctos: ${RESET} $((TOTAL_TEST-TEST_FAILURE)) "
echo -e "${RED}   Incorrectos: ${RESET} $TEST_FAILURE"

for i in $( ls )
do
    if [[ ${i##*.} = "s" ]]; then
        mv $i $DIR_ASM
    fi
done
