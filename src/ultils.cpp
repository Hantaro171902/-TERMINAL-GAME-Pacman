// ultils.cpp
#include "ultils.hpp"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

string formatTime(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    ostringstream oss;
    oss << setw(2) << setfill('0') << minutes
        << ":" << setw(2) << setfill('0') << seconds;
    return oss.str();
}

void play_sound(const string& path) {
    // Linux: use aplay or similar command
    string command = "aplay -q " + path + " &";
    system(command.c_str());
}

void clearScreen() {
    cout << "\033[2J\033[1;1H"; // Clear screen + move cursor to top-left
}

void clearTerminal() {
    cout << "\033c"; // Full reset
}

void setTextColor(int color) {
    cout << "\033[" << color << "m";
}

void move_cursor(int x, int y) {
    cout << "\033[" << y << ";" << x << "H"; // Move cursor
}


void hideCursor() {
    cout << "\033[?25l";
}

void showCursor() {
    cout << "\033[?25h";
}

void resetCursor() {
    cout << "\033[H";
}

void setCursorPosition(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

bool kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt); // <-- fixed here
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // <-- also fixed here
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

void console_size(int width, int height) {
    // Linux: usually can't resize terminal from code safely
    // Maybe print a warning or leave empty
}

void sleep_ms(int ms) {
    usleep(ms * 1000);
}

int random_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Box drawing characters
const char SYMBOL_HORIZONTAL[]   = "\u2500"; // ─
const char SYMBOL_VERTICAL[]     = "\u2502"; // │
const char SYMBOL_TOP_LEFT[]     = "\u250C"; // ┌
const char SYMBOL_TOP_RIGHT[]    = "\u2510"; // ┐
const char SYMBOL_BOTTOM_LEFT[]  = "\u2514"; // └
const char SYMBOL_BOTTOM_RIGHT[] = "\u2518"; // ┘
const char SYMBOL_T_LEFT[]       = "\u251C"; // ├
const char SYMBOL_T_RIGHT[]      = "\u2524"; // ┤
const char SYMBOL_T_TOP[]        = "\u252C"; // ┬
const char SYMBOL_T_BOTTOM[]     = "\u2534"; // ┴
const char SYMBOL_INTERSECT[]    = "\u253C"; // ┼


// Double line box drawing characters
const char SYMBOL_DOUBLE_HORIZONTAL[]   = "\u2550"; // ═
const char SYMBOL_DOUBLE_VERTICAL[]     = "\u2551"; // ║
const char SYMBOL_DOUBLE_TOP_LEFT[]     = "\u2554"; // ╔
const char SYMBOL_DOUBLE_TOP_RIGHT[]    = "\u2557"; // ╗
const char SYMBOL_DOUBLE_BOTTOM_LEFT[]  = "\u255A"; // ╚    
const char SYMBOL_DOUBLE_BOTTOM_RIGHT[] = "\u255D"; // ╝
const char SYMBOL_DOUBLE_T_LEFT[]       = "\u2560"; // ╠
const char SYMBOL_DOUBLE_T_RIGHT[]      = "\u2563"; // ╣
const char SYMBOL_DOUBLE_T_TOP[]        = "\u2566"; // ╦
const char SYMBOL_DOUBLE_T_BOTTOM[]     = "\u2569"; // ╩
const char SYMBOL_DOUBLE_INTERSECT[]    = "\u256C"; // ╬

const char BLOCK_FULL[]   = "\u2588"; // █
const char BLOCK_HALF[]   = "\u2592"; // ▒