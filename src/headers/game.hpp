#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#endif
#include "pacman.hpp"
#include "ghost.hpp"
#include "map.hpp"
#include "ultils.hpp"
#include "color.hpp"

class Game {
private:
    Pacman pacman;
    std::vector<Ghost> ghosts;
    Map gameMap;
    
    int score;
    int lives;
    int time;
    int SMtime;
    int dotsEaten;
    int maxDots;
    bool superMode;
    std::string message;
    
    bool gameRunning;
    std::thread pacmanThread;
    std::vector<std::thread> ghostThreads;
    
    void initializeGame(int level);
    void runGameLoop();
    void handleGameEnd();
    void resetGame();
    int showTitleScreen();
    void showWinScreen();
    void showGameOverScreen();
    
public:
    Game();
    ~Game();
    void start();
    void stop();
    bool isRunning() const;
    
    // Getters for game state
    int getScore() const { return score; }
    int getLives() const { return lives; }
    int getDotsEaten() const { return dotsEaten; }
    int getMaxDots() const { return maxDots; }
    bool isSuperMode() const { return superMode; }
    std::string getMessage() const { return message; }
    
    // Setters for game state
    void setScore(int s) { score = s; }
    void setLives(int l) { lives = l; }
    void setSuperMode(bool sm) { superMode = sm; }
    void setMessage(const std::string& msg) { message = msg; }
    void incrementDotsEaten() { dotsEaten++; }
    void incrementScore() { score++; }
    void decrementLives() { lives--; }
};
