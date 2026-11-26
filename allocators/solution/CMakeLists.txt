cmake_minimum_required(VERSION 3.16)
project(bump C)

add_compile_options(-Wall -Wextra)

add_executable(app main.c bump.c)
