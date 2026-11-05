#!/usr/bin/env python3
"""Copy JetBrains Mono with programming ligatures switched off.

JetBrains Mono draws `->`, `==`, `!=`, `<=` ... as ligatures through the
OpenType `calt` feature. presenterm's PDF export has no knob for font
features, so the copy keeps the `calt` feature record but with no lookups:
the shaper finds nothing to substitute and every character keeps its own
glyph. Other features (`zero`, `cv*`, ...) are untouched.

The copies go to a directory fontconfig does not scan, so the system's
JetBrains Mono (and the terminal) stay as they are.

Usage: tools/nolig-fonts.py [SRC_DIR] [OUT_DIR]
"""
import os
import sys

from fontTools.ttLib import TTFont

STYLES = ("Regular", "Bold", "Italic", "BoldItalic")
src_dir = sys.argv[1] if len(sys.argv) > 1 else "/usr/share/fonts/TTF"
out_dir = sys.argv[2] if len(sys.argv) > 2 else os.path.expanduser(
    "~/.local/share/presenterm/fonts")

os.makedirs(out_dir, exist_ok=True)
for style in STYLES:
    name = f"JetBrainsMonoNerdFontMono-{style}.ttf"
    font = TTFont(os.path.join(src_dir, name))
    cleared = 0
    for record in font["GSUB"].table.FeatureList.FeatureRecord:
        if record.FeatureTag == "calt":
            record.Feature.LookupListIndex = []
            record.Feature.LookupCount = 0
            cleared += 1
    if cleared == 0:
        sys.exit(f"{name}: no calt feature, nothing to switch off")
    font.save(os.path.join(out_dir, name))
    print(f"{out_dir}/{name}: calt cleared in {cleared} script(s)")
