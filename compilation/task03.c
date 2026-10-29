#include <stdio.h>

// без скобок: x + abs1(y) раскроется в x + y > 0 ? y : -y
#define abs1(x) x > 0 ? x : -x
// исходная версия: (-x) не защищает аргумент-выражение
#define abs2_old(x) ((x) > 0 ? (x) : (-x))
// скобки вокруг каждого вхождения параметра и всего выражения
#define abs2(x) ((x) > 0 ? (x) : -(x))

int main(void) {
    int x = 7;
    int y = -8;
    // + старше ?:, поэтому условие — x + y > 0, то есть -1 > 0: берём -y
    int a1 = x + abs1(y);
    // скобки сохраняют порядок: x + 8
    int a2 = x + abs2_old(y);
    printf("%d %d\n", a1, a2);                          // 8 15

    int a = 1;
    int b = 5;
    // abs2_old(a - b) → ((a - b) > 0 ? (a - b) : (-a - b))
    printf("%d %d\n", abs2_old(a - b), abs2(a - b));    // -6 4
    return 0;
}
