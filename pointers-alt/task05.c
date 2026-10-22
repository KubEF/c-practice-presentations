#include <stdio.h>

void print_array(const int *arr, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(void) {
    int a[3] = {1, 2, 3};
    const int b[3] = {4, 5, 6};

    int *p1 = a;         // ок: данные без const
    const int *p2 = a;   // ок: const можно добавить
    const int *p3 = b;   // ок: const на const
    // int *p4 = b;      // ошибка: const нельзя отбросить
    // p2[0] = 9;        // ошибка: через p2 менять нельзя

    a[0] = 9;
    // b[0] = 9;         // ошибка: b константный
    print_array(p1, 3);   // 9 2 3
    print_array(p2, 3);   // 9 2 3
    print_array(p3, 3);   // 4 5 6
    return 0;
}
