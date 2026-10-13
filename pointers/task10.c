#include <stdio.h>

int main(void) {
    int a[5] = {10, 20, 30, 40, 50};
    int *p = a;
    p += 3;
    printf("%d\n", *p);                         // 40
    printf("%td\n", p - a);                     // 3
    printf("%td\n", (char *)p - (char *)a);     // 12

    double d[5] = {0};
    double *q = d + 3;
    printf("%td %td\n", q - d, (char *)q - (char *)d);   // 3 24
    return 0;
}
