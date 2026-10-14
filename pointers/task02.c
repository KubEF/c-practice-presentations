#include <stdio.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void bad_swap(int a, int b) {   // работает с копиями
    int t = a;
    a = b;
    b = t;
}

int main(void) {
    int x = 3, y = 7;
    bad_swap(x, y);
    printf("%d %d\n", x, y);   // 3 7
    swap(&x, &y);
    printf("%d %d\n", x, y);   // 7 3
    return 0;
}
