#include <stdio.h>

void fill_squares(int *out, size_t n) {
    for (size_t i = 0; i < n; i++)
        out[i] = (int)(i * i);
}

int main(void) {
    int buf[5];

    fill_squares(buf, 5);
    for (size_t i = 0; i < 5; i++)
        printf("%d ", buf[i]);   // 0 1 4 9 16
    printf("\n");
    return 0;
}
