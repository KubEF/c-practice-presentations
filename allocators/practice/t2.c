#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 5;
    int *squares = malloc(n * sizeof(int));
    for (int i = 0; i <= n; i++)
        squares[i] = i * i;
    for (int i = 0; i < n; i++)
        printf("%d ", squares[i]);
    printf("\n");
    free(squares);
    return 0;
}
