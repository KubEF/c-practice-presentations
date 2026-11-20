// Задача 5: list_insert, list_remove (массив)
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
    assert(list_insert(l, 0, 20));          // в пустой
    assert(list_insert(l, 0, 10));          // в начало
    assert(list_insert(l, 2, 40));          // в конец, i == size
    assert(list_insert(l, 2, 30));          // в середину
    check(l, (int[]){10, 20, 30, 40}, 4);
    assert(list_insert(l, 4, 50));          // пятый элемент: realloc
    check(l, (int[]){10, 20, 30, 40, 50}, 5);
    list_remove(l, 0);                      // из начала
    check(l, (int[]){20, 30, 40, 50}, 4);
    list_remove(l, 1);                      // из середины
    check(l, (int[]){20, 40, 50}, 3);
    list_remove(l, 2);                      // из конца
    check(l, (int[]){20, 40}, 2);
    list_remove(l, 0);
    list_remove(l, 0);
    assert(list_size(l) == 0);
    assert(list_push_back(l, 7));           // после опустошения снова работает
    check(l, (int[]){7}, 1);
    list_destroy(l);
    puts("test05: ok");
    return 0;
}
