#include "list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Только через интерфейс: устройство списка здесь неизвестно
static void list_print(const list *l) {
    printf("[");
    for (size_t i = 0; i < list_size(l); i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", list_get(l, i));
    }
    printf("]\n");
}

// Индекс первого вхождения value или -1
static ptrdiff_t list_index_of(const list *l, int value) {
    for (size_t i = 0; i < list_size(l); i++) {
        if (list_get(l, i) == value) {
            return (ptrdiff_t)i;
        }
    }
    return -1;
}

int main(void) {
    list *l = list_create();
    if (l == NULL) {
        return EXIT_FAILURE;
    }
    bool ok = true;
    for (int v = 10; v <= 50; v += 10) {
        ok = ok && list_push_back(l, v);
    }
    ok = ok && list_insert(l, 0, 5);
    ok = ok && list_insert(l, 3, 25);
    ok = ok && list_insert(l, list_size(l), 60);
    if (!ok) {
        list_destroy(l);
        return EXIT_FAILURE;
    }
    list_print(l);                          // [5, 10, 20, 25, 30, 40, 50, 60]

    list_remove(l, 0);
    list_remove(l, 2);
    list_remove(l, list_size(l) - 1);
    if (!list_push_back(l, 70)) {
        list_destroy(l);
        return EXIT_FAILURE;
    }
    list_set(l, 1, 99);
    list_print(l);                          // [10, 99, 30, 40, 50, 70]
    printf("%d %zu\n", list_get(l, 1), list_size(l));                 // 99 6
    printf("%td %td\n", list_index_of(l, 40), list_index_of(l, 7));   // 3 -1

    list_destroy(l);
    return EXIT_SUCCESS;
}
