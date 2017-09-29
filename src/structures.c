#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct tokenLine{
    char id[20];
    int noLine;
}TokenLine;

typedef struct constLine{
    int value;
    int noLine;
}ConstLine;
