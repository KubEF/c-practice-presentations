---
title: "Практика по C"
sub_title: "Абстрактные типы данных: список"
theme:
  path: ../themes/latte-projector.yaml
  override:
    code:
      line_numbers: true
---

Задача 1. Интерфейс списка
==========================

Создайте заголовок `list.h` для АТД «список целых чисел». Только объявления: реализаций будет две, и обе подключат этот файл.

| Функция | Что делает |
| --- | --- |
| `list_create` | создаёт пустой список, без памяти — `NULL` |
| `list_destroy` | освобождает список, `NULL` допустим |
| `list_size` | возвращает число элементов |
| `list_get`, `list_set` | читает и записывает элемент `i` |
| `list_push_back` | добавляет элемент в конец |
| `list_insert` | вставляет элемент в позицию `i` |
| `list_remove` | удаляет элемент `i` |

* **а)** Скройте устройство списка: пользователь хранит `list *`, но не видит полей
* **б)** Напишите прототипы функций. Подумайте, где надо использовать`const`

<!-- end_slide -->

Задача 1. Ответ (1/2)
=====================

```file
path: list.h
language: c
start_line: 4
end_line: 16
```

<!-- end_slide -->

Задача 1. Ответ (2/2)
=====================

```file
path: list.h
language: c
start_line: 18
end_line: 27
```

<!-- end_slide -->

Задача 2. Список на массиве: создание
=====================================

Создайте `list_array.c`, подключите `list.h` и опишите структуру:

```c
struct list {
    int *data;
    size_t size;
    size_t capacity;
};
```

* **а)** Реализуйте `list_create`: новый список пуст и не выделяет память под элементы; при нехватке памяти — `NULL`
* **б)** Реализуйте `list_destroy`; вызов `list_destroy(NULL)` допустим
* **в)** Реализуйте `list_size`

<!-- end_slide -->

Задача 2. Ответ (1/2)
=====================

```file
path: list_array.c
language: c
start_line: 12
end_line: 21
```

<!-- end_slide -->

Задача 2. Ответ (2/2)
=====================

```file
path: list_array.c
language: c
start_line: 23
end_line: 33
```

<!-- end_slide -->

Задача 3. Список на массиве: доступ по индексу
==============================================

Напишите в `list_array.c`:

```c
int list_get(const list *l, size_t i);
void list_set(list *l, size_t i, int value);
```

* **а)** Реализуйте обе функции за `O(1)`
* **б)** Напишите в `assert`-ах ограничения на индексы

<!-- end_slide -->

Задача 3. Ответ
===============

```file
path: list_array.c
language: c
start_line: 35
end_line: 43
```

<!-- end_slide -->

Задача 4. Список на массиве: добавление в конец
===============================================

Напишите в `list_array.c`:

```c
static bool reserve_one(list *l);
bool list_push_back(list *l, int value);
```

* **а)** Реализуйте `reserve_one`: после успешного вызова в массиве есть место ещё под один элемент; ёмкость растёт вдвое, начиная с 4
* **б)** При нехватке памяти функции возвращают `false`, а список остаётся прежним
* **в)** Реализуйте `list_push_back` через `reserve_one`

<!-- end_slide -->

Задача 4. Ответ (1/2)
=====================

```file
path: list_array.c
language: c
start_line: 45
end_line: 58
```

<!-- end_slide -->

Задача 4. Ответ (2/2)
=====================

```file
path: list_array.c
language: c
start_line: 60
end_line: 67
```

<!-- end_slide -->

Задача 5. Список на массиве: вставка и удаление
===============================================

Напишите в `list_array.c`:

```c
bool list_insert(list *l, size_t i, int value);
void list_remove(list *l, size_t i);
```

* **а)** Реализуйте `list_insert`: элементы с позиции `i` сдвигаются вправо; при нехватке памяти — `false`, список не меняется
* **б)** Реализуйте `list_remove`: элементы после позиции `i` сдвигаются влево
* **в)** Сдвигайте одним вызовом библиотечной функции `memmove` (посмотрите через `man` его сигнатуру), корректной для пересекающихся областей

<!-- end_slide -->

Задача 5. Ответ (1/2)
=====================

```file
path: list_array.c
language: c
start_line: 69
end_line: 79
```

<!-- end_slide -->

Задача 5. Ответ (2/2)
=====================

