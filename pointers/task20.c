#include <stdio.h>

union value { unsigned int u; float f; } val;

int main(void) {
    val.f = 1.0f;
    // 0x3f800000 4
    printf("%#x %zu\n", val.u, sizeof val);
    printf("%zu\n", sizeof(union { char c[5]; int i; }));  // 8
    return 0;
}
