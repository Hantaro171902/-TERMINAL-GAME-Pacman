#include "game.hpp"
#include "cursor_input.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <csignal>
#include <iostream>

using namespace std;

void cleanup(int signal) {
    restoreTerminalBlocking();
    showCursor();
    exit(signal);
}

void showInfo(const string& arg, const string& programName) {
    if (arg == "-h" || arg == "--help") {
        cout << "Pacman Game - A simple terminal based Pacman implementation" << endl;
        cout << "Usage: " << programName << " [option]" << endl;
        cout << "Options: " << endl;
        cout << "  -h, --help   Show this help message" << endl;
        cout << "  -v, --version Show version information" << endl;
        cout << "\nControls:\n";
        cout << "  W/S or Up/Down - Move Paddle up/down\n";
        cout << "  A/D or Left/Right - Move Paddle left/right\n";
        cout << "  Q - Quit to menu\n";
        cout << "  R - Rotate\n";
    } else if (arg == "-v" || arg == "--version") {
        cout << "Pacman Game v1.0.0\n";
        cout << "A clean, object-oriented implementation of the classic Pacman game\n";
    } else {
        cout << "Unknown option: " << arg << "\n";
        cout << "Use -h or --help for usage information\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        showInfo(argv[1], argv[0]);
        return 0;
    }

    signal(SIGINT, cleanup);    // CTRL + C
    signal(SIGTERM, cleanup);   // kill command

    setTerminalNonBlocking();

    try {
        Game game;
        game.start();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown error occurred" << endl;
        return 1;
    }

    restoreTerminalBlocking();
    showCursor();

    return 0;
}
