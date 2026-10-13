#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

int is_aligned(const void *p, size_t align) {
    return (uintptr_t)p % align == 0;
}

int main(void) {
    int arr[4] = {0};
    printf("%d\n", is_aligned(&arr[1], _Alignof(int)));         // 1
    printf("%d\n", is_aligned((char *)arr + 1, _Alignof(int))); // 0
    return 0;
}
