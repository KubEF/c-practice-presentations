#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

union fbits { float f; uint32_t i; };

uint32_t bits(float f) {
    union fbits u = {.f = f};
    return u.i;
}

int main(void) {
    float zero = 0.0f;
    float nzero = -0.0f;

    // ±0: порядок и мантисса — нули, различается только знак
    printf("%#x %#x\n", bits(zero), bits(nzero));      // 0 0x80000000
    printf("%g %g\n", nzero * nzero, nzero + nzero);   // 0 -0
    printf("%d\n", nzero == zero);                     // 1

    // ±inf: порядок — все единицы, мантисса нулевая
    float inf = 1.0f / zero;                           // деление на ноль
    printf("%#x %g\n", bits(inf), -1.0f / zero);       // 0x7f800000 -inf
    printf("%g\n", FLT_MAX * 2);                       // inf: переполнение

    // NaN: порядок — все единицы, мантисса ненулевая
    float nan = zero / zero;
    printf("%d %d\n", isnan(inf - inf), isnan(zero * inf));   // 1 1
    printf("%d\n", isnan(sqrtf(-1.0f)));               // 1
    // NaN не равен ничему, даже себе, и «заражает» вычисления
    printf("%d %d\n", nan == nan, isnan(nan + 1));     // 0 1
    return 0;
}
