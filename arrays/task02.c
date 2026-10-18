#include <stdio.h>

void reverse(int *a, size_t n) {
    for (size_t i = 0; i < n / 2; i++) {
        int t = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = t;
    }
}

int main(void) {
    int a[6] = {1, 2, 3, 4, 5, 6};
    reverse(a, 6);
    for (size_t i = 0; i < 6; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");           // 6 5 4 3 2 1
    return 0;
}
