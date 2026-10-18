#include <stdio.h>

size_t uniq(int *a, size_t n) {
    if (n == 0) {
        return 0;
    }
    size_t k = 1;
    for (size_t i = 1; i < n; i++) {
        if (a[i] != a[k - 1]) {
            a[k] = a[i];
            k++;
        }
    }
    return k;
}

int main(void) {
    int a[9] = {1, 1, 2, 2, 2, 3, 5, 5, 8};
    size_t k = uniq(a, 9);
    printf("k=%zu: ", k);       // k=5:
    for (size_t i = 0; i < k; i++) {
        printf("%d ", a[i]);    // 1 2 3 5 8
    }
    printf("\n");
    return 0;
}
