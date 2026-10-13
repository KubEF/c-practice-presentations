#include <stdio.h>

int sum(const int *begin, const int *end) {
    int s = 0;
    for (const int *p = begin; p != end; p++)
        s += *p;
    return s;
}

int main(void) {
    int a[] = {10, 20, 30, 40, 50};
    printf("%d %d\n", sum(a, a + 5), sum(a, a));   // 150 0
    return 0;
}
