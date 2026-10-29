#include <stdio.h>

int binary_search(const int *a, int n, int key) {
    int lo = 0;
    int hi = n - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == key) {
            return mid;
        }
        if (a[mid] < key) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

int main(void) {
    int a[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    int n = (int)(sizeof(a) / sizeof(a[0]));

    printf("index(13) = %d\n", binary_search(a, n, 13));
    printf("index(2)  = %d\n", binary_search(a, n, 2));
    printf("index(4)  = %d\n", binary_search(a, n, 4));
    return 0;
}
