#include <stdio.h>

// сдвигаем i-й бит в младший разряд, остальные отрезаем маской 1
unsigned get_bit(unsigned a, int i) {
    return (a >> i) & 1u;
}

// | с маской 0..010..0 ставит i-й бит в 1, остальные не трогает
unsigned set_bit(unsigned a, int i) {
    return a | (1u << i);
}

// & с маской 1..101..1 обнуляет i-й бит, остальные не трогает
unsigned clear_bit(unsigned a, int i) {
    return a & ~(1u << i);
}

// сдвиг вправо на i, затем маска из j - i + 1 единиц
// при i = 0, j = 31 получится 1u << 32 — UB
unsigned get_bits(unsigned a, int i, int j) {
    return (a >> i) & ((1u << (j - i + 1)) - 1);
}

// сдвиг влево отрезает биты старше j, сдвиг вправо — младше i
unsigned get_bits2(unsigned a, int i, int j) {
    return (a << (31 - j)) >> (31 - j + i);
}

int main(void) {
    unsigned a = 0x8AE;                                 // 100010101110

    printf("%u %u\n", get_bit(a, 1), get_bit(a, 4));    // 1 0
    printf("%#x\n", set_bit(a, 4));                     // 0x8be = 100010111110
    printf("%#x\n", clear_bit(a, 1));                   // 0x8ac = 100010101100

    printf("%#x\n", get_bits(a, 2, 7));                 // 0x2b = 101011
    printf("%#x\n", get_bits2(a, 2, 7));                // 0x2b = 101011

    unsigned ip = 0xC0A80101;                           // 192.168.1.1
    // маска /N: N единиц, затем нули; /24 = 255.255.255.0
    unsigned mask = ~0u << (32 - 24);                   // 0xffffff00
    // адрес сети 192.168.1.0: биты адреса узла обнуляются
    printf("%#x\n", ip & mask);                         // 0xc0a80100
    return 0;
}
