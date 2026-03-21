cmake_minimum_required(VERSION 3.10)
project(AeroSoft C)

set(CMAKE_C_STANDARD 23)

add_executable(aerosoft main.c avl.c)

add_executable(tests tests.c avl.c)

enable_testing()
add_test(NAME AVL_Tests COMMAND tests)