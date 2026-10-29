// Сгенерировано tools/md2typ.py из presentation.md — правьте markdown, не этот файл.
#import "../themes/slides.typ": *

#show: deck.with(title: "Практика по C", subtitle: "Компиляция: от исходника до программы")

#slide([#"Задача 1. Константы и секции"])[
#"Программа "#raw("task01.c")#" печатает "#raw("Hello world! Today is September 25.")#": строку "#raw("\"September\"")#" и число "#raw("25")#" она подставляет в "#raw("printf")#" через "#raw("%s")#" и "#raw("%d")#". Ключ "#raw("gcc --save-temps")#" сохраняет промежуточные файлы сборки, "#raw("objdump")#" показывает содержимое объектного файла, "#raw("nm")#" — его таблицу символов."

- #strong[#"а)"]#" Соберите программу с "#raw("--save-temps")#". Какие файлы появятся рядом с исходником и какой стадии сборки соответствует каждый?"

- #strong[#"б)"]#" Найдите в объектном файле строку "#raw("\"September\"")#" и число "#raw("25")#". Где лежит каждая из этих констант?"

- #strong[#"в)"]#" Какими буквами "#raw("nm")#" пометит "#raw("main")#" и "#raw("printf")#"? Что они означают?"
]

#slide([#"Задача 1. Ответ (а)"])[
#code("gcc --save-temps task01.c -o task01\n# task01.i  после препроцессора: подставлены #include и макросы\n# task01.s  после компиляции: текст на ассемблере\n# task01.o  после ассемблера: машинный код, адреса ещё не подставлены\n# task01    после линковки: готовая программа", lang: "bash")
]

#slide([#"Задача 1. Ответ (б)"])[
#code("objdump -d task01.o\n#   lea    0x0(%rip),%rcx    адрес строки: сами строки лежат не в коде\n#   mov    $0x19,%edx        25 = 0x19 — непосредственный операнд команды\nobjdump -s -j .rodata task01.o\n#   September.Hello world! Today is %s %d...\n# строки — в секции констант .rodata;\n# const-переменная с числом тоже ушла бы в .rodata", lang: "bash")
]

#slide([#"Задача 1. Ответ (в)"])[
#code("nm task01.o\n# 0000000000000000 T main      T: определён в .text и виден снаружи\n#                  U printf    U: не определён здесь, адрес подставит линкер\n# линковка по умолчанию динамическая: printf возьмётся из libc при запуске", lang: "bash")
]

#slide([#"Задача 2. Программа из нескольких файлов"])[
#"Программа "#raw("prog")#" печатает расстояние между двумя числами: "#raw("./prog 10 3")#" → "#raw("7")#". Она разбита на три файла: "#raw("main.c")#" читает числа и вызывает "#raw("distance(a, b)")#", "#raw("distance.c")#" определяет её, а "#raw("distance.h")#" объявляет. В исходной версии "#raw("main.c")#" не подключает "#raw("distance.h")#", а в самом заголовке определена своя функция модуля:"

#code("static inline int abs(int x) {\n    if (x < 0)\n        return x;\n    return x;\n}", lang: "c")

- #strong[#"а)"]#" Что ответит "#raw("gcc -c main.c")#"? Почему компилятор не нашёл "#raw("distance")#", хотя она есть в "#raw("distance.c")#"?"

- #strong[#"б)"]#" Как "#raw("abs")#" будет помечен в "#raw("nm distance.o")#" и почему? Что не так с этой "#raw("abs")#"?"

- #strong[#"в)"]#" Что напечатают "#raw("./prog 10 3")#" и "#raw("./prog")#" без аргументов? Исправьте "#raw("main.c")#"."

- #strong[#"г)"]#" "#raw("distance.c")#" и "#raw("distance.h")#" перенесли в подкаталог "#raw("dist/")#", а "#raw("main.c")#" по-прежнему подключает "#raw("\"distance.h\"")#". Что ответит "#raw("gcc -c main.c")#"? Как исправить это двумя способами — с правкой кода и без неё? Нужен ли "#raw("-I")#" для сборки "#raw("dist/distance.c")#"?"
]

#slide([#"Задача 2. Ответ (а)"])[
#code("cd examples/Ex3\ngcc -c main.c -o main.o\n# error: implicit declaration of function 'distance'\n# компилятор видит только свой файл и подключённые в него заголовки;\n# до gcc 14 это было предупреждение, и объявление додумывалось само", lang: "bash")

#code(read("task02/main.c"), lang: "c", start: 1, end: 2)
]

#slide([#"Задача 2. Ответ (б)"])[
#code("gcc -c distance.c -o distance.o\nnm distance.o\n# 0000000000000000 t abs        t: локальный — static, виден только здесь\n# 0000000000000017 T distance   T: глобальный, его найдёт линкер", lang: "bash")

#code(read("task02/distance.c"), lang: "c")
]

#slide([#"Задача 2. Ответ (в)"])[
#code("gcc -fpermissive -c main.c -o main.o    # собрать, как до gcc 14\ngcc main.o distance.o -o prog\n./prog 10 3     # -7: своя abs не меняет знак\n./prog          # Segmentation fault", lang: "bash")

#code(read("task02/main.c"), lang: "c", start: 9, end: 18)
]

