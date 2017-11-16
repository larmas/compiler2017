#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * Estructura utilizada para representar un token y guardar el numero de
 * linea donde es definido.
 */
typedef struct tokenLine{
    char id[256];
    int noLine;
}TokenLine;
