#include <stdio.h>

// to и from могут перекрываться: при to == from + 1 каждый шаг читает
// только что записанное, поэтому компилятор обязан копировать по одному
void arr_copy(int *to, int *from, int n) {
    for (int i = 0; i < n; ++i)
        to[i] = from[i];
}

// restrict обещает, что области не перекрываются,
// и компилятор вправе заменить весь цикл вызовом memcpy
void arr_copy_restrict(int *restrict to, int *restrict from, int n) {
    for (int i = 0; i < n; ++i)
        to[i] = from[i];
}

int main(void) {
    int src[4] = {1, 2, 3, 4};
    int dst[4];

    arr_copy_restrict(dst, src, 4);
    for (int i = 0; i < 4; ++i)
        printf("%d ", dst[i]); // 1 2 3 4
    printf("\n");
    return 0;
}
