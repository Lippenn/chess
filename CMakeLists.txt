cmake_minimum_required(VERSION 3.20)

project(Chess C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Chess
    src/main.c
    src/textures.c
    src/board.c
    src/draw.c
    src/game.c
)

target_include_directories(Chess PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/raylib/include
)

target_link_directories(Chess PRIVATE
    ${CMAKE_SOURCE_DIR}/raylib/lib
)

target_link_libraries(Chess PRIVATE
    raylib
    opengl32
    gdi32
    winmm
)