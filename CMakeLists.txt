cmake_minimum_required(VERSION 3.16)
project(GarbageCollector C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(garbage_collector
    src/main.c
    src/allocator/allocator.c
    src/gc/gc.c
    src/runtime/init.c
)

target_include_directories(garbage_collector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(garbage_collector PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (WIN32)
    target_compile_definitions(garbage_collector PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()