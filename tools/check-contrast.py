#!/usr/bin/env python3
"""Проверка палитры темы presenterm на читаемость со слабого проектора.

Что проверяется:

  * контраст WCAG цвета каждого элемента темы к фону `base`: обычный текст
    (default/block_quote/execution_output) — не ниже 7:1 (AAA), остальные
    акценты (заголовок слайда, статусы, inline-код) — не ниже 4.5:1 (AA для
    крупного текста);
  * контраст текста к фонам блоков, на которых он рисуется (surface0 и т.п.);
  * различимость пар элементов, встречающихся на одном слайде: CIE76 ΔE и
    разница светлот L*. Слабый проектор съедает насыщенность и фактически
    показывает серое изображение, поэтому цвета близкой светлоты сливаются даже
    при разных оттенках. Именно так «слиплись» жёлтый заголовок слайда и
    зелёный inline-код — с этого начались жалобы.

Usage:
    tools/check-contrast.py themes/latte-projector.yaml [<тема>/presentation.md]

Второй аргумент необязателен: из frontmatter колоды подтягиваются цвета
`theme.override` — проверяются вместе с темой.

Коды возврата: 0 — всё в норме, 1 — есть нарушение порогов.
"""

from __future__ import annotations

import math
import re
import sys

import yaml

# Пороги контраста с фоном base (WCAG 2.1).
MIN_CONTRAST_TEXT = 7.0  # обычный текст
MIN_CONTRAST_ACCENT = 4.5  # крупный текст, заголовки, статусы
MIN_CONTRAST_ON_BLOCK = 7.0  # текст на фоне блока (surface0)

# Различимость разных цветов, оказавшихся на одном слайде.
# Замеры по CIE76 (ΔE) и по светлоте (ΔL*): выцветший проектор теряет
# насыщенность, поэтому при близкой светлоте цвета сливаются, даже если оттенки
# разные. Порог у каждой пары свой и указан рядом с причиной: у текста на светлом
# фоне требование к ΔL* жёстче, чем у декоративных префиксов заголовков.
WATCHED_PAIRS = (
    # (первый, второй, мин. ΔE, мин. ΔL*, почему пара в списке)
    ("slide_title", "inline_code", 40.0, 8.0, "заголовок слайда и inline-код — та самая жалоба"),
    ("slide_title", "default", 20.0, 8.0, "заголовок и обычный текст"),
    ("inline_code", "default", 20.0, 8.0, "inline-код и обычный текст"),
    ("intro_slide.title", "intro_slide.subtitle", 40.0, 5.0, "титульный слайд: заголовок и подзаголовок"),
    ("intro_slide.date", "intro_slide.subtitle", 40.0, 5.0, "титульный слайд: дата и подзаголовок"),
    (
        "execution_output.status.success",
        "execution_output.status.not_started",
        40.0,
        8.0,
        "соседние блоки +exec: выполнен и ещё не запущен",
    ),
    ("headings.h1", "headings.h2", 40.0, 0.0, "префиксы заголовков — декор, хватает оттенка"),
    ("headings.h3", "headings.h4", 40.0, 0.0, "префиксы заголовков — декор, хватает оттенка"),
    ("headings.h5", "headings.h6", 40.0, 8.0, "префиксы заголовков h5/h6"),
)

# Элементы, которые рендерятся обычным (не крупным) текстом: у них порог
# контраста строже.
BODY_TEXT_ELEMENTS = ("default", "block_quote", "execution_output", "typst")

# Ключи структуры темы, которые не являются элементом.
COLOR_GROUP_KEYS = ("colors", "base_colors", "highlight_colors")

# Значения, которые цветом не являются (в теме встречается mermaid.background).
NON_COLOR_VALUES = ("transparent", "auto", "default", "none", "inherit")
# Фрагменты темы, которые не рисуют текст и потому не проверяются контрастом.
# У pty_output.cursor цвета инвертированы (фон курсора = цвет текста).
SKIP_PREFIXES = ("pty_output.cursor",)

