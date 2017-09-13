#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Usage: Situado en la raiz del proyecto correr el comando..
#  $ ./script/runTest.sh

cd src/

DIR=../test/
SRC=src/

for i in $(ls $DIR)
do
    printf "\n"
    echo "File: '"$i"'"
    ./run $DIR/$i
done
