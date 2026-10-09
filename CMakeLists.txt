cmake_minimum_required(VERSION 3.20)

project(Chess C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Chess
    src/main.c
    src/textures.c
    src/draw.c
    src/board.c
    src/fen.c
    src/game.c
    src/moves.c
)


target_include_directories(Chess PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/raylib/include
)

target_link_directories(Chess PRIVATE
    ${CMAKE_SOURCE_DIR}/raylib/lib
)

target_compile_definitions(Chess PRIVATE
    ROOT_PATH="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(Chess PRIVATE
    raylib
    opengl32
    gdi32
    winmm
)

# --------------------
# Tests
# --------------------

add_executable(ChessTests
    src/textures.c
    src/draw.c
    src/board.c
    src/fen.c
    src/game.c
    src/moves.c

    tests/src/test_main.c
    tests/src/test_game.c
    tests/src/test_moves.c
    tests/src/test_fen.c
)

target_include_directories(ChessTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/raylib/include
)
target_link_directories(ChessTests PRIVATE
    ${CMAKE_SOURCE_DIR}/raylib/lib
)

target_link_directories(ChessTests PRIVATE
    ${CMAKE_SOURCE_DIR}/raylib/lib
)


target_compile_definitions(ChessTests PRIVATE
    ROOT_PATH="${CMAKE_SOURCE_DIR}"
)


target_link_libraries(ChessTests PRIVATE
    raylib
    opengl32
    gdi32
    winmm
)