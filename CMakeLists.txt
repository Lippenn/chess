cmake_minimum_required(VERSION 3.20)

project(MyApp C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(MyApp
    main.c
)

target_include_directories(MyApp PRIVATE
    ${CMAKE_SOURCE_DIR}/raylib/include
)

target_link_directories(MyApp PRIVATE
    ${CMAKE_SOURCE_DIR}/raylib/lib
)

target_link_libraries(MyApp PRIVATE
    raylib
    opengl32
    gdi32
    winmm
)