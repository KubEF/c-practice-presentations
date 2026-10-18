#include <stdio.h>

int binary_search(const int *a, size_t n, int key) {
    size_t lo = 0;
    size_t hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a[mid] == key) {
            return (int)mid;
        }
        if (a[mid] < key) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return -1;
}

int main(void) {
    int a[8] = {1, 3, 5, 7, 9, 11, 13, 15};
    printf("%d\n", binary_search(a, 8, 7));    // 3
    printf("%d\n", binary_search(a, 8, 8));    // -1
    printf("%d\n", binary_search(a, 8, 1));    // 0
    printf("%d\n", binary_search(a, 8, 15));   // 7
    return 0;
}
