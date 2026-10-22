#include <stdio.h>

int main(void) {
    char a[] = "Hello";         // массив: своя копия литерала
    const char *p = "Hello";    // указатель: на литерал

    a[0] = 'h';
    printf("%s\n", a);          // hello

    printf("%zu %zu\n", sizeof a, sizeof p);   // 6 8
    return 0;
}
