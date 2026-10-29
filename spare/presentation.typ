// Сгенерировано tools/md2typ.py из presentation.md — правьте markdown, не этот файл.
#import "../themes/slides.typ": *

#show: deck.with(title: "Практика по C", subtitle: "Запасные задачи")

#slide([#"Задача 1. Восстановление через XOR"])[
#"Исключающее «или» "#raw("^")#" даёт "#raw("1")#" в тех битах, где операнды различаются, и "#raw("0")#", где совпадают."

- #strong[#"а)"]#" Чему равны "#raw("A ^ A")#" и "#raw("A ^ 0")#" для любого "#raw("A")#"? Как отсюда получить "#raw("A")#", зная "#raw("A ^ B")#" и "#raw("B")#"?"

- #strong[#"б)"]#" Три блока данных "#raw("x1")#", "#raw("x2")#", "#raw("x3")#" хранятся вместе с контрольной суммой "#raw("x4 = x1 ^ x2 ^ x3")#". Блок "#raw("x1")#" потерян. Восстановите его по "#raw("x2")#", "#raw("x3")#" и "#raw("x4")#". Где этот приём используется на практике?"
]

#slide([#"Задача 1. Ответ (а)"])[
#code(read("task01.c"), lang: "c", start: 4, end: 9)
]

#slide([#"Задача 1. Ответ (б)"])[
#code(read("task01.c"), lang: "c", start: 11, end: 19)
]

#slide([#"Задача 2. Оптимизации и "#raw("restrict")])[
#"Две функции копируют массив и отличаются только словом "#raw("restrict")#" у параметров:"

#code("void arr_copy(int *to, int *from, int n) {\n    for (int i = 0; i < n; ++i)\n        to[i] = from[i];\n}\n\nvoid arr_copy_restrict(int *restrict to, int *restrict from, int n) {\n    for (int i = 0; i < n; ++i)\n        to[i] = from[i];\n}", lang: "c")

- #strong[#"а)"]#" Соберите их с "#raw("gcc -O2 -c")#" и дизассемблируйте "#raw("objdump -dr")#". Во что превратилась каждая функция?"

- #strong[#"б)"]#" Почему результат разный, хотя тела одинаковые? Что обещает "#raw("restrict")#"?"
]

#slide([#"Задача 2. Ответ (а)"])[
#code("gcc -O2 -c task02.c -o task02.o\nobjdump -dr task02.o\n# <arr_copy>:\n#   mov    (%rsi,%rax,1),%ecx    цикл: по одному int за шаг\n#   mov    %ecx,(%rdi,%rax,1)\n# <arr_copy_restrict>:\n#   shl    $0x2,%rdx             n * sizeof(int): длина в байтах\n#   jmp    memcpy                цикла нет, его заменил вызов memcpy", lang: "bash")
]

#slide([#"Задача 2. Ответ (б)"])[
#code(read("task02.c"), lang: "c", start: 3, end: 15)
]
