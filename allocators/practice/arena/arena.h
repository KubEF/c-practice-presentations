#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct {
    char *start;  // начало арены
    char *ptr;    // следующая свободная позиция
    char *end;    // конец арены
} arena_t;

// Создаёт арену на size байт. Нет памяти — NULL.
arena_t *arena_create(size_t size);

// Выделяет n байт из арены по адресу, кратному 8.
// Не помещается — NULL.
void *arena_alloc(arena_t *a, size_t n);

// Возвращает, сколько байт арены уже занято.
size_t arena_used(const arena_t *a);

// Освобождает всю арену целиком.
void arena_destroy(arena_t *a);

#endif
