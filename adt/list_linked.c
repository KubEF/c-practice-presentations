#include "list.h"
#include <assert.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;  // NULL у последнего узла
};

struct list {
    struct node *head;  // первый узел или NULL
    struct node *tail;  // последний узел или NULL
    size_t size;
};

list *list_create(void) {
    list *l = malloc(sizeof *l);
    if (l == NULL) {
        return NULL;
    }
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}

void list_destroy(list *l) {
    if (l == NULL) {
        return;
    }
    struct node *n = l->head;
    while (n != NULL) {
        struct node *next = n->next;    // после free(n) читать n->next нельзя
        free(n);
        n = next;
    }
    free(l);
}

size_t list_size(const list *l) {
    return l->size;
}

// Узел с индексом i: шагаем от головы, O(i)
static struct node *node_at(const list *l, size_t i) {
    struct node *n = l->head;
    for (size_t k = 0; k < i; k++) {
        n = n->next;
    }
    return n;
}

int list_get(const list *l, size_t i) {
    assert(i < l->size);
    return node_at(l, i)->value;
}

void list_set(list *l, size_t i, int value) {
    assert(i < l->size);
    node_at(l, i)->value = value;
}

static struct node *node_new(int value, struct node *next) {
    struct node *n = malloc(sizeof *n);
    if (n == NULL) {
        return NULL;
    }
    n->value = value;
    n->next = next;
    return n;
}

bool list_push_back(list *l, int value) {
    struct node *n = node_new(value, NULL);
    if (n == NULL) {
        return false;
    }
    if (l->tail == NULL) {
        l->head = n;
    } else {
        l->tail->next = n;
    }
    l->tail = n;
    l->size++;
    return true;
}

bool list_insert(list *l, size_t i, int value) {
    assert(i <= l->size);
    if (i == l->size) {
        return list_push_back(l, value);    // заодно обновит tail
    }
    // link — указатель, который должен указать на новый узел
    struct node **link = i == 0 ? &l->head : &node_at(l, i - 1)->next;
    struct node *n = node_new(value, *link);
    if (n == NULL) {
        return false;
    }
    *link = n;
    l->size++;
    return true;
}

void list_remove(list *l, size_t i) {
    assert(i < l->size);
    struct node *prev = i == 0 ? NULL : node_at(l, i - 1);
    struct node **link = prev == NULL ? &l->head : &prev->next;
    struct node *victim = *link;
    *link = victim->next;
    if (victim == l->tail) {
        l->tail = prev;
    }
    free(victim);
    l->size--;
}
