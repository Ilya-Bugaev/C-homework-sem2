cmake_minimum_required(VERSION 3.20)
project(CSV_Parser C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(csv_parser
    main.c
    csv.c
)

add_executable(tests
    tests.c
    csv.c
)

add_custom_target(run_tests
    COMMAND tests
    DEPENDS tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running CSV parser tests..."
)
