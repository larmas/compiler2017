#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int function2(){
    int x;
    x = -1;
    return -2 > x;
}
int main(int argc, char const *argv[]) {
    printf("%i\n", function2());
    return 0;
}
