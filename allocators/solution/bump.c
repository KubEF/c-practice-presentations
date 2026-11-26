#include "bump.h"

#define HEAP_SIZE 1024

static _Alignas(16) unsigned char heap[HEAP_SIZE];  // куча
static size_t top = 0;                              // номер первого свободного байта

void *bump_malloc(size_t n) {
    top = (top + 7) & ~(size_t)7;  // округляем вверх до кратного 8
    if (n == 0 || top + n > HEAP_SIZE)
        return NULL;
    void *p = heap + top;
    top += n;
    return p;
}

void bump_free(void *p) {
    (void)p;
}

size_t bump_used(void) {
    return top;
}
