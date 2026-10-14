#!/usr/bin/env bash
# Headless render sweep for a presenterm deck.
#
# presenterm needs a TTY, so run it inside a pty of a fixed size, answer the
# device-attributes query, send one space per slide and then "q", and strip the
# ANSI escapes from the capture. Output (the whole session, slides concatenated)
# goes to stdout.
#
# Usage: tools/sweep.sh <deck.md> [width] [slides] [presenterm args...]
#
#   tools/sweep.sh pointers-presentation.md 170 30 -x | less -R
#   tools/sweep.sh presentation.md 120 30 -x -X
set -euo pipefail

here=$(cd "$(dirname "$0")" && pwd)
deck=${1:?usage: sweep.sh <deck.md> [width] [slides] [presenterm args...]}
width=${2:-170}
slides=${3:-30}
if [ $# -ge 3 ]; then shift 3; else shift $#; fi

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

python3 - "$slides" > "$tmp/keys.json" <<'PY'
import json, sys
n = int(sys.argv[1])
events = [[1.2, r"\u001b[?1;2c\u001b[0n"]]
t = 2.0
for _ in range(n):
    events.append([round(t, 2), " "])
    t += 0.2
events.append([round(t + 0.6, 2), "q"])
print(json.dumps(events))
PY

python3 "$here/ptyrun.py" 44 "$width" "$tmp/raw.bin" "$(cat "$tmp/keys.json")" \
    -- presenterm "$@" "$deck" >/dev/null
python3 "$here/strip.py" "$tmp/raw.bin"
