#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = NULL;

    free(p);                    // free(NULL) безопасен
    printf("%d\n", p == NULL);  // 1

    p = malloc(sizeof *p);
    if (p == NULL)
        return 1;

    *p = 5;
    printf("%d\n", *p);         // 5

    free(p);
    p = NULL;                   // иначе останется висячий указатель
    return 0;
}
