#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 4;
    int *arr = malloc(n * sizeof *arr);
    if (!arr) {
        return 1;
    }

    arr[0] = 42;
    printf("%d\n", arr[0]);

    free(arr);
    return 0;
}
