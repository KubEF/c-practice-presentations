cmake_minimum_required(VERSION 3.16)
project(arena C)

add_compile_options(-Wall -Wextra)

add_executable(app main.c arena.c)
