#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# Usage $ ./runTest.sh

cd ../src/

DIR=../test/
SRC=src/

for i in $(ls $DIR)
do
    printf "\n"
    echo "File: '"$i"'"
    ./run $DIR/$i
done
