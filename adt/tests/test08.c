// Задача 8: list_push_back через tail (связный список)
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
    assert(list_push_back(l, 1));           // в пустой: head == tail
    check(l, (int[]){1}, 1);
    assert(list_push_back(l, 2));
    assert(list_push_back(l, 3));
    check(l, (int[]){1, 2, 3}, 3);
    list_destroy(l);
    puts("test08: ok");
    return 0;
}
