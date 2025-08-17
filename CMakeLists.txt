cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TerminalMineSweeper VERSION 1.0)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and header files
set(SOURCES
    src/main.cpp
    src/ultils.cpp
    src/minesweeper.cpp
    src/color.cpp
    src/pacman.cpp
    src/game.cpp
    src/ghost.cpp
    src/map.hpp
)

set(HEADERS
    src/headers/ultils.hpp
    src/headers/minesweeper.hpp
    src/headers/color.hpp
)

# Exe
add_executable(MineSweeper ${SOURCES} ${HEADERS})

target_include_directories(MineSweeper PRIVATE src/headers)

# Optional: copy asset folder into build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
