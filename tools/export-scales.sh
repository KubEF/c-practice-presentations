#!/usr/bin/env bash
# Экспорт колоды в PDF и HTML при нескольких «масштабах».
#
# «Масштаб» экспорта в presenterm задаётся размером сетки слайда
# (export.dimensions.columns / rows): от него зависят размер страницы PDF и то,
# сколько контента помещается на слайд. Флага размера/зума у CLI нет.
#
# Usage: tools/export-scales.sh <deck.md> [ШИРИНАxВЫСОТА ...]
#
#   tools/export-scales.sh c-basics/presentation.md
#   tools/export-scales.sh c-basics/presentation.md 120x60 160x64 220x72
#
# Если на экспортированных слайдах контент обрезан (обычно у слайдов с длинным
# кодом и выводом `+exec`), увеличьте rows — сетка та же, страница больше.
#
# Файлы кладутся в export/<колода>-<размер>.{pdf,html}. Каталог в .gitignore.
set -euo pipefail

here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/.." && pwd)
deck=${1:?usage: export-scales.sh <deck.md> [ШИРИНАxВЫСОТА ...]}
if [ $# -ge 1 ]; then shift; fi

# Абсолютный путь: presenterm ищет `path:`-сниппеты относительно файла колоды,
# а относительные пути — от текущего каталога.
case "$deck" in
    /*) ;;
    *) deck="$(cd "$(dirname "$deck")" && pwd)/$(basename "$deck")" ;;
esac

sizes=("$@")
if [ ${#sizes[@]} -eq 0 ]; then
    sizes=(120x60 160x64 220x72)
fi

base=$(basename "$deck" .md)
# deck files are all called presentation.md, so name exports after the deck's directory
if [ "$base" = presentation ]; then
    base=$(basename "$(dirname "$deck")")
fi
outdir="$root/export"
mkdir -p "$outdir"

# Шрифт блоков кода в PDF: export.pdf.fonts применяется к <pre>, то есть только
# к коду. presenterm подставляет вместо ~/.config/presenterm/config.yaml тот
# конфиг, что передан через --config-file (без смешивания), поэтому блок fonts
# дублируем в генерируемый конфиг — иначе экспорт из скрипта отличается от
# экспорта вручную. Без шрифта на месте просто экспорт без fonts.
font_dir=${JETBRAINS_FONT_DIR:-/usr/share/fonts/TTF}
if [ ! -f "$font_dir/JetBrainsMonoNerdFontMono-Regular.ttf" ]; then
    echo "предупреждение: JetBrains Mono не найден в $font_dir —" \
        "PDF уйдёт на системный моноширинный шрифт" >&2
    font_dir=
fi

for size in "${sizes[@]}"; do
    columns=${size%x*}
    rows=${size#*x}

    cfg=$(mktemp)
    trap 'rm -f "$cfg"' EXIT
    {
        printf 'export:\n  dimensions:\n    columns: %s\n    rows: %s\n' \
            "$columns" "$rows"
        if [ -n "$font_dir" ]; then
            printf '  pdf:\n    fonts:\n'
            printf '      normal: %s\n'      "$font_dir/JetBrainsMonoNerdFontMono-Regular.ttf"
            printf '      bold: %s\n'        "$font_dir/JetBrainsMonoNerdFontMono-Bold.ttf"
            printf '      italic: %s\n'      "$font_dir/JetBrainsMonoNerdFontMono-Italic.ttf"
            printf '      bold_italic: %s\n' "$font_dir/JetBrainsMonoNerdFontMono-BoldItalic.ttf"
        fi
    } > "$cfg"

    echo "== $size ($columns x $rows)"
    # -x: колода собирает ответы gcc на слайдах, без него видно
    # "snippet +exec is disabled".
    presenterm -x --config-file "$cfg" \
        --export-pdf -o "$outdir/$base-$size.pdf" "$deck"
    presenterm -x --config-file "$cfg" \
        --export-html -o "$outdir/$base-$size.html" "$deck"
done

echo "готово: $outdir"
