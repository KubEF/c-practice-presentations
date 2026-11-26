#include <stdio.h>
#include <string.h>
#include "bump.h"

int main(void) {
    char *name = bump_malloc(5);
    strcpy(name, "Anna");
    int *marks = bump_malloc(3 * sizeof(int));
    marks[0] = 5;
    marks[1] = 4;
    marks[2] = 5;
    double *avg = bump_malloc(sizeof(double));
    *avg = (marks[0] + marks[1] + marks[2]) / 3.0;
    printf("%s: %d %d %d, avg %.2f\n", name, marks[0], marks[1], marks[2], *avg);
    printf("name=%p marks=%p avg=%p\n", (void *)name, (void *)marks, (void *)avg);
    printf("used %zu bytes\n", bump_used());
    return 0;
}
