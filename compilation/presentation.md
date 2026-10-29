---
title: "Практика по C"
sub_title: "Компиляция: от исходника до программы"
theme:
  path: ../themes/latte-projector.yaml
  override:
    code:
      line_numbers: true
---

Задача 1. Константы и секции
============================

Программа `task01.c` печатает `Hello world! Today is September 25.`: строку `"September"` и число `25` она подставляет в `printf` через `%s` и `%d`. Ключ `gcc --save-temps` сохраняет промежуточные файлы сборки, `objdump` показывает содержимое объектного файла, `nm` — его таблицу символов.

* **а)** Соберите программу с `--save-temps`. Какие файлы появятся рядом с исходником и какой стадии сборки соответствует каждый?
* **б)** Найдите в объектном файле строку `"September"` и число `25`. Где лежит каждая из этих констант?
* **в)** Какими буквами `nm` пометит `main` и `printf`? Что они означают?

<!-- end_slide -->

Задача 1. Ответ (а)
===================

```bash
gcc --save-temps task01.c -o task01
# task01.i  после препроцессора: подставлены #include и макросы
# task01.s  после компиляции: текст на ассемблере
# task01.o  после ассемблера: машинный код, адреса ещё не подставлены
# task01    после линковки: готовая программа
```

<!-- end_slide -->

Задача 1. Ответ (б)
===================

```bash
objdump -d task01.o
#   lea    0x0(%rip),%rcx    адрес строки: сами строки лежат не в коде
#   mov    $0x19,%edx        25 = 0x19 — непосредственный операнд команды
objdump -s -j .rodata task01.o
#   September.Hello world! Today is %s %d...
# строки — в секции констант .rodata;
# const-переменная с числом тоже ушла бы в .rodata
```

<!-- end_slide -->

Задача 1. Ответ (в)
===================

```bash
nm task01.o
# 0000000000000000 T main      T: определён в .text и виден снаружи
#                  U printf    U: не определён здесь, адрес подставит линкер
# линковка по умолчанию динамическая: printf возьмётся из libc при запуске
```

<!-- end_slide -->

Задача 2. Программа из нескольких файлов
========================================

Программа `prog` печатает расстояние между двумя числами: `./prog 10 3` → `7`. Она разбита на три файла: `main.c` читает числа и вызывает `distance(a, b)`, `distance.c` определяет её, а `distance.h` объявляет. В исходной версии `main.c` не подключает `distance.h`, а в самом заголовке определена своя функция модуля:

```c
static inline int abs(int x) {
    if (x < 0)
        return x;
    return x;
}
```

* **а)** Что ответит `gcc -c main.c`? Почему компилятор не нашёл `distance`, хотя она есть в `distance.c`?
* **б)** Как `abs` будет помечен в `nm distance.o` и почему? Что не так с этой `abs`?
* **в)** Что напечатают `./prog 10 3` и `./prog` без аргументов? Исправьте `main.c`.
* **г)** `distance.c` и `distance.h` перенесли в подкаталог `dist/`, а `main.c` по-прежнему подключает `"distance.h"`. Что ответит `gcc -c main.c`? Как исправить это двумя способами — с правкой кода и без неё? Нужен ли `-I` для сборки `dist/distance.c`?

<!-- end_slide -->

Задача 2. Ответ (а)
===================

```bash
cd examples/Ex3
gcc -c main.c -o main.o
# error: implicit declaration of function 'distance'
# компилятор видит только свой файл и подключённые в него заголовки;
# до gcc 14 это было предупреждение, и объявление додумывалось само
```

```file
path: task02/main.c
language: c
start_line: 1
end_line: 2
```

<!-- end_slide -->

Задача 2. Ответ (б)
===================

```bash
gcc -c distance.c -o distance.o
nm distance.o
# 0000000000000000 t abs        t: локальный — static, виден только здесь
# 0000000000000017 T distance   T: глобальный, его найдёт линкер
```

```file
path: task02/distance.c
language: c
```

<!-- end_slide -->

Задача 2. Ответ (в)
===================

```bash
gcc -fpermissive -c main.c -o main.o    # собрать, как до gcc 14
gcc main.o distance.o -o prog
./prog 10 3     # -7: своя abs не меняет знак
./prog          # Segmentation fault
```

```file
path: task02/main.c
language: c
start_line: 9
end_line: 18
```

<!-- end_slide -->

Задача 2. Ответ (г)
===================

