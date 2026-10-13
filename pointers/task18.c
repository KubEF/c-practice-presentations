#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1)
struct PackedHeader { uint8_t type; uint16_t length; uint32_t id; };
#pragma pack(pop)

struct NormalHeader { uint8_t type; uint16_t length; uint32_t id; };

int main(void) {
    size_t packed = sizeof(struct PackedHeader);
    size_t normal = sizeof(struct NormalHeader);
    printf("%zu %zu\n", packed, normal);   // 7 8
    return 0;
}
