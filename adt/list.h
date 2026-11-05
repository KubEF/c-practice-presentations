#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

// Список целых чисел; устройство struct list скрыто от пользователя
// Везде, кроме list_destroy, l != NULL
typedef struct list list;

// Пустой список или NULL, если не хватило памяти
list *list_create(void);
// Освобождает всю память списка; l == NULL допустим
void list_destroy(list *l);

size_t list_size(const list *l);

// Требуют i < list_size(l)
int list_get(const list *l, size_t i);
void list_set(list *l, size_t i, int value);

// false, если не хватило памяти; список при этом не меняется
bool list_push_back(list *l, int value);
// Требует i <= list_size(l)
bool list_insert(list *l, size_t i, int value);
// Требует i < list_size(l)
void list_remove(list *l, size_t i);

#endif
