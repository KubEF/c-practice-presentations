#!/usr/bin/env bash
# Build typst versions of decks: regenerate <topic>/presentation.typ from
# <topic>/presentation.md (tools/md2typ.py), compile it to
# <topic>/presentation-typst.pdf and check that every slide fits on one page
# (page count == number of <!-- end_slide --> separators + 2).
#
# Usage: tools/build-typst.sh <topic>/presentation.md [...]
#
#   tools/build-typst.sh compilation/presentation.md bitops-floats/presentation.md spare/presentation.md
set -euo pipefail

root=$(cd "$(dirname "$0")/.." && pwd)
cd "$root"
[ $# -ge 1 ] || { echo "usage: build-typst.sh <topic>/presentation.md [...]" >&2; exit 2; }

status=0
for deck in "$@"; do
    python3 tools/md2typ.py "$deck"
    typ=${deck%.md}.typ
    pdf=${deck%.md}-typst.pdf
    typst compile --root . "$typ" "$pdf"
    pages=$(pdfinfo "$pdf" | awk '/^Pages:/ {print $2}')
    expected=$(( $(grep -c '<!-- end_slide -->' "$deck") + 2 ))
    if [ "$pages" -ne "$expected" ]; then
        echo "$pdf: $pages pages, expected $expected — a slide overflowed" >&2
        status=1
    else
        echo "$pdf: $pages pages"
    fi
done
exit $status
