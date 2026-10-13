#include <stdio.h>

int main(void) {
    char str[80] = "abcdefgh";
    char *p1 = str;

    printf("%c %c %c\n", str[4], *(p1 + 4), 4[str]);   // e e e
    printf("%zu %zu\n", sizeof str, sizeof p1);        // 80 8
    return 0;
}
