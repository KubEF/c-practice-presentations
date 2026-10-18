#include <assert.h>
#include <stdio.h>

int array_sum(const int *a, size_t n) {
    int sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

int array_max(const int *a, size_t n) {
    assert(n > 0);
    int max = a[0];
    for (size_t i = 1; i < n; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

int main(void) {
    int a[5] = {3, -1, 7, 7, 2};
    printf("sum=%d max=%d\n", array_sum(a, 5), array_max(a, 5));   // sum=18 max=7
    return 0;
}
