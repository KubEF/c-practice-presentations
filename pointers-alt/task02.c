#include <stdio.h>
#include <stdlib.h>

void get_odd(const int *src, size_t n, int **dst, size_t *n_dst) {
    size_t len = 0;
    for (size_t i = 0; i < n; i++)
        if (src[i] % 2 != 0)
            len++;

    int *out = malloc(len * sizeof *out);
    if (out == NULL) {
        *dst = NULL;
        *n_dst = 0;
        return;
    }

    size_t j = 0;
    for (size_t i = 0; i < n; i++)
        if (src[i] % 2 != 0)
            out[j++] = src[i];

    *dst = out;
    *n_dst = len;
}

int main(void) {
    const int data[] = {1, 2, 3, 4, 5, 6, 7};
    int *odd = NULL;
    size_t n = 0;

    get_odd(data, 7, &odd, &n);
    printf("%zu:", n);                 // 4:
    for (size_t i = 0; i < n; i++)
        printf(" %d", odd[i]);         // 1 3 5 7
    printf("\n");
    free(odd);
    return 0;
}
