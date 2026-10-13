#include <stdio.h>

struct str1  { int big1:31; int small1:1; int big2:31; int small2:1; };
struct str2  { int big1:31; int big2:31;  int small1:1; int small2:1; };
struct flags { unsigned a:4; unsigned :0; unsigned b:4; };

int main(void) {
    printf("%zu %zu %zu\n", sizeof(struct str1), sizeof(struct str2),
           sizeof(struct flags));   // 8 12 8
    return 0;
}