HEX_RE = re.compile(r"#?[0-9a-fA-F]{6}\Z")


def rgb(hex_color: str) -> tuple[int, int, int]:
    hex_color = hex_color.lstrip("#")
    return tuple(int(hex_color[i : i + 2], 16) for i in (0, 2, 4))  # type: ignore[return-value]


def linear(channel: float) -> float:
    return channel / 12.92 if channel <= 0.03928 else ((channel + 0.055) / 1.055) ** 2.4


def relative_luminance(hex_color: str) -> float:
    r, g, b = (linear(c / 255) for c in rgb(hex_color))
    return 0.2126 * r + 0.7152 * g + 0.0722 * b


def contrast(first: str, second: str) -> float:
    la, lb = relative_luminance(first), relative_luminance(second)
    hi, lo = max(la, lb), min(la, lb)
    return (hi + 0.05) / (lo + 0.05)


def lab(hex_color: str) -> tuple[float, float, float]:
    r, g, b = (linear(c / 255) for c in rgb(hex_color))
    x = 0.4124 * r + 0.3576 * g + 0.1805 * b
    y = 0.2126 * r + 0.7152 * g + 0.0722 * b
    z = 0.0193 * r + 0.1192 * g + 0.9505 * b

    def f(t: float) -> float:
        return t ** (1 / 3) if t > 0.008856 else 7.787 * t + 16 / 116

    fx, fy, fz = f(x / 0.95047), f(y / 1.0), f(z / 1.08883)
    return (116 * fy - 16, 500 * (fx - fy), 200 * (fy - fz))


def delta_e(first: str, second: str) -> float:
    return math.dist(lab(first), lab(second))


def lightness(hex_color: str) -> float:
    return lab(hex_color)[0]


def load_theme(path: str) -> dict:
    with open(path, encoding="utf-8") as handle:
        theme = yaml.safe_load(handle)
    if "palette" not in theme:
        raise SystemExit(f"{path}: в теме нет palette")
    return theme


def apply_frontmatter_overrides(theme: dict, path: str) -> None:
    """Накладывает theme.override из frontmatter колоды поверх темы."""
    with open(path, encoding="utf-8") as handle:
        text = handle.read()
    match = re.search(r"^---\n(.*?)\n---", text, re.DOTALL | re.MULTILINE)
    if not match:
        return
    frontmatter = yaml.safe_load(match.group(1)) or {}
    override = ((frontmatter.get("theme") or {}).get("override")) or {}
    for element, spec in override.items():
        target = theme.setdefault(element, {})
        if isinstance(spec, dict):
            target.update(spec)


def resolve(spec: object, palette: dict[str, str]) -> str | None:
    """`palette:name` -> hex; hex оставляем как есть; всё прочее -> None."""
    if not isinstance(spec, str):
        return None
    if spec.startswith("palette:"):
        value = palette.get(spec[len("palette:") :])
        return value.lstrip("#") if value else None
    if spec.lower() in NON_COLOR_VALUES:
        return None
    return spec.lstrip("#") if HEX_RE.match(spec) else None


def element_name(path: str) -> str:
    parts = [part for part in path.split(".") if part]
    while parts and parts[-1] in COLOR_GROUP_KEYS:
        parts.pop()
    return ".".join(parts)


def collect(
    node: dict,
    path: str,
    palette: dict[str, str],
    foregrounds: dict[str, str],
    backgrounds: dict[str, str],
    unresolved: list[str],
) -> None:
    for key, value in node.items():
        child = f"{path}.{key}" if path else key
        if isinstance(value, dict):
            collect(value, child, palette, foregrounds, backgrounds, unresolved)
            continue
        if key not in ("foreground", "background", "color", "prefix"):
            continue
        if isinstance(value, str) and value.startswith("palette:") is False and not HEX_RE.match(value):
            continue  # строка-украшение вроде prefix: "██" или текст футера
        resolved = resolve(value, palette)
        if resolved is None:
            if isinstance(value, str) and (value.startswith("palette:") or HEX_RE.match(value)):
                unresolved.append(f"{child} -> {value!r}")
            continue
        name = element_name(path)
        if key == "prefix":
            name = f"{name}.prefix"
        if not name or name.startswith(SKIP_PREFIXES):
            continue
        (backgrounds if key == "background" else foregrounds)[name] = resolved


