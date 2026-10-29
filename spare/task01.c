#include <stdio.h>

int main(void) {
    unsigned a = 0x8AE;                     // 100010101110
    // каждый бит совпадает сам с собой
    printf("%#x\n", a ^ a);                 // 0
    // XOR с нулём биты не меняет
    printf("%#x\n", a ^ 0);                 // 0x8ae
    // поэтому (a ^ b) ^ b = a ^ (b ^ b) = a: второй XOR отменяет первый

    unsigned x1 = 0x8AE;
    unsigned x2 = 0xD74;
    unsigned x3 = 0x5DA;
    unsigned x4 = x1 ^ x2 ^ x3;             // контрольная сумма

    // x1 потерян: x2 ^ x3 ^ x4 = x2 ^ x3 ^ x1 ^ x2 ^ x3 = x1
    // так RAID 5 восстанавливает отказавший диск по контрольной сумме
    unsigned restored = x2 ^ x3 ^ x4;
    printf("%#x\n", restored);              // 0x8ae
    return 0;
}
