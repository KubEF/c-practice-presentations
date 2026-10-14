# Пример презентации (presenterm)

Простая демонстрация возможностей [presenterm](https://github.com/mfontanini/presenterm).
Примеры кода — на C и вынесены в отдельные файлы `code/*.c`.
Тема — `catppuccin-latte` (задана во frontmatter презентации).

```
presentation.md            # вводная презентация (демо возможностей)
code/
  hello.c                  # hello world, исполняется по Ctrl+E
  person.c                 # struct Person + greet, динамическая подсветка
  dynamic_array.c          # malloc / free; фрагмент виден, весь файл запускается
pointers-presentation.md   # колода задач: слайд с условием + слайд с ответом
pointers/
  taskNN.c                 # код задач — обычные C-программы, единый источник правды
tools/                     # headless-прогон презентации (см. «Проверка колоды»)
AGENTS.md                  # правила создания новых презентаций
README.md
```

> Как делать новые презентации в этом проекте (структура слайдов, конвенции,
> проверка перед коммитом) — см. [`AGENTS.md`](AGENTS.md).

## Запуск

```sh
presenterm presentation.md           # обычный показ

presenterm -x -X presentation.md     # + исполнение кода

presenterm -x pointers-presentation.md   # колода задач: ответы 1, 6, 8, 10 запускают код
```

`-x` нужен колоде задач: на слайдах с ответами задач 1, 6, 8 и 10 стоит
`file +exec` (`Ctrl+E` компилирует и запускает соответствующий `pointers/taskNN.c`).

`-x` — разрешает `+exec` (слайд «Hello, C», запуск по `Ctrl+E`).
`-X` — разрешает `+exec_replace` (слайд «Две колонки», запуск автоматический).

Управление:

- `Space` / `→` / `j` — следующий слайд или пункт
- `←` / `k` — назад
- `<число>G` — прыжок на слайд, `G` — последний, `gg` — первый
- `Ctrl+E` — выполнить исполняемый сниппет на текущем слайде
- `Ctrl+P` — список слайдов
- `q` — выход
- `?` — справка по клавишам

## Внешние файлы

### Код — тип сниппета `file`

````markdown
```file +line_numbers
path: code/hello.c
language: c
```
````

- `path` — путь относительно каталога файла презентации.
- `language` — язык для подсветки.
- `start_line` / `end_line` — показать только фрагмент файла.
- Работают флаги: `+line_numbers`, `+no_background`, `+exec`, динамическая подсветка `{1-4|6-9|all}`.

### Markdown — команда `include`

```markdown
<!-- include: slides/intro.md -->
```

Содержимое внешнего markdown подставляется как часть презентации. Пути внутри
включаемого файла резолвятся относительно **его** расположения.

## Исполнение кода

К C применим `+exec`: сниппет компилируется и запускается по `Ctrl+E`, вывод
показывается под блоком. Встроенный исполнитель для C вызывает `gcc`, поэтому
нужен `gcc` в `PATH` (свой компилятор можно задать в конфиге presenterm).

| Атрибут | Когда выполняется | Нужен флаг |
| --- | --- | --- |
| `+exec` | по `Ctrl+E` | `-x` |
| `+auto_exec` | сразу при показе слайда | `-x` |
| `+exec_replace` | автоматически, блок заменяется выводом | `-X` |

Исполнение выключено по умолчанию; в примере `+exec` стоит на слайде «Hello, C»
(`code/hello.c`, C-сниппет, запуск по `Ctrl+E`), а `+exec_replace` — на слайде
«Две колонки» (bash-сниппет, запускается автоматически).

- `--validate-snippets` — прогоняет `+exec` / `+exec_replace` при старте и сообщает
  об ошибках компиляции.
- `+validate` — только проверка во время разработки, без показа вывода.

### «Две колонки»: показываем фрагмент, запускаем весь файл

Слайд показывает только строки 6–9 `code/dynamic_array.c` (`start_line` /
`end_line`) — это просмотр. Рядом `bash +exec_replace`, который собирает и
запускает **весь** файл и заменяет свой блок выводом:

```bash +exec_replace +no_background
gcc -Wall -Wextra -std=c17 code/dynamic_array.c -o /tmp/dynamic_array
/tmp/dynamic_array
```

`+exec_replace` запускается автоматически при показе слайда и требует `-X` (или
`snippet.exec_replace.enable: true` в конфиге). Без этого presenterm покажет сам
скрипт и надпись `snippet +exec_replace is disabled, run with -X to enable`.
`+no_background` убирает фон у вывода — так строка `42` выглядит как результат, а
не как блок кода.

Почему не `+exec` на самом C-фрагменте: `start_line` / `end_line` отрезают
содержимое сниппета **до** исполнения, поэтому C-сниппет получил бы только 4
видимые строки и не скомпилировался бы. Bash-сниппет компилирует реальный файл
целиком, а `code/dynamic_array.c` остаётся обычной программой, которую можно
собрать отдельно.

Исполняемые сниппеты запускаются в каталоге файла презентации, поэтому
относительный путь `code/dynamic_array.c` работает независимо от того, откуда
вы запустили `presenterm`.

> ⚠️ Не запускайте `-x` / `-X` на чужих презентациях: код выполняется на вашей машине
> без изоляции.

## Проверка колоды

`tools/sweep.sh` прогоняет презентацию в псевдотерминале фиксированного размера
(presenterm требует настоящий TTY), листает слайды и снимает ANSI-разметку:

```sh
tools/sweep.sh presentation.md 170 10 -x -X       # вводная колода
tools/sweep.sh pointers-presentation.md 170 30 -x # колода задач, широкая
tools/sweep.sh pointers-presentation.md 120 30 -x # колода задач, узкая
```

Аргументы: `<колода> [ширина] [число слайдов] [флаги presenterm...]`. Смотреть
вывод удобно через `less -R`. Что проверять: нет строк `error` / `disabled` /
`not found`, все цели `path:` на месте, итоговый номер слайда совпадает
(число `<!-- end_slide -->` + 2). Подробнее — в `AGENTS.md`, §9.

## Экспорт

```sh
presenterm --export-pdf -o presentation.pdf presentation.md
presenterm --export-html -o presentation.html presentation.md
```

## Установка presenterm

```sh
# Arch Linux
sudo pacman -S presenterm

# или бинарник с GitHub
curl -L -o presenterm.tar.gz \
  https://github.com/mfontanini/presenterm/releases/latest/download/presenterm-x86_64-unknown-linux-gnu.tar.gz
tar xzf presenterm.tar.gz && install -m755 presenterm-*/presenterm ~/.local/bin/
```

## Структура `presentation.md`

- `---` в начале — frontmatter (`title`, `sub_title`, `author`, `theme`) → титульный слайд.
- `<!-- end_slide -->` — разделитель слайдов.
- `Заголовок` + `---` на следующей строке — setext-заголовок (заголовок слайда), **не** разделитель.
- `<!-- pause -->` — раскрытие контента по шагам.
- `<!-- column_layout: [1, 1] -->` + `<!-- column: 0 -->` — колонки, `<!-- reset_layout -->` — сброс.

## Тема

Встроенные темы: `catppuccin-latte`, `catppuccin-frappe`, `catppuccin-macchiato`, `catppuccin-mocha`,
`dark`, `light`, `gruvbox-dark`, `terminal-dark`, `terminal-light`, `tokyonight-moon`,
`tokyonight-day`, `tokyonight-night`, `tokyonight-storm`. Список: `presenterm --list-themes`.

```yaml
---
theme:
  name: catppuccin-latte
---
```

Либо через CLI: `presenterm --theme catppuccin-latte presentation.md`.
Свои темы кладутся в `~/.config/presenterm/themes/*.yaml`.
