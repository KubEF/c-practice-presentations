---
title: Пример презентации
sub_title: presenterm + C
author: Ваше имя
theme:
  name: catppuccin-latte
  override:
    code:
      line_numbers: true
---

Что такое presenterm?
---

_presenterm_ — презентации прямо в терминале, слайды пишутся в **Markdown**.

* Подсветка кода на C (и 50+ других языков).
* Код вынесен в отдельные `.c` файлы и подключается в слайды.
* Исполнение кода по `Ctrl+E`.
* Колонки, таблицы, экспорт в PDF и HTML.

<!-- pause -->

Тема — `catppuccin-latte`, задана во frontmatter.

<!-- end_slide -->

Hello, C
---

Код лежит в `hello.c` и подключён как внешний сниппет:

```file +exec +line_numbers
path: hello.c
language: c
```

<!-- pause -->

Нажмите `Ctrl+E`, чтобы скомпилировать и запустить (нужен запуск с `-x`).

```bash +no_background
cc -Wall -Wextra -std=c17 hello.c -o hello && ./hello
```

<!-- end_slide -->

Структуры
---

Динамическая подсветка работает и для внешних файлов:

```file {3-6|8-9|12-15|all} +line_numbers
path: person.c
language: c
```

<!-- end_slide -->

Две колонки
---

<!-- column_layout: [1, 1] -->

<!-- column: 0 -->

### Динамическая память

Фрагмент `dynamic_array.c`, строки 6–9:

```file +line_numbers
path: dynamic_array.c
language: c
start_line: 6
end_line: 9
```

Собрать и запустить весь файл:

```bash +exec_replace +no_background
gcc -Wall -Wextra -std=c17 dynamic_array.c -o /tmp/dynamic_array
/tmp/dynamic_array
```

<!-- column: 1 -->

### Типы

| Тип      | Размер |
| -------- | ------ |
| `char`   | 1      |
| `int`    | 4      |
| `double` | 8      |
| `void *` | 8      |

<!-- end_slide -->

Диаграммы
---

Сниппет `d2` с атрибутом `+render` превращается в картинку инструментом
[d2](https://d2lang.com) (нужен `d2` в `PATH`):

```d2 +render +width:80%
source: "hello.c"
compiler: "gcc -Wall -Wextra -std=c17"
binary: "./hello"
terminal: "терминал"

source -> compiler: сборка
compiler -> binary
binary -> terminal: "Ctrl+E"
```

<!-- pause -->

Масштаб картинки задаётся атрибутом `+width:N%` (уходит в `d2 --scale`).

<!-- end_slide -->

Форматирование
---

* **Жирный**, _курсив_, `код`.
* Ссылка: [presenterm](https://github.com/mfontanini/presenterm)
* <span style="color: cyan">Цветной</span> текст.

> Компилируйте с `-Wall -Wextra`.

<!-- end_slide -->

Спасибо!
---

Запуск: `presenterm -x presentation.md`
