#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct list {
    int *data;          // элементы подряд в одном блоке
    size_t size;        // сколько занято
    size_t capacity;    // под сколько выделено
};

list *list_create(void) {
    list *l = malloc(sizeof *l);
    if (l == NULL) {
        return NULL;
    }
    l->data = NULL;
    l->size = 0;
    l->capacity = 0;
    return l;
}

void list_destroy(list *l) {
    if (l == NULL) {
        return;
    }
    free(l->data);
    free(l);
}

size_t list_size(const list *l) {
    return l->size;
}

int list_get(const list *l, size_t i) {
    assert(i < l->size);
    return l->data[i];
}

void list_set(list *l, size_t i, int value) {
    assert(i < l->size);
    l->data[i] = value;
}

// Гарантирует место ещё под один элемент
static bool reserve_one(list *l) {
    if (l->size < l->capacity) {
        return true;
    }
    size_t capacity = l->capacity == 0 ? 4 : 2 * l->capacity;
    int *data = realloc(l->data, capacity * sizeof *data);
    if (data == NULL) {
        return false;   // старый блок l->data цел
    }
    l->data = data;
    l->capacity = capacity;
    return true;
}

bool list_push_back(list *l, int value) {
    if (!reserve_one(l)) {
        return false;
    }
    l->data[l->size] = value;
    l->size++;
    return true;
}

bool list_insert(list *l, size_t i, int value) {
    assert(i <= l->size);
    if (!reserve_one(l)) {
        return false;
    }
    // Хвост [i, size) сдвигается на одну позицию вправо
    memmove(&l->data[i + 1], &l->data[i], (l->size - i) * sizeof *l->data);
    l->data[i] = value;
    l->size++;
    return true;
}

void list_remove(list *l, size_t i) {
    assert(i < l->size);
    // Хвост (i, size) сдвигается на одну позицию влево
    memmove(&l->data[i], &l->data[i + 1], (l->size - i - 1) * sizeof *l->data);
    l->size--;
}
