set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(FreeMine main.c help_routines.c draw.c mines.c menu.c lead.c)

target_include_directories(FreeMine PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(FreeMine PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
