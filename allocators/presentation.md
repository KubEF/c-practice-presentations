---
title: "Практика по C"
sub_title: "Аллокаторы, анализаторы, системы сборки"
theme:
  path: ../themes/latte-projector.yaml
  override:
    code:
      line_numbers: true
---

Задача 1. Лишний элемент
========================

**Файл:** `practice/t2.c` — не меняйте его до пункта **г**. Все команды выполняйте в папке `practice/`.

Программа печатает квадраты чисел 0–4 и вроде бы работает правильно. Но в ней есть ошибка.

```file
path: practice/t2.c
language: c
```

* **а)** Соберите и запустите как обычно: `gcc -g t2.c -o t2 && ./t2`. Видна ли ошибка?
* **б)** Соберите с AddressSanitizer и запустите: `gcc -g -fsanitize=address t2.c -o t2 && ./t2`
* **в)** Найдите в отчёте ASan: название ошибки; номер строки; чтение это или запись и сколько байт; размер выделенного блока
* **г)** Исправьте ошибку в `t2.c` и повторите пункт **б**

**Готово, если** с ASan программа печатает `0 1 4 9 16` и ничего больше.

<!-- end_slide -->

Задача 1. Ответ
===============

```bash
gcc -g t2.c -o t2 && ./t2          # 0 1 4 9 16 — ошибки не видно
gcc -g -fsanitize=address t2.c -o t2 && ./t2
# ERROR: AddressSanitizer: heap-buffer-overflow on address ...
# WRITE of size 4 at ... thread T0
#     #0 ... in main t2.c:8
# ... is located 0 bytes after 20-byte region
# запись squares[5] сразу за концом блока из 5 int = 20 байт
```

```file
path: solution/t2.c
language: c
start_line: 7
end_line: 8
```

«Работает» не значит «правильно»: домашние задания стоит прогонять с ASan.

<!-- end_slide -->

Задачи 2–5. Файлы мини-проекта
==============================

Дальше работаем в папке `practice/bump/`. Все команды выполняйте в ней.

| Файл | Что в нём | Что с ним делать |
| --- | --- | --- |
| `bump.h` | объявления `bump_malloc`, `bump_free`, `bump_used` | не менять |
| `bump.c` | заготовка: куча `heap` и `top` | написать в задаче 2, исправить в задаче 5 |
| `main.c` | программа: имя, три оценки и среднее | не менять |
| `Makefile` | заготовка | дописать в задаче 3 |
| `CMakeLists.txt` | заготовка | дописать в задаче 4 |

Из кода вы пишете только аллокатор в `bump.c`. Остальное — готовая программа и заготовки сборки.

<!-- end_slide -->

Задача 2. Свой аллокатор
========================

**Файл:** `practice/bump/bump.c` — напишите три функции из `bump.h`. `bump.h` и `main.c` не меняем.

```file
path: practice/bump/bump.h
language: c
```

Простейший аллокатор из лекции — «массив с указателем». В заготовке `bump.c` уже есть куча `heap` из 1024 байт и `top` — номер первого свободного байта.

* **а)** `bump_malloc(n)`: если `n == 0` или `n` байт не помещаются в кучу после `top`, вернуть `NULL`; иначе вернуть адрес `heap + top` и сдвинуть `top` на `n`
* **б)** `bump_free(p)` ничего не делает, `bump_used()` возвращает `top`
* **в)** Соберите и запустите: `gcc -Wall -Wextra main.c bump.c -o app && ./app`

**Готово, если** программа собирается без предупреждений и печатает `Anna: 5 4 5, avg 4.67` и `used 25 bytes`.

<!-- end_slide -->

Задача 2. Ответ
===============

```file
path: solution/bump_naive.c
language: c
start_line: 8
end_line: 22
```

```bash
gcc -Wall -Wextra main.c bump.c -o app && ./app
# Anna: 5 4 5, avg 4.67
# name=0x...030 marks=0x...035 avg=0x...041
# used 25 bytes        5 + 12 + 8 = 25
```

<!-- end_slide -->

Задача 3. Makefile
==================

**Файл:** `practice/bump/Makefile` — допишите правило для `app` и цель `clean`. Остальные файлы не меняем.

```file
path: practice/bump/Makefile
language: makefile
start_line: 4
end_line: 12
```

Соберите и запустите:

```bash
make && ./app
```

**Готово, если** `./app` печатает то же, что в задаче 2, а `make clean` удаляет `app`.

<!-- end_slide -->

Задача 3. Ответ
===============

```file
path: solution/Makefile
language: makefile
start_line: 7
end_line: 11
```

```bash
make && ./app     # gcc -Wall -Wextra main.c bump.c -o app
make clean        # rm -f app
```

<!-- end_slide -->

Задача 4. CMake
===============

