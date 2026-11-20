// Задача 1: list.h компилируется, повторное подключение безопасно,
// прототипы совпадают с условием (включая const)
#include "../list.h"

#define HAS_TYPE(f, type) _Generic(&(f), type: 1, default: 0)

_Static_assert(HAS_TYPE(list_create, list *(*)(void)), "list_create");
_Static_assert(HAS_TYPE(list_destroy, void (*)(list *)), "list_destroy");
_Static_assert(HAS_TYPE(list_size, size_t (*)(const list *)), "list_size");
_Static_assert(HAS_TYPE(list_get, int (*)(const list *, size_t)), "list_get");
_Static_assert(HAS_TYPE(list_set, void (*)(list *, size_t, int)), "list_set");
_Static_assert(HAS_TYPE(list_push_back, bool (*)(list *, int)), "list_push_back");
_Static_assert(HAS_TYPE(list_insert, bool (*)(list *, size_t, int)), "list_insert");
_Static_assert(HAS_TYPE(list_remove, void (*)(list *, size_t)), "list_remove");

int main(void) {
    list *l = NULL;     // указатель на неполный тип объявить можно
    (void)l;
    return 0;
}
