#include <stdio.h>
#include <stdlib.h>

int cmp_desc(const void *pa, const void *pb) {
    int a = *(const int *)pa, b = *(const int *)pb;
    return (a < b) - (a > b);
}

int main(void) {
    int v[] = {5, 2, 9, 1, 7};
    qsort(v, 5, sizeof v[0], cmp_desc);
    for (int i = 0; i < 5; i++)
        printf("%d ", v[i]);   // 9 7 5 2 1
    printf("\n");
    return 0;
}
