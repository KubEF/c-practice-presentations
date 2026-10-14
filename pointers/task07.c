#include <stdio.h>

int main(void) {
    int x = 0x01020304;
    unsigned char *b = (unsigned char *)&x;
    for (int i = 0; i < 4; i++)
        printf("%d ", b[i]);   // 4 3 2 1
    printf("\n");
    return 0;
}
