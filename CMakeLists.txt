cmake_minimum_required(VERSION 3.17)
project(mio C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)