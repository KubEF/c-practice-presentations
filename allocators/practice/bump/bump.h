#ifndef BUMP_H
#define BUMP_H

#include <stddef.h>

// Выделяет n байт из кучи.
// Если n == 0 или блок не помещается в кучу, возвращает NULL.
void *bump_malloc(size_t n);

// Освобождает блок p, выделенный bump_malloc.
void bump_free(void *p);

// Возвращает, сколько байт кучи уже занято.
size_t bump_used(void);

#endif
