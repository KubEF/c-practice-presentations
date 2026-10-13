#include <stdio.h>

int get_or_default(const int *p, int def) {
    return p ? *p : def;
}

int main(void) {
    int v = 5;
    // 5 -1
    printf("%d %d\n", get_or_default(&v, 0), get_or_default(NULL, -1));
    return 0;
}
