#include <stdbool.h>
#include <stdio.h>

int func1(void) {
    printf("func1 ");
    return 1;
}

int func2(void) {
    printf("func2 ");
    return 1;
}

int main(void) {
    // &: каждая пара битов отдельно, 01 & 10 = 00
    // &&: операнды целиком, оба ненулевые, значит истина
    printf("%d %d\n", 1 & 2, 1 && 2);                // 0 1

    // && останавливается на первом ложном операнде: функции не вызываются
    printf("[%d]\n", false && func1() && func2());   // [0]

    // у & нет вычислений по Маккарти: вычисляются все операнды
    // порядок вызова func1 и func2 не определён
    printf("[%d]\n", 0 & func1() & func2());         // func1 func2 [0]

    // false — макрос, равный 0, выражение то же самое
    printf("[%d]\n", false & func1() & func2());     // func1 func2 [0]
    return 0;
}
