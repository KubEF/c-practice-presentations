---
title: "Практика по C"
sub_title: "Запасные задачи"
theme:
  path: ../themes/latte-projector.yaml
  override:
    code:
      line_numbers: true
---

Задача 1. Восстановление через XOR
==================================

Исключающее «или» `^` даёт `1` в тех битах, где операнды различаются, и `0`, где совпадают.

* **а)** Чему равны `A ^ A` и `A ^ 0` для любого `A`? Как отсюда получить `A`, зная `A ^ B` и `B`?
* **б)** Три блока данных `x1`, `x2`, `x3` хранятся вместе с контрольной суммой `x4 = x1 ^ x2 ^ x3`. Блок `x1` потерян. Восстановите его по `x2`, `x3` и `x4`. Где этот приём используется на практике?

<!-- end_slide -->

Задача 1. Ответ (а)
===================

```file
path: task01.c
language: c
start_line: 4
end_line: 9
```

<!-- end_slide -->

Задача 1. Ответ (б)
===================

```file
path: task01.c
language: c
start_line: 11
end_line: 19
```

<!-- end_slide -->

Задача 2. Оптимизации и `restrict`
==================================

Две функции копируют массив и отличаются только словом `restrict` у параметров:

```c
void arr_copy(int *to, int *from, int n) {
    for (int i = 0; i < n; ++i)
        to[i] = from[i];
}

void arr_copy_restrict(int *restrict to, int *restrict from, int n) {
    for (int i = 0; i < n; ++i)
        to[i] = from[i];
}
```

* **а)** Соберите их с `gcc -O2 -c` и дизассемблируйте `objdump -dr`. Во что превратилась каждая функция?
* **б)** Почему результат разный, хотя тела одинаковые? Что обещает `restrict`?

<!-- end_slide -->

Задача 2. Ответ (а)
===================

```bash
gcc -O2 -c task02.c -o task02.o
objdump -dr task02.o
# <arr_copy>:
#   mov    (%rsi,%rax,1),%ecx    цикл: по одному int за шаг
#   mov    %ecx,(%rdi,%rax,1)
# <arr_copy_restrict>:
#   shl    $0x2,%rdx             n * sizeof(int): длина в байтах
#   jmp    memcpy                цикла нет, его заменил вызов memcpy
```

<!-- end_slide -->

Задача 2. Ответ (б)
===================

```file
path: task02.c
language: c
start_line: 3
end_line: 15
```
