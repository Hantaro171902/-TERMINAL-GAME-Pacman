#pragma once

#ifdef _WIN32
// Windows color constants
enum TextColor {
    DEFAULT = 7,
    RED = 4,
    GREEN = 2,
    YELLOW = 6,
    BLUE = 1,
    MAGENTA = 5,
    GRAY = 8,
    CYAN = 3,
    WHITE = 7,
    BRIGHT_RED = 12,
    BRIGHT_GREEN = 10,
    BRIGHT_YELLOW = 14,
    BRIGHT_BLUE = 9,
    BRIGHT_MAGENTA = 13,
    BRIGHT_CYAN = 11,
    BRIGHT_WHITE = 15
};
#else
// ANSI color constants for Linux/Unix
enum TextColor {
    DEFAULT = 0,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    GRAY = 30,
    CYAN = 36,
    WHITE = 37
};
#endif

void setTextColor(TextColor color);
void resetTextColor();
