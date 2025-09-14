cmake_minimum_required(VERSION 3.10)

project(TerminalPacman VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

# Option to enable sanitizers for debugging
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" ON)

# Source and header lists
set(SOURCES
    src/main.cpp
    src/ultils.cpp
    src/color.cpp
    src/pacman.cpp
    src/game.cpp
    src/ghost.cpp
    src/map.cpp
    src/cursor_input.cpp
)

set(HEADERS
    src/headers/ultils.hpp
    src/headers/color.hpp
    src/headers/pacman.hpp
    src/headers/game.hpp
    src/headers/ghost.hpp
    src/headers/map.hpp
    src/headers/game_forward.hpp
    src/headers/cursor_input.hpp
)

add_executable(Pacman ${SOURCES} ${HEADERS})
target_include_directories(Pacman PRIVATE src/headers)

# Link pthreads properly
find_package(Threads REQUIRED)
target_link_libraries(Pacman PRIVATE Threads::Threads)

# Debug flags by default in Debug build
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Pacman PRIVATE -g -O0)
endif()

# If user enabled sanitizers, add flags (useful while hunting segfaults / UB)
if (ENABLE_SANITIZERS)
    message(STATUS "Sanitizers enabled: AddressSanitizer + UBSan")
    # compile flags
    target_compile_options(Pacman PRIVATE -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer)
    # link flags (some CMake versions need link flags explicitly)
    target_link_libraries(Pacman PRIVATE -fsanitize=address -fsanitize=undefined)
endif()

# Optional: copy asset folder into build dir
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Print a friendly post build hint
add_custom_command(TARGET Pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Build finished. Resize your terminal to at least 70x32 for best display."
    COMMAND ${CMAKE_COMMAND} -E echo "Build dir: ${CMAKE_BINARY_DIR}"
)
