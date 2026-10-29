#include <stdio.h>

size_t my_strlen(const char *s) {
    const char *p = s;
    while (*p)
        p++;
    return p - s;
}

void my_strcpy(char *dst, const char *src) {
    while ((*dst++ = *src++))
        ;
}

// void my_strcpy(char *dst, const char *src) {
//     while (*src) {
//         *dst = *src;
//         *dst++;
//         *src++;
//     }
//     *dst = '\0';
// }

int main(void) {
    char buf[16];
    my_strcpy(buf, "Hello");
    printf("%zu %s\n", my_strlen(buf), buf); // 5 Hello
    return 0;
}
