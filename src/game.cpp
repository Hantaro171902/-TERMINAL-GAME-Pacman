#include "game.hpp"
#include "map.hpp"
#include "pacman.hpp"
#include "ghost.hpp"
#include "ultils.hpp"
#include "color.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

Game::Game() : score(0), lives(3), time(0), SMtime(0), dotsEaten(0), maxDots(0), 
               superMode(false), message("Round start!"), gameRunning(false) {
    // Initialize ghosts
    ghosts.push_back(Ghost(GhostType::BLINKY, 9, 12, 250));
    ghosts.push_back(Ghost(GhostType::PINKY, 9, 14, 250));
    ghosts.push_back(Ghost(GhostType::INKY, 10, 12, 450));
    ghosts.push_back(Ghost(GhostType::CLYDE, 10, 14, 150));
}

Game::~Game() {
    stop();
}

void Game::start() {
    clearTerminal();
    hideCursor();
    
    char input = 'r';
    while (input == 'r') {
        int level = showTitleScreen();
        clearScreen();
        
        initializeGame(level);
        runGameLoop();
        handleGameEnd();
        
          // after handleGameEnd():
        setTextColor(BRIGHT_CYAN);
        cout << "(Press 'r' to play again, any other key to exit.)" << endl;

        // wait for a single keypress and use it
        while (!kbhit()) std::this_thread::sleep_for(std::chrono::milliseconds(50));
        input = getch();
        clearScreen();
    }
    
    cout << "THANKS FOR PLAYING!" << endl;
    showCursor();
}

void Game::stop() {
    gameRunning = false;
    if (pacmanThread.joinable()) {
        pacmanThread.join();
    }
    for (auto& thread : ghostThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    ghostThreads.clear();
}

bool Game::isRunning() const {
    return gameRunning;
}

void Game::initializeGame(int level) {
    gameMap.loadLevel(level);
    maxDots = gameMap.getMaxDots();
    
    // Reset game state
    score = 0;
    lives = 3;
    time = 0;
    SMtime = 0;
    dotsEaten = 0;
    superMode = false;
    message = "Round start!";
    
    // Reset characters
    pacman.reset();
    for (auto& ghost : ghosts) {
        ghost.reset();
    }
    
    gameRunning = true;
}

void Game::runGameLoop() {
    // Start threads
    // pacman thread
    pacmanThread = thread([this]() {
        while (gameRunning && lives > 0 && dotsEaten < maxDots) {
            {
                lock_guard<mutex> lock(gameMutex); // protect everything below
                resetCursor();
                ++time;

                if (superMode && (time - SMtime >= 40)) {
                    superMode = false;
                    message = "Super mode is now over.";
                }

                pacman.update(gameMap, *this);   // map + state changes protected
                displayGame();                   // read map + other state while locked

                // If Pacman died but lives remain, respawn characters
                if (!pacman.isAlive() && lives > 0) {
                    superMode = false;
                    pacman.reset();
                    for (auto& ghost : ghosts) ghost.reset();
                }
            } // unlock here before sleeping / waiting for input

            // Handle input (doesn't need map lock)
            if (kbhit()) {
                char input = getch();
                // move modifies pacman and might need lock depending on your implementation
                lock_guard<std::mutex> lock(gameMutex);
                pacman.move(input, gameMap, *this);
            }

            this_thread::sleep_for(chrono::milliseconds(150));
        }
    });
    
    // Start ghost threads
    ghostThreads.reserve(ghosts.size());

    // spawn one thread per ghost — capture the index, not the loop variable reference
    for (size_t i = 0; i < ghosts.size(); ++i) {
        ghostThreads.emplace_back([this, i]() {
            while (gameRunning && lives > 0 && dotsEaten < maxDots) {
                {
                    lock_guard<mutex> lock(gameMutex);
                    Ghost &ghost = ghosts[i];
                    ghost.update(pacman.getY(), pacman.getX(), gameMap, *this);
                } // unlock quickly
                this_thread::sleep_for(chrono::milliseconds(250));
            }
        });
    }

        // Wait for game to end
    pacmanThread.join();
    for (auto& thread : ghostThreads) {
        thread.join();
    }
    ghostThreads.clear();
}


void Game::displayGame() {
    // Score + Lives (hearts)
    setTextColor(BRIGHT_RED);
    cout << "  Score: " << score;

    setTextColor(BRIGHT_YELLOW);
    cout << "  Lives: ";

    // print 3 hearts (solid if present, empty if lost)
    const int MAX_LIVES = 3;
    for (int i = 0; i < MAX_LIVES; ++i) {
        if (i < lives) {
            cout << HEART_SOLID << " ";
        } else {
            cout << HEART_EMPTY << " ";
        }
    }
    cout << endl;

    // Build a local char buffer copy of the map (so we can overlay dynamic characters)
    int h = gameMap.getHeight();
    int w = gameMap.getWidth();
    vector<string> buffer;
    buffer.assign(h, std::string(w, ' '));
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            buffer[y][x] = gameMap.getCell(y, x);
        }
    }

    // overlay pacman
    int py = pacman.getY();
    int px = pacman.getX();
    if (py >= 0 && py < h && px >= 0 && px < w) {
        buffer[py][px] = pacman.getChar(); // e.g. '<' or whatever pacman uses
    }

    // overlay ghosts (use their char code: 'M','Y','W','U' or whatever your Ghost::getChar() returns)
    for (auto &g : ghosts) {
        int gy = g.getY();
        int gx = g.getX();
        if (gy >= 0 && gy < h && gx >= 0 && gx < w) {
            buffer[gy][gx] = g.getChar();
        }
    }

    // Now render the buffer row-by-row. Use BLOCK_FULL string for walls, otherwise print the single char.
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            char cellChar = buffer[y][x];

            // choose color based on character
            switch(cellChar) {
                case '#':
                    setTextColor(BLUE);
                    break;
                case '<': case '^': case '>': case 'v': // pacman glyphs
                    setTextColor(BRIGHT_YELLOW);
                    break;
                case 'M':
                    setTextColor(superMode ? WHITE : BRIGHT_RED);
                    break;
                case 'Y':
                    setTextColor(superMode ? WHITE : BRIGHT_GREEN);
                    break;
                case 'W':
                    setTextColor(superMode ? WHITE : BRIGHT_MAGENTA);
                    break;
                case 'U':
                    setTextColor(superMode ? WHITE : BRIGHT_CYAN);
                    break;
                case '.':
                    setTextColor(YELLOW);
                    break;
                case 'O':
                    setTextColor(BRIGHT_WHITE);
                    break;
                case '[': case ']':
                    setTextColor(CYAN);
                    break;
                default:
                    setTextColor(WHITE);
                    break;
            }

            // print glyph; walls use BLOCK_FULL (multi-byte), others print the char
            if (cellChar == '#') {
                cout << BLOCK_FULL;
            } else {
                // print single-char items (pacman, ghosts, dots, fruit)
                cout << cellChar;
            }
        }
        cout << endl;
    }

    // Show message line
    setTextColor(YELLOW);
    cout << "[GAME] " << message << "                          " << endl;
}


