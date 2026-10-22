#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

typedef int (*binop_t)(int, int);

binop_t get_op(char op) {
    switch (op) {
    case '+':
        return add;
    case '-':
        return sub;
    case '*':
        return mul;
    default:
        return NULL;
    }
}

int main(void) {
    const char ops[] = {'+', '-', '*'};

    for (size_t i = 0; i < 3; i++) {
        binop_t op = get_op(ops[i]);
        printf("%c: %d\n", ops[i], op(6, 3));   // +: 9, -: 3, *: 18
    }
    return 0;
}
