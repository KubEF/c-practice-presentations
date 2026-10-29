# Presentation guidelines

How presentations in this repository are built. Read this before creating or
editing a deck — it encodes the conventions that the existing decks follow and
the mistakes that were already made once.

The goal: a request like *"make a presentation with roughly these tasks"* should
produce a deck that looks and behaves exactly like the existing ones, with no
extra back-and-forth.

Runtime: **presenterm 0.16.1** (`~/.local/bin/presenterm`, also on `PATH` as
`presenterm`). Every deck is plain Markdown; every piece of C code shown on a
slide lives in a real `.c` file.

---

## 1. Repository layout

```
intro/                     intro/demo deck (exec demos), requires -x -X
  presentation.md
  hello.c, person.c, dynamic_array.c
pointers/                  task deck, requires -x (four answers use file +exec)
  presentation.md
  taskNN.c                 one file per task — single source of truth for code
pointers-alt/              alternative pointers task deck (exec-free)
arrays/                    arrays task deck
c-basics/                  basic examples, answers built by bash +exec (-x)
compilation/               compilation task deck (exec-free)
  examples/                original lecture examples Ex1–Ex6 used by the slides
bitops-floats/             bit operations and floats (exec-free, link with -lm)
spare/                     spare tasks for a fast group (exec-free)
themes/latte-projector.yaml  shared theme for pale projectors (see §11)
tools/                     headless render helpers and contrast checkers (see §9, §11)
export/                    exported PDF/HTML, ignored by git
AGENTS.md                  this file
```

- One deck per topic: a directory `<topic>/` holding `presentation.md` and the
  deck's task files `<topic>/taskNN.c` (plus any multi-file examples). Nothing
  deck-specific lives in the repository root; only `themes/`, `tools/`,
  `export/` and the docs are shared.
- Every path inside a deck is relative to its directory: `path: task01.c`,
  `theme.path: ../themes/latte-projector.yaml`, and shell commands shown or
  executed on slides assume the deck directory as the working directory.
- A task deck is a sequence of **pairs**: a condition slide, then an answer
  slide. Nothing else, unless a topic genuinely needs an intro/TOC slide.
- Language: slide text is **Russian**, code comments are Russian too, commit
  messages and this file are **English**.

## 2. Hard rules

1. **Frontmatter of every deck** carries the theme and turns line numbers on
   once, globally:

   ```yaml
   ---
   title: "Название"
   sub_title: "Подзаголовок"
   theme:
     path: ../themes/latte-projector.yaml
     override:
       code:
         line_numbers: true
   ---
   ```

   The override is applied to every snippet by `push_code`, so it belongs here
   and not on individual snippets. `author` is optional.

   `theme.path` and `theme.name` cannot be combined; `theme.path` is relative to
   the deck file. Use `path: ../themes/latte-projector.yaml` for decks shown on a
   pale projector, and `name: catppuccin-latte` only for decks that need the
   stock look (see §11).

2. **All code lives in `.c` files**, never inline on answer slides. Every file
   must compile warning-free and be a standalone program:

   ```sh
   gcc -Wall -Wextra -std=c17 -fsyntax-only pointers/task01.c   # from the repo root
   ```

3. **One statement per line** in the `.c` files (expand `if (x) return y;`,
   loop bodies, etc.). Exception: struct/union member lists stay on one line.

4. **Answer slides show code full width** through a `file` snippet. No exec, no
   columns, no file-name captions.

5. **Expected output is a trailing comment** next to the `printf` that produces
   it, so the deck never needs to run anything:

   ```c
   printf("%d\n", *p);            // а: 10
   *p = 20;                       // б
   ```

6. **Hide lines with `start_line`/`end_line`, never by editing the file.** A file
   stays a complete, compilable program; a slide may show a fragment of it. Note
   that line numbers in a fragment restart at 1 (`filter_lines` cuts content
   before numbering).

7. **Don't put spacing into a deck.** Margins/padding and the cell-level
   `font_size` live in the theme: stock values in `catppuccin-latte`, and the
   projector deck's padding deltas in `themes/latte-projector.yaml`
   (documented and measured in §11). Font *family* is not a theme key at all —
   the terminal font comes from `kitty.conf` and the PDF export's code font
   from `export.pdf.fonts` (README, «Шрифты»). `theme.override` in a
   frontmatter is for the per-deck knobs only (`code.line_numbers`). Earlier
   requests to change margins were declined for the same reason: a deck must
   not carry theme numbers.

## 3. Deck skeleton