```file
path: list_array.c
language: c
start_line: 81
end_line: 86
```

<!-- end_slide -->

Задача 6. Связный список: создание
==================================

Вторая реализация того же `list.h` — файл `list_linked.c`:

```c
struct node {
    int value;
    struct node *next;
};

struct list {
    struct node *head;
    struct node *tail;
    size_t size;
};
```

* **а)** Реализуйте `list_create` для этого представления
* **б)** Реализуйте `list_size` за `O(1)`
* **в)** Реализуйте `list_destroy`

<!-- end_slide -->

Задача 6. Ответ (1/2)
=====================

```file
path: list_linked.c
language: c
start_line: 16
end_line: 25
```

```file
path: list_linked.c
language: c
start_line: 40
end_line: 42
```

<!-- end_slide -->

Задача 6. Ответ (2/2)
=====================

```file
path: list_linked.c
language: c
start_line: 27
end_line: 38
```

<!-- end_slide -->

Задача 7. Связный список: доступ по индексу
===========================================

Напишите `list_get` и `list_set` для `list_linked.c`.

* **а)** Напишите `static struct node *node_at(const list *l, size_t i)`, возвращающую узел с индексом `i`
* **б)** Реализуйте через неё `list_get` и `list_set` с проверкой индекса

<!-- end_slide -->

Задача 7. Ответ (1/2)
=====================

```file
path: list_linked.c
language: c
start_line: 44
end_line: 51
```

<!-- end_slide -->

Задача 7. Ответ (2/2)
=====================

```file
path: list_linked.c
language: c
start_line: 53
end_line: 61
```

<!-- end_slide -->

Задача 8. Связный список: добавление в конец
============================================

Реализуйте `list_push_back` для `list_linked.c` за `O(1)`, без обхода списка.

* **а)** Напишите `static struct node *node_new(int value, struct node *next)`: новый узел или `NULL` при нехватке памяти
* **б)** Реализуйте `list_push_back` с помощью поля `tail`
* **в)** Добавление в пустой список тоже должно работать: `head` и `tail` всегда согласованы

<!-- end_slide -->

Задача 8. Ответ (1/2)
=====================

```file
path: list_linked.c
language: c
start_line: 63
end_line: 71
```

<!-- end_slide -->

Задача 8. Ответ (2/2)
=====================

```file
path: list_linked.c
language: c
start_line: 73
end_line: 86
```

<!-- end_slide -->

Задача 9. Связный список: вставка и удаление
============================================

Напишите `list_insert` и `list_remove` для `list_linked.c`.

* **а)** Реализуйте `list_insert` для `i` от `0` до `size` и `list_remove` для `i` от `0` до `size - 1`; при нехватке памяти вставка возвращает `false`, список не меняется
* **б)** После любой операции `tail` указывает на последний узел — проверьте вставку в конец и удаление последнего и единственного узла
* **в)** Обойдитесь без отдельной ветки для головы: работайте через `struct node **` — адрес указателя, который нужно изменить

<!-- end_slide -->

Задача 9. Ответ (1/2)
=====================

```file
path: list_linked.c
language: c
start_line: 88
end_line: 102
```

<!-- end_slide -->

Задача 9. Ответ (2/2)
=====================

```file
path: list_linked.c
language: c
start_line: 104
end_line: 115
```

<!-- end_slide -->

Задача 10. Сложность операций
=============================

Заполните таблицу для двух реализаций; `n` — длина списка, `i` — индекс.

| Операция | Массив | Связный список |
| --- | --- | --- |
| `list_size` | ? | ? |
| `list_get`, `list_set` | ? | ? |
| `list_push_back` | ? | ? |
| вставка/удаление в начале | ? | ? |
| вставка/удаление по индексу `i` | ? | ? |
| `list_destroy` | ? | ? |

<!-- end_slide -->

Задача 10. Ответ
================

| Операция | Массив | Связный список |
| --- | --- | --- |
| `list_size` | `O(1)` | `O(1)` |
| `list_get`, `list_set` | `O(1)` | `O(i)` |
| `list_push_back` | `O(1)` амортизированно | `O(1)` |
| вставка/удаление в начале | `O(n)` | `O(1)` |
| вставка/удаление по индексу `i` | `O(n − i)` | `O(i)` |
| `list_destroy` | `O(1)` | `O(n)` |
