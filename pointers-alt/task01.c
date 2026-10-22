#include <stdio.h>
#include <string.h>

typedef struct {
    int x;
    float y;
    char s[16];
} item_t;

void set_by_value(item_t m) {
    m.x = 1;
    m.y = 1.0f;
    strcpy(m.s, "one");
}

void set_by_pointer(item_t *m) {
    m->x = 2;
    m->y = 2.0f;
    strcpy(m->s, "two");
}

int main(void) {
    item_t it = {0, 0.0f, ""};

    set_by_value(it);
    printf("[%d %.0f %s]\n", it.x, it.y, it.s);   // [0 0 ]

    set_by_pointer(&it);
    printf("[%d %.0f %s]\n", it.x, it.y, it.s);   // [2 2 two]
    return 0;
}
