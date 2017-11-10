#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int function2(){
    int x;
    x = -3;
    return x / -2;
}
int main(int argc, char const *argv[]) {
    printf("%i\n", function2());
    return 0;
}