```markdown
---
title: "Практика по C"
sub_title: "Указатели и выравнивание"
theme:
  name: catppuccin-latte
  override:
    code:
      line_numbers: true
---

Задача 1. Короткое название
===========================

Короткая формулировка условия.

```c
int x = 10;
int *p = &x;
```

* **а)** первый пункт
* **б)** второй пункт

<!-- end_slide -->

Задача 1. Ответ
===============

```file
path: task01.c
language: c
```

<!-- end_slide -->
```

- Slide separator is `<!-- end_slide -->`; the footer's total slide count equals
  the number of separators + 2 — use that to sanity-check a render.
- A slide title is a **setext heading**: title line followed by `=====` (or
  `-----`). It is a heading, not a separator.
- `<!-- pause -->` reveals content step by step — use it when a condition slide
  would otherwise be an overwhelming wall of text.
- Keep the numbering from the source lecture; **gaps are fine** (the current
  pointers deck is 1-6, 8-11, 13-14 because tasks 7, 12, 15-20 were dropped).

## 4. Snippet cookbook

### External C file

````markdown
```file
path: task04.c
language: c
start_line: 1
end_line: 13
```
````

- `path` is relative to the **deck file's** directory.
- `start_line` / `end_line` trim the shown content (YAML keys of `ExternalFile`).
- There is **no native filename caption** for a code block. If a caption is
  needed, it is a separate line of inline code — but the current decks have none,
  and captions were explicitly removed once, so don't add them unasked.

### Flags

| Flag | Effect |
| --- | --- |
| `+line_numbers` | per-snippet line numbers (already on globally by the theme) |
| `+no_background` | no background box (used for program output) |
| `+exec` | run on `Ctrl+E`, needs `presenterm -x` |
| `+auto_exec` | run as soon as the slide appears, needs `-x` |
| `+exec_replace` | run automatically and replace the block with its output, needs `-X` |
| `+validate` | compile-check while authoring, show no output |
| `{1,3,5-7}` | selective highlighting — only these lines are highlighted |
| `{1,3\|5-7}` | dynamic highlighting — one frame per `\|`, `all` highlights everything |

Highlighting is expressed **only** through the `{...}` code attribute on the
fence. There is no `+dynamic` / `+highlight` flag: ```` ```c +dynamic ```` fails with
`invalid code attribute: +dynamic`.

### Execution rules

- Exec snippets run in the presentation file's parent directory, so relative
  paths like `task01.c` work from any launch directory.
- **`start_line`/`end_line` cut the snippet before it executes.** A C snippet
  that shows only lines 6-9 of a file will not compile. To run a whole file while
  showing a fragment, keep the fragment as a plain `file` snippet and let a
  neighbouring `bash +exec_replace +no_background` snippet compile the real file:

  ````markdown
  ```bash +exec_replace +no_background
  gcc -Wall -Wextra -std=c17 dynamic_array.c -o /tmp/dynamic_array
  /tmp/dynamic_array
  ```
  ````

- Task decks are **exec-free by default** (expected output lives in comments).
  The current pointers deck makes an exception for four tasks (1, 6, 8, 10) whose
  printed values *are* the answer: those use `file +exec`, so that deck must be
  run with `presenterm -x pointers/presentation.md`. The intro deck
  `intro/presentation.md` needs `-x -X`.
- Without the right flag presenterm shows `snippet +exec is disabled, run with -x
  to enable` — mention the required flag in the README tree when a deck uses it.

## 5. Columns

- Integer ratios only: `<!-- column_layout: [1, 1] -->`, `[3, 2]`, … Fractional
  values (`[1, 1.5]`) are rejected — `columns` is `&[u8]`.
- Each column is introduced with `<!-- column: N -->`, and the layout is closed
  with `<!-- reset_layout -->`.
- `latte-projector` left-aligns code blocks with a fixed 3% margin, so a code
  block fills its column (or the slide) instead of hugging its content; there is
  no `minimum_size` any more. Put the wider content (tables, prose) in the
  other column, and expect presenterm to wrap long table cells inside a column
  instead of shrinking them (this is accepted).

## 6. Task-deck conventions

- **`main`**: keep it in the answer snippet only when its `printf` output *is* the
  answer (tasks 1, 6, 8, 10, 11). Otherwise hide it with `start_line`/`end_line`.
- **Includes**: hide `#include` lines with `start_line` when the shown code uses
  nothing from them; keep the include when the code uses a name it provides
  (e.g. `size_t` from `<stdio.h>`, task 4 — a deliberately debatable case).
