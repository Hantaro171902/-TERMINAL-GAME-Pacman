#pragma once

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

void setTextColor(TextColor color);
void resetTextColor();
