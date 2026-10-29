// Шаблон слайдов для typst-версий колод. Палитра — та же, что в
// themes/latte-projector.yaml (бледный проектор), фон блока кода — от
// Coldark-Cold. Колоды presentation.typ не пишутся руками: их собирает
// tools/md2typ.py из presentation.md, код берётся из тех же .c файлов.

#let palette = (
  base: rgb("#eff1f5"),
  text: rgb("#2b2d3c"),
  title: rgb("#a15c07"),      // yellow: заголовок слайда
  inline: rgb("#0b5f38"),     // green: inline-код, заголовок колоды
  subtitle: rgb("#7b2ce0"),   // mauve: подзаголовок колоды
  muted: rgb("#5c5f75"),      // overlay2: номера строк, номер слайда
  code-bg: rgb("#e3eaf2"),
)

#let body-size = 17pt
#let code-size = 0.88em   // относительно текста: слайд уменьшается целиком

#let deck(title: none, subtitle: none, body) = {
  set document(title: title)
  set page(
    paper: "presentation-16-9",
    fill: palette.base,
    margin: (x: 1.4cm, top: 1.0cm, bottom: 1.2cm),
    footer: context {
      set text(size: 11pt, fill: palette.muted)
      align(right)[#counter(page).display() / #counter(page).final().first()]
    },
  )
  set text(font: "Noto Sans", lang: "ru", size: body-size, fill: palette.text)
  set par(justify: false, leading: 0.55em, spacing: 0.9em)
  set list(marker: [•], indent: 0.4em, body-indent: 0.5em, spacing: 0.75em)
  show raw: set text(font: "JetBrainsMono NFM")
  set raw(theme: "coldark-cold.tmTheme")
  show raw.where(block: false): set text(fill: palette.inline, size: 1.0em)
  show strong: set text(weight: "bold")

  // титульный слайд
  align(center + horizon)[
    #text(size: 2.2em, weight: "bold", fill: palette.inline, title)
    #v(0.4em)
    #text(size: 1.3em, fill: palette.subtitle, subtitle)
  ]
  body
}

// Если слайд выше страницы, весь текст на нём уменьшается пропорционально
// (код задан в em и уменьшается вместе с ним) — вместо переноса на новую
// страницу, который сломал бы нумерацию слайдов.
#let fit(content) = layout(size => {
  let render(k) = block(width: size.width, {
    set text(size: body-size * k)
    content
  })
  let k = 1.0
  for _ in range(4) {
    let h = measure(render(k)).height
    if h <= size.height { break }
    k = k * size.height / h * 0.98
  }
  render(k)
})

#let slide(title, body) = {
  pagebreak()
  fit({
    align(center, text(size: 1.25em, weight: "bold", fill: palette.title, title))
    v(0.5em)
    body
  })
}

// Блок кода с номерами строк. src — полный текст файла или сниппета,
// start/end — номера строк файла (включительно), как start_line/end_line
// в presenterm; нумерация фрагмента, как и там, начинается с 1.
#let code(src, lang: none, start: none, end: none) = {
  let lines = src.split("\n")
  if lines.len() > 0 and lines.last() == "" {
    lines = lines.slice(0, -1)
  }
  let first = if start == none { 1 } else { start }
  let last = if end == none { lines.len() } else { end }
  let fragment = lines.slice(first - 1, last).join("\n")
  let width = str(last - first + 1).len()
  block(
    fill: palette.code-bg,
    width: 100%,
    inset: (x: 0.8em, y: 0.55em),
    radius: 3pt,
    below: 0.9em,
  )[
    #set text(size: code-size)
    #set par(leading: 0.42em)
    #show raw.line: it => {
      box(width: (width + 0.5) * 0.6em, align(right, text(fill: palette.muted, str(it.number))))
      h(0.9em)
      it.body
    }
    #raw(fragment, lang: lang, block: true)
  ]
}