```bash
cd examples/Ex3_subdir
gcc -c main.c -o main.o
# fatal error: distance.h: No such file or directory
# "..." ищется в каталоге самого файла, затем в -I и системных;
# <...> — только в каталогах -I и системных
gcc -I dist -c main.c -o main.o         # способ 1: каталог поиска заголовков
#                                         способ 2: #include "dist/distance.h"
gcc -c dist/distance.c -o distance.o    # -I не нужен: distance.h лежит рядом
gcc main.o distance.o -o prog
./prog 3 10                             # 7
```

<!-- end_slide -->

Задача 3. Макросы и скобки
==========================

Два макроса должны вычислять модуль числа:

```c
#define abs1(x) x > 0 ? x : -x
#define abs2(x) ((x) > 0 ? (x) : (-x))

int x = 7;
int y = -8;
int a1 = x + abs1(y);
int a2 = x + abs2(y);
```

* **а)** Во что препроцессор раскроет `a1` и `a2`? Какой командой это увидеть?
* **б)** Чему равны `a1` и `a2`? Почему они разные?
* **в)** Всё ли в порядке с `abs2`? Подставьте в него `a - b`.

<!-- end_slide -->

Задача 3. Ответ (а)
===================

```bash
cd examples/Ex4
gcc -E main.c | grep 'int a[12] ='     # -E: остановиться после препроцессора
#  int a1 = x + y > 0 ? y : -y;
#  int a2 = x + ((y) > 0 ? (y) : (-y));
```

<!-- end_slide -->

Задача 3. Ответ (б)
===================

```file
path: task03.c
language: c
start_line: 11
end_line: 17
```

<!-- end_slide -->

Задача 3. Ответ (в)
===================

```file
path: task03.c
language: c
start_line: 5
end_line: 8
```

```file
path: task03.c
language: c
start_line: 19
end_line: 22
```

<!-- end_slide -->

Задача 4. Предупреждения компилятора
====================================

В трёх программах есть ошибки, которые `gcc` находит только с нужными ключами. `-Wall` включает основные предупреждения, `-Wextra` — дополнительные, `-Wpedantic` проверяет соответствие стандарту ISO C, а `-Werror` превращает предупреждения в ошибки.

```c
// warn.c: сложить два числа, если они равны
int c = -1, d = 0;
scanf("%d %d", &a, &b);
if (a = b)
    c = sum(a, b);

// wextra.c
int i = 1;
unsigned int j = -1;
if (i < j)
    printf("%d is bigger then %d\n", j, i);

// wpedantic.c
int a[0];
```

* **а)** Что скажет `gcc -Wall warn.c`? Что напечатает программа на вводе `3 4`?
* **б)** Что скажут `gcc -Wall wextra.c` и `gcc -Wall -Wextra wextra.c`? Что напечатает программа и почему условие истинно, хотя `-1 < 1` ложно?
* **в)** Что скажут `-Wall`, `-Wextra` и `-Wpedantic` для `wpedantic.c`? Почему код компилируется, если массив нулевого размера запрещён?
* **г)** Что изменит ключ `-Werror`?

<!-- end_slide -->

Задача 4. Ответ (а)
===================

```bash
cd examples/Ex6
gcc -Wall warn.c -o warn
# warning: suggest parentheses around assignment used as truth value
# warning: unused variable 'd'
echo "3 4" | ./warn     # 8: a = b кладёт 4 в a, условие 4 истинно
```

```file
path: task04.c
language: c
start_line: 8
end_line: 18
```

<!-- end_slide -->

Задача 4. Ответ (б)
===================

```bash
gcc -Wall wextra.c -o wextra              # без предупреждений
gcc -Wall -Wextra wextra.c -o wextra
# warning: comparison of integer expressions of different signedness:
#   'int' and 'unsigned int' [-Wsign-compare]
./wextra    # -1 is bigger then 1
```

```file
path: task04.c
language: c
start_line: 20
end_line: 28
```

<!-- end_slide -->

Задача 4. Ответ (в)
===================

```bash
gcc -Wall wpedantic.c         # warning: unused variable 'a'
gcc -Wextra wpedantic.c       # без предупреждений
gcc -Wpedantic wpedantic.c    # warning: ISO C forbids zero-size array 'a'
```

```file
path: task04.c
language: c
start_line: 30
end_line: 34
```

<!-- end_slide -->

Задача 4. Ответ (г)
===================

```bash
gcc -Wall -Werror warn.c -o warn
# error: suggest parentheses around assignment used as truth value
# error: unused variable 'd'
# cc1: all warnings being treated as errors
# предупреждения стали ошибками, программа не собирается
```
