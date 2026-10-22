#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node *next;
} node_t;

void push_front(node_t **head, int value) {
    node_t *n = malloc(sizeof *n);
    if (n == NULL)
        return;
    n->value = value;
    n->next = *head;
    *head = n;
}

const node_t *find(const node_t *head, int value) {
    for (const node_t *cur = head; cur != NULL; cur = cur->next)
        if (cur->value == value)
            return cur;
    return NULL;
}

void destroy(node_t *head) {
    while (head != NULL) {
        node_t *next = head->next;
        free(head);
        head = next;
    }
}

int main(void) {
    node_t *head = NULL;

    push_front(&head, 10);
    push_front(&head, 20);
    push_front(&head, 30);

    for (const node_t *cur = head; cur != NULL; cur = cur->next)
        printf("%d ", cur->value);          // 30 20 10
    printf("\n");

    const node_t *found = find(head, 20);
    printf("%d\n", found ? found->value : -1);   // 20

    destroy(head);
    return 0;
}
