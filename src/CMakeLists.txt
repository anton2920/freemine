cmake_minimum_required(VERSION 3.14)
project(src C)

set(CMAKE_C_STANDARD 90)
set(SOURCE_FILES main.c libs/help_routines.c)
set(HEADER_FILES headers/mines.h)

add_executable(src ${HEADER_FILES} ${SOURCE_FILES})