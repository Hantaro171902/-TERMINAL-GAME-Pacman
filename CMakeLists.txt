cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TerminalPacman VERSION 1.0)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and header files
set(SOURCES
    src/main.cpp
    src/ultils.cpp
    src/color.cpp
    src/pacman.cpp
    src/game.cpp
    src/ghost.cpp
    src/map.cpp
)

set(HEADERS
    src/headers/ultils.hpp
    src/headers/color.hpp
    src/headers/pacman.hpp
    src/headers/game.hpp
    src/headers/ghost.hpp
    src/headers/map.hpp
    src/headers/game_forward.hpp
)

# Exe
add_executable(Pacman ${SOURCES} ${HEADERS})

target_include_directories(Pacman PRIVATE src/headers)

# Optional: copy asset folder into build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Set terminal size for better display
add_custom_command(TARGET Pacman POST_BUILD
    COMMAND echo "To run the game properly, resize your terminal to at least 70x32 characters"
    COMMAND echo "You can do this with: stty size 32 70"
)
