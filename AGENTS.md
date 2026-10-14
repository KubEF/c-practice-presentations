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
presentation.md            intro/demo deck (exec demos), requires -x -X
code/                      demo programs for the intro deck
pointers-presentation.md   task deck: condition slide + answer slide per task
pointers/taskNN.c          one file per task — single source of truth for code
tools/                     headless render helpers (see §9)
AGENTS.md                  this file
```

- One deck per topic, named `<topic>-presentation.md`; its task files live in a
  sibling `<topic>/` directory.
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
     name: catppuccin-latte
     override:
       code:
         line_numbers: true
   ---
   ```

   The override is applied to every snippet by `push_code`, so it belongs here
   and not on individual snippets. `author` is optional.

2. **All code lives in `.c` files**, never inline on answer slides. Every file
   must compile warning-free and be a standalone program:

   ```sh
   gcc -Wall -Wextra -std=c17 -fsyntax-only pointers/task01.c
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

7. **Don't touch the theme beyond `theme.override`.** Margins/padding/fonts live
   in the theme; earlier requests to change margins were declined.

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
path: pointers/task01.c
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
path: pointers/task04.c
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
  paths like `pointers/task01.c` work from any launch directory.
- **`start_line`/`end_line` cut the snippet before it executes.** A C snippet
  that shows only lines 6-9 of a file will not compile. To run a whole file while
  showing a fragment, keep the fragment as a plain `file` snippet and let a
  neighbouring `bash +exec_replace +no_background` snippet compile the real file:

  ````markdown
  ```bash +exec_replace +no_background
  gcc -Wall -Wextra -std=c17 code/dynamic_array.c -o /tmp/dynamic_array
  /tmp/dynamic_array
  ```
  ````

- Task decks are **exec-free by default** (expected output lives in comments).
  The current pointers deck makes an exception for four tasks (1, 6, 8, 10) whose
  printed values *are* the answer: those use `file +exec`, so that deck must be
  run with `presenterm -x pointers-presentation.md`. The intro deck
  `presentation.md` needs `-x -X`.
- Without the right flag presenterm shows `snippet +exec is disabled, run with -x
  to enable` — mention the required flag in the README/prompt when a deck uses it.

## 5. Columns

- Integer ratios only: `<!-- column_layout: [1, 1] -->`, `[3, 2]`, … Fractional
  values (`[1, 1.5]`) are rejected — `columns` is `&[u8]`.
- Each column is introduced with `<!-- column: N -->`, and the layout is closed
  with `<!-- reset_layout -->`.
- The theme gives code blocks `minimum_size: 50` columns and 8% margins, so a
  code column never gets narrower than ~50 — put the wider content (tables,
  prose) in the other column, and expect presenterm to wrap long table cells
  inside a column instead of shrinking them (this is accepted).

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
  `*.bin`, `a.out`. An exported `presentation.pdf` is kept on disk but ignored.
- Export with `presenterm --export-pdf -o presentation.pdf <deck>.md` (and
  `--export-html`).
- Don't break the existing decks: `presentation.md` (intro, `-x -X`) and
  `pointers-presentation.md` (task deck, currently `-x`).
- Commits: English, imperative subject, body bullets explaining the "why".

## 9. Verification workflow — run this before every commit

1. **Compile everything** warning-free and check the outputs still match the
   comments printed on the slides:

   ```sh
   gcc -Wall -Wextra -std=c17 -fsyntax-only pointers/*.c code/*.c
   gcc -Wall -Wextra -std=c17 pointers/task11.c -o /tmp/t11 && /tmp/t11
   ```

2. **Headless render sweep.** presenterm needs a TTY, so drive it through a pty
   of a fixed size, answer the device-attributes query
   (`\x1b[?1;2c\x1b[0n`), send a space per slide, then `q`, and strip the ANSI
   escapes. Helpers are in `tools/`:

   ```sh
   tools/sweep.sh pointers-presentation.md 170 30 -x | less -R
   tools/sweep.sh pointers-presentation.md 120 30 -x
   ```

   Check for: error strings (`error`, `disabled`, `not found`, `no such`,
   `panic`), every `path:` target present, and the footer total (separators + 2).
   Render at two widths — 170 (comfortable) and 120 (narrow, shows column
   wrapping).

3. `--validate-overflows` / `--validate-snippets` require a real terminal and
   fail headless with `io: No such device or address`; don't rely on them in
   automation. `--current-theme` cannot be combined with a `[PATH]`.

## 10. Checklist for "make a presentation with roughly these tasks"

1. Create `<topic>-presentation.md` with the frontmatter from §2 and one
   condition/answer slide pair per task.
2. Create `<topic>/taskNN.c` for every task: complete, standalone, warning-free,
   one statement per line, expected output as trailing comments.
3. Wire the answer slides to the files with `file` snippets; trim with
   `start_line`/`end_line` (hide includes/`main`/trivial definitions) and hide
   nothing by editing a file.
4. Decide exec per slide: default no exec; `file +exec` only when the printed
   output is the answer, and then document the required flag.
5. Run §9: compile, sweep at 170 and 120, diff the rendered answers against the
   comments.
6. Commit in English; if the deck needs a flag at launch, say so in the commit
   body and in `README.md`.
