// без этого include компилятор не знает, что такое distance
#include "distance.h"
#include <stdio.h>

int main(int argc, char **argv) {
    int a;
    int b;

    // без аргументов argv[1] == NULL, и sscanf падал с segmentation fault
    if (argc != 3) {
        fprintf(stderr, "Usage: %s A B\n", argv[0]);    // Usage: ./prog A B
        return 1;
    }
    if (sscanf(argv[1], "%d", &a) != 1 || sscanf(argv[2], "%d", &b) != 1) {
        fprintf(stderr, "Error: A and B must be integers\n");
        return 1;
    }
    printf("%d\n", distance(a, b));                     // ./prog 10 3: 7
    return 0;
}
