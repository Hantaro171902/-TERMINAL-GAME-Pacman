#include "color.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void setTextColor(TextColor color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    SetConsoleTextAttribute(hConsole, color);
#else
    cout << "\033[" << color << "m";
#endif
}

void resetTextColor() {
#ifdef _WIN32
    setTextColor(WHITE);
#else
    cout << "\033[0m";
#endif
}
