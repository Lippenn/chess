cmake_minimum_required(VERSION 3.20)

project(MyApp)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(MyApp
    main.c
)