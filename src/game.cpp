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
    hideCursor();
    
    char input = 'r';
    while (input == 'r') {
        int level = showTitleScreen();
        clearScreen();
        
        initializeGame(level);
        runGameLoop();
        handleGameEnd();
        
        cout << "(Press r to play again, or any other key to exit.)" << endl;
        cin >> input;
        clearScreen();
    }
    
    cout << "THANKS FOR PLAYING!" << endl;
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
    pacmanThread = thread([this]() {
        while (gameRunning && lives > 0 && dotsEaten < maxDots) {
            resetCursor();
            time++;
            
            // Super mode timeout
            if (superMode && (time - SMtime >= 40)) {
                superMode = false;
                message = "Super mode is now over.";
            }
            
            pacman.update(gameMap, *this);
            
            // Display game
            displayGame();
            
            // Handle input
            if (kbhit()) {
                char input = getch();
                pacman.move(input, gameMap, *this);
            }
            
            this_thread::sleep_for(chrono::milliseconds(150));
        }
    });
    
    // Start ghost threads
    for (auto& ghost : ghosts) {
        ghostThreads.push_back(std::thread([this, &ghost]() {
            while (gameRunning && lives > 0 && dotsEaten < maxDots) {
                ghost.update(pacman.getY(), pacman.getX(), gameMap, *this);
                this_thread::sleep_for(chrono::milliseconds(250));
            }
        }));
    }
    
    // Wait for game to end
    pacmanThread.join();
    for (auto& thread : ghostThreads) {
        thread.join();
    }
    ghostThreads.clear();
}

void Game::displayGame() {
    // Display score and lives
    setTextColor(BRIGHT_RED);
    cout << "    Score: " << score;
    setTextColor(BRIGHT_YELLOW);
    cout << "  Lives: " << lives << " " << endl;
    
    // Display map with colors
    for (int y = 0; y < gameMap.getHeight(); y++) {
        for (int x = 0; x < gameMap.getWidth(); x++) {
            char cell = gameMap.getCell(y, x);
            
            switch(cell) {
                case '#': case ']': case '[':
                    setTextColor(BLUE);
                    break;
                case '<': case '^': case '>': case 'v':
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
                    setTextColor(MAGENTA);
                    break;
                case 'O':
                    setTextColor(BRIGHT_WHITE);
                    break;
                default:
                    setTextColor(WHITE);
                    break;
            }
            cout << cell;
        }
        cout << endl;
    }
    
    // Display message
    setTextColor(YELLOW);
    cout << "[GAME] " << message << "                          " << endl;
}

void Game::handleGameEnd() {
    clearScreen();
    if (dotsEaten == maxDots) {
        showWinScreen();
    } else {
        showGameOverScreen();
    }
}

int Game::showTitleScreen() {
    play_sound("assets/pac_intro.wav");
    
    
    setTextColor(BLUE);
    cout << "       ###############              \n";
    cout << "      ##################        \n";
    cout << "    #######################         \n";
    cout << "  ##############   ###########                  \n";
    cout << "##############################                  \n";
    cout << "#######################                 \n";
    cout << "####################            \n";
    cout << "################            \n";
    cout << "#############               \n";
    cout << "###########                 \n";
    cout << "#########                       \n";
    cout << "############            ";
    cout << "##################          \n";
    cout << "#######################         \n";
    cout << "###########################                 \n";
    cout << "##############################                  \n";
    cout << "    #######################                 \n";
    cout << "      ####################              \n";
    cout << "        ################            " << endl;
    
    setTextColor(BRIGHT_YELLOW);
    cout << "     _____              _____     __  __                _   _ \n";
    cout << "    |  __ \\    /\\      / ____|   |  \\/  |      /\\      | \\ | |\n";
    cout << "    | |__) |  /  \\     | |       | \\  / |     /  \\     |  \\| |\n";
    cout << "    |  ___/  / /\\ \\    | |       | |\\/| |    / /\\ \\    |     |\n";
    cout << "    | |     / ____ \\   | |___    | |  | |   / ____ \\   | |\\  |  \n";
    cout << "    |_|    /_/   \\__\\  \\____|    |_|  |_|  /_/    \\_\\  |_| \\_|" << std::endl;
    cout << "\nPress s to start, q to quit." << endl;
    
    char input = getch();
    cout << " ";
    
    switch(input) {
        case 's':
            cout << "New game starting...\nSelect level (1 or 2): ";
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
    cout << " __     ______  _    _  __          _______ _   _ _ \n";
    cout << " \\ \\   / / __ \\| |  | | \\ \\        / /_   _| \\ | | |\n";
    cout << "  \\ \\_/ / |  | | |  | |  \\ \\  /\\  / /  | | |  \\| | |\n";
    cout << "   \\   /| |  | | |  | |   \\ \\/  \\/ /   | | | . ` | |\n";
    cout << "    | | | |__| | |__| |    \\  /\\  /   _| |_| |\\  |_|\n";
    cout << "    |_|  \\____/ \\____/      \\/  \\/   |_____|_| \\_(_)" << endl;
    cout << "\nCongratulations! \nYOUR SCORE: " << score;
    setTextColor(GREEN);
    cout << "\n\nWould you like to play again?" << endl;
}

void Game::showGameOverScreen() {
    setTextColor(BRIGHT_RED);
    cout << "  _____              __  __   ______      ____  __      __  ______   ____  \n";
    cout << " / ____|     /\\     |  \\/  | |   ___|    / __ \\ \\ \\    / / |  ____| |  __ \\ \n";
    cout << " | |  __    /  \\    | \\  / | |  |__     | |  | | \\ \\  / /  | |__    | |__) |\n";
    cout << " | | |_ |  / /\\ \\   | |\\/| | |  __|     | |  | |  \\ \\/ /   |  __|   |  _  / \n";
    cout << " | |__| | / ____ \\  | |  | | |  |____   | |__| |   \\  /    | |____  | | \\ \\ \n";
    cout << " \\_____/ /_/   \\_ \\ |_|  |_| |______|    \\____/     \\/     |______| |_|  \\_\\" << std::endl;
    setTextColor(GREEN);
    cout << "\nRetry?" << endl;
}
