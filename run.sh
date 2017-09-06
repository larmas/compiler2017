#!/bin/bash
# -*- ENCODING: UTF-8 -*-
# Usage $ ./run.sh

DIR = /test

for i in $(ls $DIR)
do
./a.out $i
done
