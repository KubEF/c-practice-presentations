#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

int apply(int (*op)(int, int), int a, int b) {
    return op(a, b);
}

int main(void) {
    int (*ops[])(int, int) = {add, sub, mul};
    for (int i = 0; i < 3; i++)
        printf("%d ", apply(ops[i], 6, 3));   // 9 3 18
    printf("\n");
    return 0;
}
