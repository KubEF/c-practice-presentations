#include <stdio.h>
#include <string.h>
#include "arena.h"

int main(void) {
    arena_t *a = arena_create(1024);
    if (a == NULL)
        return 1;
    char *name = arena_alloc(a, 5);
    strcpy(name, "Anna");
    int *marks = arena_alloc(a, 3 * sizeof(int));
    marks[0] = 5;
    marks[1] = 4;
    marks[2] = 5;
    double *avg = arena_alloc(a, sizeof(double));
    *avg = (marks[0] + marks[1] + marks[2]) / 3.0;
    printf("%s: %d %d %d, avg %.2f\n", name, marks[0], marks[1], marks[2], *avg);
    printf("name=%p marks=%p avg=%p\n", (void *)name, (void *)marks, (void *)avg);
    printf("used %zu bytes\n", arena_used(a));
    arena_destroy(a);
    return 0;
}
