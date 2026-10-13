#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void *a, void *b, size_t size) {
    char *tmp = malloc(size);
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    free(tmp);
}

int main(void) {
    int x = 1, y = 2;
    double d1 = 1.5, d2 = 2.5;
    swap(&x, &y, sizeof x);
    swap(&d1, &d2, sizeof d1);
    printf("%d %d %.1f %.1f\n", x, y, d1, d2);   // 2 1 2.5 1.5
    return 0;
}