void Game::handleGameEnd() {
    // Make sure all threads are stopped, then clear and show the summary at top-left
    stop();             // joins threads if not already joined
    clearScreen();      // move to top-left
    showCursor();       // show cursor so the player sees the prompt

    if (dotsEaten == maxDots) {
        showWinScreen();
    } else {
        showGameOverScreen();
    }

    // Prompt and wait for a single key (use your kbhit/getch helpers)
    setTextColor(BRIGHT_YELLOW);
    cout << "\n(Press any key to continue...)" << endl;

    // Wait for a keypress (works with raw mode)
    while (!kbhit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    char c = getch(); // consume the key so subsequent logic isn't messed up
    (void)c;
}


int Game::showTitleScreen() {
    // play_sound("assets/pac_intro.wav");
    
    setTextColor(BRIGHT_YELLOW);
    cout << R"(
     _____ _____ _____ _____ _____ _____ 
    |  _  |  _  |     |     |  _  |   | |
    |   __|     |   --| | | |     | | | |
    |__|  |__|__|_____|_|_|_|__|__|_|___|
    )";
    
    char input = getch();
    cout << " ";
    
    switch(input) {
        case 's':
            cout << "New game starting..." << endl;
            cout << "Select level (1 or 2): ";
            int level;
            cin >> level;
            return level;
        case 'q':
            exit(0);
        default:
            return 1;
    }
}

void Game::showWinScreen() {
    setTextColor(BRIGHT_YELLOW);
    cout << R"(
         __ __ _____ _____    _ _ _ _____ _____ 
        |  |  |     |  |  |  | | | |     |   | |
        |_   _|  |  |  |  |  | | | |  |  | | | |
          |_| |_____|_____|  |_____|_____|_|___|
    )";
    cout << endl;
    cout << "Congratulations! ";
    cout << "YOUR SCORE: " << score;
    setTextColor(GREEN);
    cout << "\nYou completed the game!" << endl;
    cout << "Would you like to play again?" << endl;
}

void Game::showGameOverScreen() {
    setTextColor(BRIGHT_RED);
    cout << R"(
     _____ _____ _____ _____    _____ _____ _____ _____ 
    |   __|  _  |     |   __|  |     |  |  |   __| __  |
    |  |  |     | | | |   __|  |  |  |  |  |   __|    -|
    |_____|__|__|_|_|_|_____|  |_____|\___/|_____|__|__|
    
    )";  
    cout << endl;
    setTextColor(GREEN);
    cout << "Retry?" << endl;
}
