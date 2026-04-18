cmake_minimum_required(VERSION 3.10)
project(dfa_number_checker LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(dfa main.c dfa.c)
target_compile_options(dfa PRIVATE -Wall -Wextra)