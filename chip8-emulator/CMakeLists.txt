cmake_minimum_required(VERSION 3.20)

project(chip8_emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(chip8
    src/main.cpp
    src/chip8.cpp
)