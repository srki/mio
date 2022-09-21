cmake_minimum_required(VERSION 3.14)
project(mio C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(external)
add_subdirectory(src)

if (DEV)
    add_subdirectory(test)
endif ()