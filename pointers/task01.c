#include <stdio.h>

int main(void) {
    int x = 10;
    int *p = &x;

    printf("%p\n", &x);          // а: 10
    *p = 20;                     // б
    int y = *p;                  // в
    y = 99;                      // г
    printf("x=%d y=%d\n", x, y); // x=20 y=99
    return 0;
}
