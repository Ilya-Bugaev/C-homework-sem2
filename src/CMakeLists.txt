cmake_minimum_required(VERSION 3.10)
project(CityDistribution C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(solution
    main.c
    graph.c
    minHeap.c
)

add_executable(test_runner
    tests.c
)