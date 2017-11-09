#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int function2(){
    int x,y;
    y = 2;
    x = 1 - y;
    return x;
}
int main(int argc, char const *argv[]) {
    printf("%i\n", function2());
    return 0;
}
