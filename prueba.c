#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int z1, z2, z3, z4, z5, z6, z7;

int incr(int x1, int x2, int x3, int x4, int x5, int x6){
    return x1 + 1;
}

int main(){
    z1 = 1;
    incr(z1,z2,z3,z4,z5,z6);
    return 1;
}
