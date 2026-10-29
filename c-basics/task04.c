#include <stdio.h>

int quotient(int a, int b) {
    int sign = 1;
    if (a < 0) {
        a = -a;
        sign = -sign;
    }
    if (b < 0) {
        b = -b;
        sign = -sign;
    }

    int q = 0;
    while (a >= b) {
        a = a - b;
        q = q + 1;
    }
    return sign * q;
}

int main(void) {
    printf("17 / 5  = %d\n", quotient(17, 5));
    printf("20 / 4  = %d\n", quotient(20, 4));
    printf("7 / 2   = %d\n", quotient(7, 2));
    printf("-17 / 5 = %d\n", quotient(-17, 5));
    printf("17 / -5 = %d\n", quotient(17, -5));
    return 0;
}
