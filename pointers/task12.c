#include <stdio.h>

#define SIZE 3
int stack[SIZE];
int *current   = stack;
int *stack_end = stack + SIZE;

int push(int v) {
    if (current == stack_end) return 0;
    *current++ = v;
    return 1;
}

int pop(int *out) {
    if (current == stack) return 0;
    *out = *--current;
    return 1;
}

int main(void) {
    for (int i = 1; i <= 4; i++)
        printf("push %d -> %d\n", i, push(i));
    int v;
    while (pop(&v)) printf("pop %d\n", v);
    return 0;
}