#slide([#"Задача 2. Ответ (г)"])[
#code("cd examples/Ex3_subdir\ngcc -c main.c -o main.o\n# fatal error: distance.h: No such file or directory\n# \"...\" ищется в каталоге самого файла, затем в -I и системных;\n# <...> — только в каталогах -I и системных\ngcc -I dist -c main.c -o main.o         # способ 1: каталог поиска заголовков\n#                                         способ 2: #include \"dist/distance.h\"\ngcc -c dist/distance.c -o distance.o    # -I не нужен: distance.h лежит рядом\ngcc main.o distance.o -o prog\n./prog 3 10                             # 7", lang: "bash")
]

#slide([#"Задача 3. Макросы и скобки"])[
#"Два макроса должны вычислять модуль числа:"

#code("#define abs1(x) x > 0 ? x : -x\n#define abs2(x) ((x) > 0 ? (x) : (-x))\n\nint x = 7;\nint y = -8;\nint a1 = x + abs1(y);\nint a2 = x + abs2(y);", lang: "c")

- #strong[#"а)"]#" Во что препроцессор раскроет "#raw("a1")#" и "#raw("a2")#"? Какой командой это увидеть?"

- #strong[#"б)"]#" Чему равны "#raw("a1")#" и "#raw("a2")#"? Почему они разные?"

- #strong[#"в)"]#" Всё ли в порядке с "#raw("abs2")#"? Подставьте в него "#raw("a - b")#"."
]

#slide([#"Задача 3. Ответ (а)"])[
#code("cd examples/Ex4\ngcc -E main.c | grep 'int a[12] ='     # -E: остановиться после препроцессора\n#  int a1 = x + y > 0 ? y : -y;\n#  int a2 = x + ((y) > 0 ? (y) : (-y));", lang: "bash")
]

#slide([#"Задача 3. Ответ (б)"])[
#code(read("task03.c"), lang: "c", start: 11, end: 17)
]

#slide([#"Задача 3. Ответ (в)"])[
#code(read("task03.c"), lang: "c", start: 5, end: 8)

#code(read("task03.c"), lang: "c", start: 19, end: 22)
]

#slide([#"Задача 4. Предупреждения компилятора"])[
#"В трёх программах есть ошибки, которые "#raw("gcc")#" находит только с нужными ключами. "#raw("-Wall")#" включает основные предупреждения, "#raw("-Wextra")#" — дополнительные, "#raw("-Wpedantic")#" проверяет соответствие стандарту ISO C, а "#raw("-Werror")#" превращает предупреждения в ошибки."

#code("// warn.c: сложить два числа, если они равны\nint c = -1, d = 0;\nscanf(\"%d %d\", &a, &b);\nif (a = b)\n    c = sum(a, b);\n\n// wextra.c\nint i = 1;\nunsigned int j = -1;\nif (i < j)\n    printf(\"%d is bigger then %d\\n\", j, i);\n\n// wpedantic.c\nint a[0];", lang: "c")

- #strong[#"а)"]#" Что скажет "#raw("gcc -Wall warn.c")#"? Что напечатает программа на вводе "#raw("3 4")#"?"

- #strong[#"б)"]#" Что скажут "#raw("gcc -Wall wextra.c")#" и "#raw("gcc -Wall -Wextra wextra.c")#"? Что напечатает программа и почему условие истинно, хотя "#raw("-1 < 1")#" ложно?"

- #strong[#"в)"]#" Что скажут "#raw("-Wall")#", "#raw("-Wextra")#" и "#raw("-Wpedantic")#" для "#raw("wpedantic.c")#"? Почему код компилируется, если массив нулевого размера запрещён?"

- #strong[#"г)"]#" Что изменит ключ "#raw("-Werror")#"?"
]

#slide([#"Задача 4. Ответ (а)"])[
#code("cd examples/Ex6\ngcc -Wall warn.c -o warn\n# warning: suggest parentheses around assignment used as truth value\n# warning: unused variable 'd'\necho \"3 4\" | ./warn     # 8: a = b кладёт 4 в a, условие 4 истинно", lang: "bash")

#code(read("task04.c"), lang: "c", start: 8, end: 18)
]

#slide([#"Задача 4. Ответ (б)"])[
#code("gcc -Wall wextra.c -o wextra              # без предупреждений\ngcc -Wall -Wextra wextra.c -o wextra\n# warning: comparison of integer expressions of different signedness:\n#   'int' and 'unsigned int' [-Wsign-compare]\n./wextra    # -1 is bigger then 1", lang: "bash")

#code(read("task04.c"), lang: "c", start: 20, end: 28)
]

#slide([#"Задача 4. Ответ (в)"])[
#code("gcc -Wall wpedantic.c         # warning: unused variable 'a'\ngcc -Wextra wpedantic.c       # без предупреждений\ngcc -Wpedantic wpedantic.c    # warning: ISO C forbids zero-size array 'a'", lang: "bash")

#code(read("task04.c"), lang: "c", start: 30, end: 34)
]

#slide([#"Задача 4. Ответ (г)"])[
#code("gcc -Wall -Werror warn.c -o warn\n# error: suggest parentheses around assignment used as truth value\n# error: unused variable 'd'\n# cc1: all warnings being treated as errors\n# предупреждения стали ошибками, программа не собирается", lang: "bash")
]