def used_palette_names(theme: dict) -> set[str]:
    found: set[str] = set()

    def walk(node: object) -> None:
        if isinstance(node, dict):
            for value in node.values():
                walk(value)
        elif isinstance(node, str) and node.startswith("palette:"):
            found.add(node[len("palette:") :])

    walk({key: value for key, value in theme.items() if key != "palette"})
    return found


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print(__doc__.strip(), file=sys.stderr)
        return 2

    theme = load_theme(argv[1])
    if len(argv) > 2:
        apply_frontmatter_overrides(theme, argv[2])

    palette = {name: str(value).lstrip("#") for name, value in theme["palette"].get("colors", {}).items()}
    foregrounds: dict[str, str] = {}
    backgrounds: dict[str, str] = {}
    unresolved: list[str] = []
    collect({k: v for k, v in theme.items() if k != "palette"}, "", palette, foregrounds, backgrounds, unresolved)

    base = backgrounds.get("default")
    if base is None:
        raise SystemExit("в палитре нет base (фон)")

    failures: list[str] = list(f"неизвестный цвет: {item}" for item in unresolved)

    print(f"Фон base: #{base}\n")
    print(f"{'элемент':<44} {'hex':<9} {'контраст':>9}  {'L*':>5}  порог")
    for name, value in foregrounds.items():
        ratio = contrast(value, base)
        limit = MIN_CONTRAST_TEXT if name in BODY_TEXT_ELEMENTS else MIN_CONTRAST_ACCENT
        flag = ""
        if ratio < limit:
            flag = "  <-- МАЛО"
            failures.append(f"{name}: контраст {ratio:.2f} < {limit:.1f}")
        print(f"{name:<44} #{value:<8} {ratio:>8.2f}  {lightness(value):>5.1f}  {limit:.1f}{flag}")

    print("\nТекст на фонах блоков:")
    text = foregrounds.get("default")
    for name, background in backgrounds.items():
        if name == "default" or text is None:
            continue
        ratio = contrast(text, background)
        flag = ""
        if ratio < MIN_CONTRAST_ON_BLOCK:
            flag = "  <-- МАЛО"
            failures.append(f"текст на {name}: контраст {ratio:.2f} < {MIN_CONTRAST_ON_BLOCK:.1f}")
        print(f"  {name:<42} #{background:<8} {ratio:>8.2f}{flag}")

    print("\nРазличимость пар элементов на одном слайде:")
    print(f"{'пара':<58} {'ΔE':>6}  {'треб.':>6}  {'ΔL*':>5}  {'треб.':>5}")
    for first, second, min_de, min_dl, why in WATCHED_PAIRS:
        if first not in foregrounds or second not in foregrounds:
            continue
        de = delta_e(foregrounds[first], foregrounds[second])
        dl = abs(lightness(foregrounds[first]) - lightness(foregrounds[second]))
        flag = ""
        if de < min_de or dl < min_dl:
            flag = "  <-- СЛИТНО"
            failures.append(f"{first} / {second}: ΔE {de:.1f} (нужно {min_de:.0f}), ΔL* {dl:.1f} (нужно {min_dl:.0f}) — {why}")
        print(f"{first + ' / ' + second:<58} {de:>6.1f}  {min_de:>6.0f}  {dl:>5.1f}  {min_dl:>5.0f}{flag}")

    unused = sorted(set(palette) - used_palette_names(theme))
    print(f"\nТема не использует: {', '.join(unused) or '-'}")

    if failures:
        print("\nНарушения:")
        for item in failures:
            print(f"  * {item}")
        return 1

    print("\nOK: контраст и различимость пар в норме.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
