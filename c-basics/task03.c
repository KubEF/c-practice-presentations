#include <stdio.h>

int fib(int n) {
    if (n < 2) {
        return n;
    }
    //   return fib(n - 1) + fib(n - 2);
    int a = 0; // F(i - 2)
    int b = 1; // F (i - 1)
    for (int i = 1; i <= n; i++) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

int main(void) {
    for (int i = 0; i < 10; i++) {
        printf("%d ", fib(i));
    }
    printf("\n");
    return 0;
}
