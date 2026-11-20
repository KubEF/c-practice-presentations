// Задача 7: node_at через list_get, list_set (связный список)
#include "../list.h"
#include <assert.h>
#include <stdio.h>

// Содержимое списка совпадает с want[0..n)
static void check(const list *l, const int *want, size_t n) {
    assert(list_size(l) == n);
    for (size_t i = 0; i < n; i++) {
        assert(list_get(l, i) == want[i]);
    }
}

int main(void) {
    list *l = list_create();
    assert(l != NULL);
    for (int v = 10; v <= 40; v += 10) {
        assert(list_push_back(l, v));
    }
    check(l, (int[]){10, 20, 30, 40}, 4);               // голова, середина, хвост
    list_set(l, 0, 1);
    list_set(l, 2, 3);
    list_set(l, 3, 4);
    check(l, (int[]){1, 20, 3, 4}, 4);
    list_destroy(l);
    puts("test07: ok");
    return 0;
}
