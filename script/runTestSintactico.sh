#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Script utilizado para correr los test del proyecto
# Usage: Situado en la raiz del proyecto correr el comando..
#  $ ./script/runTestSintactico.sh

cd src/

DIR=../test/test_sintactico
SRC=src/

for i in $(ls $DIR)
do
    printf "\n"
    echo "In file: '"$i"'"
    ./run.out $DIR/$i
done
