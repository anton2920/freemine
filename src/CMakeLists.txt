cmake_minimum_required(VERSION 3.14)
project(src C)

set(CMAKE_C_STANDARD 90)
set(SOURCE_FILES main.c libs/help_routines.c libs/draw.c libs/mines.c libs/menu.c libs/lead.c)
set(HEADER_FILES headers/mines.h)

add_executable(mines ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(mines SDL2 SDL2_image SDL2_ttf SDL2_mixer m STL)