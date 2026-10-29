#include <stdio.h>

int sum(int a, int b) {
    return a + b;
}

int main(void) {
    // warn.c: неиспользуемая переменная d убрана, часто это след опечатки
    int a;
    int b;
    int c = -1;

    if (scanf("%d %d", &a, &b) != 2)
        return 1;
    // было if (a = b): присваивание b в a, условие истинно при b != 0
    if (a == b)
        c = sum(a, b);
    printf("%d\n", c);              // «3 3» → 6, «3 4» → -1

    // wextra.c: -1 в unsigned int — это 2^32 - 1
    int i = 1;
    unsigned int j = -1;

    // при сравнении int с unsigned int i тоже приводится к unsigned;
    // явное приведение показывает, что так и задумано
    if ((unsigned int)i < j)
        printf("%u > %d\n", j, i);  // 4294967295 > 1
    // было %d: те же биты печатались как знаковое -1

    // wpedantic.c: int x[0] — расширение GCC, стандарт C требует размер
    // больше нуля, а соответствие ISO C проверяет только -Wpedantic
    int x[1] = {0};

    printf("%zu\n", sizeof x);      // 4
    return 0;
}
