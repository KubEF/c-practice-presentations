#!/usr/bin/env python3
"""Замер реальных цветов отрендеренного слайда: что видно, а что выцвело.

Палитру темы проверяет `tools/check-contrast.py`, но подсветку кода тема
задаёт только именем (`code.theme_name`), и её цвета оттуда не видны. Этот
скрипт смотрит на готовую картинку слайда: находит блок кода и считает
контраст каждого цвета текста внутри него.

Порядок работы:

    presenterm --config-file <конфиг с export.dimensions> --export-pdf \\
        -o /tmp/deck.pdf <тема>/presentation.md
    pdftoppm -f 3 -l 3 -r 200 -png -aa no /tmp/deck.pdf /tmp/deck
    tools/check-render-colors.py /tmp/deck-03.png

`-aa no` обязателен: со сглаживанием половина пикселей — это смеси цвета
текста с фоном, и измерение показывает несуществующие «бледные» цвета.

Usage:
    tools/check-render-colors.py <слайд>.png [<слайд2>.png ...]

Код возврата: 0 — всё в норме, 1 — есть цвета текста ниже порога.
"""

from __future__ import annotations

import sys
from collections import Counter

from PIL import Image

# Порог для текста внутри блока кода: 4.5:1 — AA для обычного текста.
# Ниже — на бледном проекторе символы перестают читаться.
MIN_CONTRAST = 4.5

# Сколько самых частых цветов считать «фонами» (фон слайда и фон блока кода).
BACKGROUND_CANDIDATES = 2

# Цвет должен встретиться столько раз, чтобы быть текстом, а не мусором
# от рамок и заливок.
MIN_PIXELS = 40

# Ближе этого к фону блока — считается фоном/рамкой, а не текстом.
MIN_INK_CONTRAST = 1.5


def linear(channel: float) -> float:
    return channel / 12.92 if channel <= 0.03928 else ((channel + 0.055) / 1.055) ** 2.4


def luminance(color: tuple[int, int, int]) -> float:
    r, g, b = (linear(c / 255) for c in color)
    return 0.2126 * r + 0.7152 * g + 0.0722 * b


def contrast(first: tuple[int, int, int], second: tuple[int, int, int]) -> float:
    la, lb = luminance(first), luminance(second)
    hi, lo = max(la, lb), min(la, lb)
    return (hi + 0.05) / (lo + 0.05)


def hex_color(color: tuple[int, int, int]) -> str:
    return "#%02x%02x%02x" % color


def find_code_box(image: Image.Image, code_background: tuple[int, int, int]) -> tuple[int, int, int, int] | None:
    """Прямоугольник, занятый блоком кода (по пикселям его фона)."""
    width, height = image.size
    pixels = image.load()
    left, top, right, bottom = width, height, -1, -1
    for y in range(height):
        for x in range(width):
            if pixels[x, y] == code_background:
                if x < left:
                    left = x
                if x > right:
                    right = x
                if y < top:
                    top = y
                if y > bottom:
                    bottom = y
    if right < 0:
        return None
    return (left, top, right + 1, bottom + 1)


def check(path: str) -> list[str]:
    image = Image.open(path).convert("RGB")
    counts = Counter(image.get_flattened_data())
    if not counts:
        return [f"{path}: пустая картинка"]

    backgrounds = {color for color, _ in counts.most_common(BACKGROUND_CANDIDATES)}
    slide_background = counts.most_common(1)[0][0]
    # Фон блока кода — самый частый цвет, кроме фона слайда. Если его нет,
    # блок кода сливается со слайдом и проверять нечего.
    code_background = next((color for color in backgrounds if color != slide_background), None)

    print(f"== {path}")
    print(f"   фон слайда {hex_color(slide_background)}")
    if code_background is None:
        print("   блок кода на слайде не найден: фон кода совпадает с фоном слайда\n")
        return []

    box = find_code_box(image, code_background)
    if box is None:
        print("   блок кода на слайде не найден\n")
        return []
    left, top, right, bottom = box
    print(f"   блок кода {hex_color(code_background)}, границы {left},{top}..{right},{bottom}")
    print("   цвета текста на фоне блока:")

    # Текст вне прямоугольника (заголовок слайда, подписи) к блоку не относится.
    inside = Counter(image.crop((left, top, right, bottom)).get_flattened_data())
    inks = [
        (color, number)
        for color, number in inside.most_common(80)
        if number >= MIN_PIXELS and contrast(color, code_background) >= MIN_INK_CONTRAST
    ]

    problems: list[str] = []
    for color, number in sorted(inks, key=lambda item: contrast(item[0], code_background)):
        ratio = contrast(color, code_background)
        flag = ""
        if ratio < MIN_CONTRAST:
            flag = "  <-- НЕ ВИДНО"
            problems.append(f"{path}: {hex_color(color)} — контраст {ratio:.2f} < {MIN_CONTRAST:.1f}")
        print(f"     {hex_color(color)}  {number:7d}px  контраст {ratio:5.2f}{flag}")
    print()
    return problems


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print(__doc__.strip(), file=sys.stderr)
        return 2

    problems: list[str] = []
    for path in argv[1:]:
        problems.extend(check(path))

    if problems:
        print("Нарушения:")
        for problem in problems:
            print(f"  * {problem}")
        return 1

    print("OK: текст в блоках кода не ниже порога контраста.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
