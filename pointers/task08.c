#include <stdio.h>

int main(void) {
    int x = 10, y = 0;
    int *p = &x, **q = &p;

    printf("%d\n", **q);      // б: 10
    **q = 20;                 // в
    printf("%d\n", x);        // 20
    *q = &y;                  // г: p указывает на y
    *p = 5;
    printf("%d %d\n", x, y);  // 20 5
    return 0;
}
