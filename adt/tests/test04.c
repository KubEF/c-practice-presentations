// Задача 4: list_push_back и рост ёмкости 4 -> 8 -> 16 -> ... (массив)
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
    int want[100];
    for (int v = 0; v < 100; v++) {
        assert(list_push_back(l, v));
        want[v] = v;
        check(l, want, (size_t)v + 1);  // старые элементы пережили realloc
    }
    list_destroy(l);
    puts("test04: ok");
    return 0;
}
