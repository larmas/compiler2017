#!/bin/bash
# -*- ENCODING: UTF-8 -*-

RESET='\033[0m'
BOLD='\033[1m'
ITALIC='\033[3m'
UNDERLINE='\033[4m'
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36'

cd src/
ROOT=../test

if [ $1 = '-c' ]; then
    for i in $(ls $ROOT)
    do
        echo "$2"
        echo "$i"
        if [ $i = $2 ]; then
            ./run.out $2
        else
            echo -e "${RED} Invalid file:  $2${RESET}"
        fi
    done
else
    echo -e "${RED} Invalid argument $1${RESET}"
    echo -e "${CYAN} Possible arguments: "
    echo -e "${CYAN}  -c : for normal compilation. "
fi
