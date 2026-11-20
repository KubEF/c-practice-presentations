// Задача 2: list_create, list_destroy, list_size (массив)
#include "../list.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    list *l = list_create();
    assert(l != NULL);
    assert(list_size(l) == 0);
    list_destroy(l);
    list_destroy(NULL);
    puts("test02: ok");
    return 0;
}
