#!/bin/sh
# Собирает и запускает тесты к слайдам adt/ под ASan и UBSan.
# Задачи 2-5 проверяют list_array.c, задачи 6-9 — list_linked.c.
# Запуск: adt/tests/run.sh [NN ...], без аргументов — все тесты.
set -eu
cd "$(dirname "$0")/.."
out=$(mktemp -d)
trap 'rm -rf "$out"' EXIT
cflags="-Wall -Wextra -Werror -std=c17 -g -fsanitize=address,undefined -fno-sanitize-recover=all"

for t in ${*:-01 02 03 04 05 06 07 08 09}; do
    case $t in
        01)
            gcc -Wall -Wextra -Werror -std=c17 -fsyntax-only tests/test01.c
            echo "test01: ok"
            continue
            ;;
        0[2-5]) impl=list_array.c ;;
        0[6-9]) impl=list_linked.c ;;
        *) echo "unknown test: $t" >&2; exit 2 ;;
    esac
    gcc $cflags "tests/test$t.c" "$impl" -o "$out/test$t"
    "$out/test$t"
done