- **Trimming a definition** (e.g. trivial `add`/`sub`/`mul`) is done with
  `start_line`/`end_line` around the interesting part. The snippet may then
  reference functions that are defined earlier in the file but not shown; that is
  accepted, and the condition slide right before it usually shows them.
- Question snippet on the condition slide and the answer snippet should be
  recognisably the same code, so the reader can line up the two slides.

## 7. Consistency pitfalls (already hit once)

- **Numbering must agree between code and table.** Task 5's answer table labels
  rows `(1)…(6)` while the code carried the matching `// (1)…(6)` comments; when
  the comments were removed the table was left with `(6)…(11)`, which now matches
  nothing. Whenever you add or drop per-line markers, update both sides — or drop
  the numbering from both.
- **No no-op dereferences.** `*dst++;` discards the dereferenced value and warns
  (`-Wunused-value`); write `dst++;`. Check new slide code with the compiler, not
  just by eye.
- **Column labels and text** in a table should stay short enough for a half-width
  column (~33 characters in the "Причина" column of task 5 is already at the
  limit).
- Don't leave prose that no longer matches the code (a note explaining `4[str]`
  after the third way was removed from the file).

## 8. Repo hygiene

- `.gitignore` covers build artefacts: `*.pdf`, `*.o`, `*.out`, `*.so`, `*.a`,
  `*.bin`, `a.out`, and the `export/` directory. An exported
  `<topic>/presentation.pdf` is kept on disk but ignored.
- Export with `presenterm --export-pdf -o <topic>/presentation.pdf
  <topic>/presentation.md` (and `--export-html`), or several sizes at once with
  `tools/export-scales.sh <topic>/presentation.md`, which names files in
  `export/` after the deck directory.
- Don't break the existing decks: `intro/presentation.md` (`-x -X`),
  `pointers/` and `c-basics/` (`-x`); the rest are exec-free.
- Commits: English, imperative subject, body bullets explaining the "why".
  No `Co-Authored-By` or other attribution trailers.
- `README.md` stays minimal: the directory tree plus a few lines on fonts, the
  theme and typst. Details, measurements and how-tos go into this file.

## 9. Verification workflow — run this before every commit

1. **Compile everything** warning-free and check the outputs still match the
   comments printed on the slides:

   ```sh
   gcc -Wall -Wextra -std=c17 -fsyntax-only */task*.c */task*/*.c intro/*.c
   gcc -Wall -Wextra -std=c17 pointers/task11.c -o /tmp/t11 && /tmp/t11
   ```

2. **Headless render sweep.** presenterm needs a TTY, so drive it through a pty
   of a fixed size, answer the device-attributes query
   (`\x1b[?1;2c\x1b[0n`), send a space per slide, then `q`, and strip the ANSI
   escapes. Helpers are in `tools/`:

   ```sh
   tools/sweep.sh pointers/presentation.md 170 30 -x | less -R
   tools/sweep.sh pointers/presentation.md 120 30 -x
   ```

   Check for: error strings (`error`, `disabled`, `not found`, `no such`,
   `panic`), every `path:` target present, and the footer total (separators + 2).
   Render at two widths — 170 (comfortable) and 120 (narrow, shows column
   wrapping).

3. `--validate-overflows` / `--validate-snippets` require a real terminal and
   fail headless with `io: No such device or address`; don't rely on them in
   automation. `--current-theme` cannot be combined with a `[PATH]`.

## 9a. Typst versions

`compilation/`, `bitops-floats/` and `spare/` also ship `presentation.typ`,
generated by `tools/md2typ.py` from `presentation.md`; code comes from the same
`.c` files via `read()`. Never edit a `.typ` by hand — edit the markdown or the
`.c` file, then run

```sh
tools/build-typst.sh <topic>/presentation.md
```

which regenerates, compiles (`typst compile --root .`, the template is
`themes/slides.typ`) and fails if the page count differs from separators + 2.
An over-tall slide is scaled down as a whole by `fit` in the template rather
than spilling onto a new page. The converter only accepts what the task decks
use and errors on anything else (`+exec`, highlighting, columns), so content
cannot silently disappear. Syntax colours come from
`themes/coldark-cold.tmTheme` (Coldark-Cold as presenterm renders it); check a
rendered page with `tools/check-render-colors.py` after changing them.

## 10. Checklist for "make a presentation with roughly these tasks"

1. Create `<topic>/presentation.md` with the frontmatter from §2 and one
   condition/answer slide pair per task.
2. Create `<topic>/taskNN.c` for every task: complete, standalone, warning-free,
   one statement per line, expected output as trailing comments.
