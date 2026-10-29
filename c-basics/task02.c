#include <stdio.h>

unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= (unsigned long long)i;
    }
    return result;
}

int main(void) {
    for (int n = 0; n <= 10; n++) {
        printf("%2d! = %llu\n", n, factorial(n));
    }
    return 0;
}
