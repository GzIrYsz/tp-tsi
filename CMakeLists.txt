cmake_minimum_required(VERSION 3.30)
project(tp_tsi C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(lib/nrc)

add_subdirectory(src/tp1)
add_subdirectory(src/tp2)
add_subdirectory(src/tp3)