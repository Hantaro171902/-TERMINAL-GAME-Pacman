#pragma once 

#include <vector>
#include <string>

struct GameData
{
    int pY, pX;
    int score, lives, time, SMtime;
    int dotsEaten, maxDots;
    int eY1, eX1, eY2, eX2, eY3, eX3, eY4, eX4;
    bool SM;
    std::vector<std::string> level;
    std::string message;
};

class Pacman
{
private:
    GameData data;
public:
    Pacman();
    void startGame();
    bool moveAllowed(char c, GameData& data);
    void updateGame();
    void thread(GameData& data);
};
