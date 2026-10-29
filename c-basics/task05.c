#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = 0;

    if (argc > 1) {
        n = (int)strtol(argv[1], NULL, 10); // число пришло аргументом
    } else {
        printf("Введите число: ");
        fflush(stdout);
        if (scanf("%d", &n) != 1) { // иначе читаем из stdin
            printf("нет ввода — беру 0\n");
            n = 0;
        }
    }

    printf("n = %d\n", n);
    return 0;
}
