// Задача 6: list_create, list_size, list_destroy (связный список)
#include "../list.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    list *l = list_create();
    assert(l != NULL);
    assert(list_size(l) == 0);
    list_destroy(l);                        // пустой
    list_destroy(NULL);

    l = list_create();
    assert(l != NULL);
    for (int v = 1; v <= 3; v++) {
        assert(list_push_back(l, v));
    }
    assert(list_size(l) == 3);
    list_destroy(l);                        // с узлами: утечки поймает ASan
    puts("test06: ok");
    return 0;
}
