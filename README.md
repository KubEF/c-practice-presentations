# Практика по C: презентации (presenterm)

```
intro/                     # вводная колода, демо presenterm (presenterm -x -X)
pointers/                  # указатели (presenterm -x)
pointers-alt/              # указатели, альтернативный набор
arrays/                    # массивы
c-basics/                  # базовые примеры (presenterm -x)
compilation/               # компиляция; examples/ — исходные примеры с лекции
bitops-floats/             # битовые операции и float
spare/                     # запасные задачи
themes/                    # тема latte-projector.yaml, шаблон slides.typ
tools/                     # прогон, экспорт, сборка typst, проверка контраста
AGENTS.md                  # правила создания колод
```

Каждая колода — `<тема>/presentation.md`, код задач лежит рядом в `taskNN.c`.

## Шрифты

Код в экспортированном PDF набран JetBrains Mono: задано в `export.pdf.fonts`
глобального конфига presenterm. `--config-file` заменяет этот конфиг целиком,
поэтому блок `fonts` нужно продублировать.

## Тема

`themes/latte-projector.yaml` — `catppuccin-latte` с цветами, усиленными для
бледного проектора, и подсветкой `Coldark-Cold`. Подключается в колоде через
`theme.path: ../themes/latte-projector.yaml`.

## Typst

У `compilation/`, `bitops-floats/` и `spare/` есть `presentation.typ`. Он
генерируется из `presentation.md`, руками не правится:

```sh
tools/build-typst.sh compilation/presentation.md bitops-floats/presentation.md spare/presentation.md
```
