cmake_minimum_required(VERSION 3.14)
project(Rotate_SoLE C)

set(CMAKE_C_STANDARD 99)

add_executable(Rotate_SoLE lss_26_09.h lss_26_09.c main_lss_26_09.c)
target_link_libraries(Rotate_SoLE m)