3. Wire the answer slides to the files with `file` snippets; trim with
   `start_line`/`end_line` (hide includes/`main`/trivial definitions) and hide
   nothing by editing a file.
4. Decide exec per slide: default no exec; `file +exec` only when the printed
   output is the answer, and then document the required flag.
5. Run §9: compile, sweep at 170 and 120, diff the rendered answers against the
   comments; for decks with a typst version also run `tools/build-typst.sh` (§9a).
6. Commit in English; if the deck needs a flag at launch, say so in the commit
   body and in `README.md`.

## 11. Theme for a pale projector

`themes/latte-projector.yaml` is a local copy of `catppuccin-latte`: same layout
and element names, palette recomputed for a washed-out projector, where pastel
Latte colours fade and neighbouring hues stop being distinguishable. Complaints
were specifically about the yellow slide title and the green inline code: in
Latte they are 2.31:1 and 2.96:1 against the background, with almost the same
lightness (ΔL\* 7.4), so on a washed-out screen they merged.

Deltas from `catppuccin-latte`:

| Element | Latte | `latte-projector` | Contrast |
| --- | --- | --- | --- |
| Slide title | `df8e1d` | `a15c07` | 2.31:1 → 4.59:1 |
| Inline code, h5, "finished" status | `40a02b` | `0b5f38` | 2.96:1 → 6.86:1 |
| Body text | `4c4f69` | `2b2d3c` | 7.06:1 → 12.03:1 |
| "Running" status | `04a5e5` | `086a94` | 2.47:1 → 5.31:1 |
| Intro subtitle and location | `209fb5` | `7b2ce0` | 2.78:1 → 5.66:1 |
| Code highlighting | `GitHub` | `Coldark-Cold` | comment 2.9:1 → 6.6:1 |

Spacing deltas (0.16.1 has no `margin` key for code blocks, so the empty space
above the first code line is built from two values — the cell padding *inside*
the block and the slide title's bottom padding, which is what makes the gap
*above* the block):

| Element | Latte | `latte-projector` |
| --- | --- | --- |
| `code.padding.vertical` | `1` | `0` |
| `slide_title.padding_bottom` | `1` | `0` |
| `code.alignment` | `center`, `minimum_size: 50`, `minimum_margin: 8%` | `left`, `margin: 3%` |

Code block width: a centered block is only as wide as its longest line, so the
side gaps came from the content, not from `minimum_margin` — lowering it to 2%
changed nothing. Measured on `bitops-floats/presentation.md` (export at 160×64,
`pdftoppm -r 100 -aa no`, edges of the block background): the gaps were 23–28%
of the page width on each side and became 7.5% / 8.4%, and two snippets on one
slide now have the same width instead of each hugging its own content.

Measured on `c-basics/presentation.md` (export at 160×64, `pdftoppm -r 200
-aa no`): the space above the first code line was 92px (1 cell inside the block
+ 2.7 cells of gap) and became 42px (−54%), the first code line moved from
y=175 to y=125 on a 1600px page, and every code block also lost one cell of
bottom padding. Text ink per line is pixel-identical, only shifted.
`execution_output.padding.vertical` deliberately stays `1`, so a `+exec` output
pane still reads as a separate box.

Rules for this theme:

1. The theme's own palette is checked by `tools/check-contrast.py`; it fails the
   run when text drops below 7:1, accents below 4.5:1, or a colour pair that
   shares a slide falls under its ΔE/ΔL\* thresholds.
2. `code.theme_name` is not part of the palette, so syntax highlight colours are
   measured on a rendered page with `tools/check-render-colors.py` (render with
   `pdftoppm -aa no`, otherwise antialiasing invents pale colours). `GitHub`
   came out at 2.9:1 for comments, so `Coldark-Cold` is used instead.
3. Don't tune colours by eye or hard-code a hex in a deck: change the palette,
   then re-run both checkers and a §9 sweep.
4. `theme.name` and `theme.path` cannot be combined, and CLI `--theme` accepts a
   built-in name only — a local theme is always referenced through
   `theme.path`.
5. This is a deliberate deviation from `catppuccin-latte`; decks that need the
   stock look keep `theme.name: catppuccin-latte`. Switching a deck back to
   Latte is a one-line change, including `theme_name: GitHub` inside the theme
   file.
6. Spacing is changed by measurement too: export the deck, rasterise with
   `pdftoppm -r 200 -aa no`, and diff the row profile of the page (top edge of
   the block, first ink row inside it, gap to the slide title) before/after.
   Eyeballing a gap on screen is what produced the 92px hole above the code.
