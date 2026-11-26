#include <stdlib.h>
#include "arena.h"

arena_t *arena_create(size_t size) {
    arena_t *a = malloc(sizeof(arena_t));
    if (a == NULL)
        return NULL;
    a->start = malloc(size);
    if (a->start == NULL) {
        free(a);
        return NULL;
    }
    a->ptr = a->start;
    a->end = a->start + size;
    return a;
}

void *arena_alloc(arena_t *a, size_t n) {
    size_t aligned = (n + 7) & ~(size_t)7;      // округляем вверх до кратного 8
    if (aligned > (size_t)(a->end - a->ptr))
        return NULL;                            // арена переполнена
    void *p = a->ptr;
    a->ptr += aligned;
    return p;
}

size_t arena_used(const arena_t *a) {
    return (size_t)(a->ptr - a->start);
}

void arena_destroy(arena_t *a) {
    free(a->start);
    free(a);
}
