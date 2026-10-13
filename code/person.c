#include <stdio.h>

typedef struct {
    char name[32];
    int age;
} Person;

void greet(const Person *p) {
    printf("Привет, %s (%d)!\n", p->name, p->age);
}

int main(void) {
    Person p = {.name = "Мир", .age = 42};
    greet(&p);
    return 0;
}
