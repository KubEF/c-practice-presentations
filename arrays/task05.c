#include <stdio.h>

void move_zeroes(int *a, size_t n) {
    size_t w = 0;
    for (size_t r = 0; r < n; r++) {
        if (a[r] != 0) {
            a[w] = a[r];
            w++;
        }
    }
    while (w < n) {
        a[w] = 0;
        w++;
    }
}

int main(void) {
    int a[7] = {0, 1, 0, 3, 0, 0, 2};
    move_zeroes(a, 7);
    for (size_t i = 0; i < 7; i++) {
        printf("%d ", a[i]);    // 1 3 2 0 0 0 0
    }
    printf("\n");
    return 0;
}