**Файл:** `practice/bump/CMakeLists.txt` — допишите цель `app`. Дальше собираем только через CMake.

```file
path: practice/bump/CMakeLists.txt
language: cmake
start_line: 4
end_line: 9
```

Соберите и запустите:

```bash
cmake -B build          # один раз: настроить сборку в папке build/
cmake --build build     # собрать; повторяйте после каждого изменения
./build/app
```

**Готово, если** `./build/app` печатает то же, что в задаче 2.

<!-- end_slide -->

Задача 4. Ответ
===============

```file
path: solution/CMakeLists.txt
language: cmake
start_line: 6
end_line: 6
```

```bash
cmake -B build          # -- Build files have been written to: .../build
cmake --build build     # [100%] Built target app
```

<!-- end_slide -->

Задача 5. Чиним аллокатор
=========================

**Файл:** `practice/bump/bump.c` — исправьте функцию `bump_malloc`. Остальные файлы не меняем.

Посмотрите на адреса, которые печатает `./build/app`: `name=0x...030 marks=0x...035 avg=0x...041`. `int` должен лежать по адресу, кратному 4, `double` — кратному 8, а `marks` и `avg` лежат как попало. Настоящий `malloc` выравнивает каждый блок — сделаем так же: `bump_malloc` должен всегда возвращать адрес, кратный 8.

Так выглядит `bump_malloc` из задачи 2:

```file
path: solution/bump_naive.c
language: c
start_line: 8
end_line: 14
```

* **а)** На бумаге нарисуйте кучу как ленту байтов 0, 1, 2, … и отметьте `name` (5 байт), `marks` (12 байт) и `avg` (8 байт): сначала как сейчас, потом — если каждый блок начинается с адреса, кратного 8. Сколько байт пропадает зря?
* **б)** В начале `bump_malloc` округлите `top` вверх до числа, кратного 8. Проверка, что блок помещается в кучу, должна идти уже после округления
* **в)** Соберите и запустите:

```bash
cmake --build build && ./build/app
```

**Готово, если** все три адреса оканчиваются на 0 или 8, а программа печатает `used 32 bytes` вместо 25.

<!-- end_slide -->

Задача 5. Ответ (а)
===================

```text
байт    0       8       16      24      32
было    nnnnnmmmmmmmmmmmmaaaaaaaa              used 25
стало   nnnnn...mmmmmmmmmmmm....aaaaaaaa       used 32
        name    marks           avg
```

`name` 0–4, пусто 5–7, `marks` 8–19, пусто 20–23, `avg` 24–31: зря пропадает 7 байт.

Это внутренняя фрагментация: попросили 25 байт, заняли 32.

<!-- end_slide -->

Задача 5. Ответ (б, в)
======================

```file
path: solution/bump.c
language: c
start_line: 8
end_line: 15
```

`(top + 7) & ~(size_t)7` — то же, что `(top + 7) / 8 * 8`: так пишут в настоящих аллокаторах.

```bash
cmake --build build && ./build/app
# name=0x...030 marks=0x...038 avg=0x...048
# used 32 bytes
```

<!-- end_slide -->

Задача 6. Арена
===============

**Файл:** `practice/arena/arena.c` — напишите четыре функции из `arena.h`. `arena.h`, `main.c` и `CMakeLists.txt` готовы, их не меняем. Команды выполняйте в папке `practice/arena/`.

<!-- column_layout: [3, 2] -->

<!-- column: 0 -->

```file
path: practice/arena/arena.h
language: c
start_line: 6
end_line: 23
```

<!-- column: 1 -->

Арена из лекции: кусок памяти от `malloc`, выделение сдвигает `ptr`, освобождается вся арена сразу. `main.c` — программа из задачи 2, но на арене.

* **а)** `arena_create`: два `malloc` — под дескриптор и под `size` байт; если второй не удался, освободите первый
* **б)** `arena_alloc`: округлите `n` вверх до кратного 8; не помещается до `end` — `NULL`; иначе верните `ptr` и сдвиньте его
* **в)** `arena_used` — сколько байт от `start` до `ptr`; `arena_destroy` — освободить память, потом дескриптор

<!-- reset_layout -->

Соберите и запустите: `cmake -B build && cmake --build build && ./build/app`

**Готово, если** программа печатает `used 32 bytes`.

<!-- end_slide -->

Задача 6. Ответ (а)
===================

```file
path: solution/arena.c
language: c
start_line: 4
end_line: 16
```

В лекции результаты `malloc` не проверялись — в рабочем коде их проверять нужно.

<!-- end_slide -->

Задача 6. Ответ (б, в)
======================

```file
path: solution/arena.c
language: c
start_line: 18
end_line: 34
```

```bash
cmake -B build && cmake --build build && ./build/app
# Anna: 5 4 5, avg 4.67
# used 32 bytes          выравнивание уже в arena_alloc
```
