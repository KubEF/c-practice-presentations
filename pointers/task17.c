#include <stdio.h>

struct bad  { char a; double b; char c; int d; short e; };
struct good { double b; int d; short e; char a; char c; };

int main(void) {
    // 32 16
    printf("%zu %zu\n", sizeof(struct bad), sizeof(struct good));
    return 0;
}
