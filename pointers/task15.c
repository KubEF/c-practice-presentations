#include <stdio.h>
#include <stddef.h>

struct A       { char a; int b; };
struct struct1 { char *p; char c; };
struct struct2 { short s; char c; char pad[1]; };
struct struct3 { char a; short b; char c; int d; };

int main(void) {
    printf("A       %zu  b@%zu\n", sizeof(struct A),
           offsetof(struct A, b));
    printf("struct1 %zu  c@%zu\n", sizeof(struct struct1),
           offsetof(struct struct1, c));
    printf("struct2 %zu\n", sizeof(struct struct2));
    printf("struct3 %zu  b@%zu c@%zu d@%zu\n", sizeof(struct struct3),
           offsetof(struct struct3, b), offsetof(struct struct3, c),
           offsetof(struct struct3, d));
    return 0;
}
