set(CMAKE_C_STANDARD 99)

find_package(SDL2 REQUIRED)

add_executable(FreeMine main.c help_routines.c draw.c mines.c menu.c lead.c)

target_include_directories(FreeMine PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(FreeMine PRIVATE ${SDL2_LIBRARIES})
