#include <stdio.h>

int main(void) {
    int x = 5;
    int *a, b;                                 // a: int*, b: int
    printf("%zu %zu\n", sizeof a, sizeof b);   // 8 4

    int *c, *d;                                // два указателя
    c = &x;
    d = &x;
    x = 7;
    printf("%d %d\n", *c, *d);                 // 7 7
    return 0;
}
