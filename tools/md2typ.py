#!/usr/bin/env python3
"""Generate a typst version of a presenterm deck.

Usage: tools/md2typ.py <topic>/presentation.md [...]

Writes <topic>/presentation.typ next to each deck. The deck stays the single
source of truth: slide text is converted from the markdown, and `file`
snippets become `read("taskNN.c")` calls, so the code still comes from the
.c files. Compile from the repository root (the template lives in themes/):

    typst compile --root . <topic>/presentation.typ <topic>/presentation-typst.pdf

Supported markdown is the subset the task decks use: frontmatter title and
sub_title, setext slide titles, paragraphs, `* ` bullets, inline code, bold,
fenced code blocks and `file` snippets. Anything else is an error, so a deck
cannot silently lose content in the typst version.
"""
import re
import sys
from pathlib import Path

SLIDE_SEP = '<!-- end_slide -->'
IGNORED_COMMENTS = ('<!-- pause -->',)


def tstr(s):
    """A typst string literal."""
    return '"' + s.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n') + '"'


def inline(text):
    """Markdown inline (code spans, bold) -> typst markup made of expressions."""
    out = []
    pos = 0
    for m in re.finditer(r'`([^`]+)`|\*\*(.+?)\*\*', text):
        if m.start() > pos:
            out.append('#' + tstr(text[pos:m.start()]))
        if m.group(1) is not None:
            out.append('#raw(' + tstr(m.group(1)) + ')')
        else:
            out.append('#strong[' + inline(m.group(2)) + ']')
        pos = m.end()
    if pos < len(text):
        out.append('#' + tstr(text[pos:]))
    rest = re.sub(r'`[^`]+`|\*\*.+?\*\*', '', text)
    if '`' in rest or '**' in rest:
        raise ValueError(f'unbalanced inline markup: {text!r}')
    return ''.join(out)


def parse_frontmatter(md):
    if not md.startswith('---\n'):
        raise ValueError('deck has no frontmatter')
    head, body = md[4:].split('\n---\n', 1)
    meta = {}
    for line in head.splitlines():
        m = re.match(r'(title|sub_title):\s*"?(.*?)"?\s*$', line)
        if m:
            meta[m.group(1)] = m.group(2)
    return meta, body


def file_snippet(lines):
    keys = {}
    for line in lines:
        k, _, v = line.partition(':')
        keys[k.strip()] = v.strip()
    unknown = set(keys) - {'path', 'language', 'start_line', 'end_line'}
    if unknown:
        raise ValueError(f'unsupported file snippet keys: {sorted(unknown)}')
    args = [f'read({tstr(keys["path"])})']
    if 'language' in keys:
        args.append(f'lang: {tstr(keys["language"])}')
    if 'start_line' in keys:
        args.append(f'start: {int(keys["start_line"])}')
    if 'end_line' in keys:
        args.append(f'end: {int(keys["end_line"])}')
    return '#code(' + ', '.join(args) + ')'


def convert_slide(text, deck_dir):
    lines = text.strip('\n').splitlines()
    while lines and not lines[0].strip():
        lines.pop(0)
    if len(lines) < 2 or not re.fullmatch(r'=+|-+', lines[1].strip()):
        raise ValueError(f'slide without a setext title: {lines[:2]}')
    title = inline(lines[0].strip())
    body = []
    i = 2
    para = []

    def flush():
        if para:
            body.append(inline(' '.join(para)))
            para.clear()

    while i < len(lines):
        line = lines[i]
        s = line.strip()
        if s.startswith('```'):
            flush()
            info = s[3:].strip()
            lang = info.split()[0] if info else ''
            flags = info.split()[1:]
            if any(f.startswith('+exec') or f.startswith('{') for f in flags):
                raise ValueError(f'unsupported code attributes: {info!r}')
            j = i + 1
            while j < len(lines) and lines[j].strip() != '```':
                j += 1
            if j == len(lines):
                raise ValueError('unterminated code fence')
            content = lines[i + 1:j]
            if lang == 'file':
                snippet = file_snippet(content)
                path = re.search(r'read\("([^"]+)"\)', snippet).group(1)
                if not (deck_dir / path).is_file():
                    raise ValueError(f'file snippet target not found: {path}')
                body.append(snippet)
            else:
                lang_arg = f', lang: {tstr(lang)}' if lang else ''
                body.append(f'#code({tstr(chr(10).join(content))}{lang_arg})')
            i = j + 1
            continue
        if not s:
            flush()
        elif s in IGNORED_COMMENTS:
            flush()
        elif s.startswith('<!--'):
            raise ValueError(f'unsupported presenterm command: {s}')
        elif re.match(r'[*-] ', s):
            flush()
            body.append('- ' + inline(s[2:]))
        else:
            para.append(s)
        i += 1
    flush()
    return f'#slide([{title}])[\n' + '\n\n'.join(body) + '\n]\n'


def convert(md_path):
    md_path = Path(md_path)
    meta, body = parse_frontmatter(md_path.read_text())
    slides = [s for s in body.split(SLIDE_SEP) if s.strip()]
    template = Path('themes/slides.typ').resolve()
    rel = Path(*(['..'] * len(md_path.parent.resolve().relative_to(Path.cwd().resolve()).parts)),
               'themes', 'slides.typ')
    if not template.is_file():
        raise SystemExit('run from the repository root: themes/slides.typ not found')
    out = [
        f'// Сгенерировано tools/md2typ.py из {md_path.name} — правьте markdown, не этот файл.',
        f'#import "{rel.as_posix()}": *',
        '',
        f'#show: deck.with(title: {tstr(meta.get("title", ""))}, '
        f'subtitle: {tstr(meta.get("sub_title", ""))})',
        '',
    ]
    for n, s in enumerate(slides, 1):
        try:
            out.append(convert_slide(s, md_path.parent))
        except ValueError as e:
            raise SystemExit(f'{md_path}: slide {n}: {e}')
    typ = md_path.with_suffix('.typ')
    typ.write_text('\n'.join(out))
    print(f'{typ}: {len(slides)} slides')


if __name__ == '__main__':
    if len(sys.argv) < 2:
        raise SystemExit(__doc__)
    for p in sys.argv[1:]:
        convert(p)
