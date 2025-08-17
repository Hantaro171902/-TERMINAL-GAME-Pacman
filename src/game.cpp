#include "headers/game.hpp"
#include "headers/map.hpp"
#include "headers/pacman.hpp"
#include "headers/ghost.hpp"
#include "headers/ultils.hpp"
#include "headers/color.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <mmsystem.h>

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
        
        std::cout << "(Press r to play again, or any other key to exit.)" << std::endl;
        std::cin >> input;
        clearScreen();
    }
    
    std::cout << "THANKS FOR PLAYING!" << std::endl;
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
    pacmanThread = std::thread([this]() {
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
            
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    });
    
    // Start ghost threads
    for (auto& ghost : ghosts) {
        ghostThreads.push_back(std::thread([this, &ghost]() {
            while (gameRunning && lives > 0 && dotsEaten < maxDots) {
                ghost.update(pacman.getY(), pacman.getX(), gameMap, *this);
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
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
    std::cout << "    Score: " << score;
    setTextColor(BRIGHT_YELLOW);
    std::cout << "  Lives: " << lives << " " << std::endl;
    
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
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    
    // Display message
    setTextColor(YELLOW);
    std::cout << "[GAME] " << message << "                          " << std::endl;
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
    PlaySound(TEXT("pac_intro.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
    
    setTextColor(WHITE);
    std::cout << "Zoraiz Qureshi         Zainab Aamir" << std::endl;
    std::cout << "[21100130]               [21100113]" << std::endl << std::endl;
    
    setTextColor(BLUE);
    std::cout << "       ###############              \n";
    std::cout << "      ##################        \n";
    std::cout << "    #######################         \n";
    std::cout << "  ##############   ###########                  \n";
    std::cout << "##############################                  \n";
    std::cout << "#######################                 \n";
    std::cout << "####################            \n";
    std::cout << "################            \n";
    std::cout << "#############               \n";
    std::cout << "###########                 \n";
    std::cout << "#########                       \n";
    std::cout << "############            ";
    
    setTextColor(BRIGHT_YELLOW);
    std::cout << "PACMAN C++" << std::endl;
    
    setTextColor(BLUE);
    std::cout << "##################          \n";
    std::cout << "#######################         \n";
    std::cout << "###########################                 \n";
    std::cout << "##############################                  \n";
    std::cout << "    #######################                 \n";
    std::cout << "      ####################              \n";
    std::cout << "        ################            " << std::endl;
    
    setTextColor(BRIGHT_YELLOW);
    std::cout << "     _____              _____     __  __                _   _ \n";
    std::cout << "    |  __ \\    /\\      / ____|   |  \\/  |      /\\      | \\ | |\n";
    std::cout << "    | |__) |  /  \\     | |       | \\  / |     /  \\     |  \\| |\n";
    std::cout << "    |  ___/  / /\\ \\    | |       | |\\/| |    / /\\ \\    |     |\n";
    std::cout << "    | |     / ____ \\   | |___    | |  | |   / ____ \\   | |\\  |  \n";
    std::cout << "    |_|    /_/   \\__\\  \\____|    |_|  |_|  /_/    \\_\\  |_| \\_|" << std::endl;
    std::cout << "\nPress s to start, q to quit." << std::endl;
    
    char input = getch();
    std::cout << " ";
    
    switch(input) {
        case 's':
            std::cout << "New game starting...\nSelect level (1 or 2): ";
            int level;
            std::cin >> level;
            return level;
        case 'q':
            exit(0);
        default:
            return 1;
    }
}

void Game::showWinScreen() {
    setTextColor(BRIGHT_YELLOW);
    std::cout << " __     ______  _    _  __          _______ _   _ _ \n";
    std::cout << " \\ \\   / / __ \\| |  | | \\ \\        / /_   _| \\ | | |\n";
    std::cout << "  \\ \\_/ / |  | | |  | |  \\ \\  /\\  / /  | | |  \\| | |\n";
    std::cout << "   \\   /| |  | | |  | |   \\ \\/  \\/ /   | | | . ` | |\n";
    std::cout << "    | | | |__| | |__| |    \\  /\\  /   _| |_| |\\  |_|\n";
    std::cout << "    |_|  \\____/ \\____/      \\/  \\/   |_____|_| \\_(_)" << std::endl;
    std::cout << "\nCongratulations! \nYOUR SCORE: " << score;
    setTextColor(GREEN);
    std::cout << "\n\nWould you like to play again?" << std::endl;
}

void Game::showGameOverScreen() {
    setTextColor(BRIGHT_RED);
    std::cout << "  _____              __  __   ______      ____  __      __  ______   ____  \n";
    std::cout << " / ____|     /\\     |  \\/  | |   ___|    / __ \\ \\ \\    / / |  ____| |  __ \\ \n";
    std::cout << " | |  __    /  \\    | \\  / | |  |__     | |  | | \\ \\  / /  | |__    | |__) |\n";
    std::cout << " | | |_ |  / /\\ \\   | |\\/| | |  __|     | |  | |  \\ \\/ /   |  __|   |  _  / \n";
    std::cout << " | |__| | / ____ \\  | |  | | |  |____   | |__| |   \\  /    | |____  | | \\ \\ \n";
    std::cout << " \\_____/ /_/   \\_ \\ |_|  |_| |______|    \\____/     \\/     |______| |_|  \\_\\" << std::endl;
    setTextColor(GREEN);
    std::cout << "\nRetry?" << std::endl;
}
