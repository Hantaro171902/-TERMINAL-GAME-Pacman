#pragma once

#include <iostream>
#include <string>
#include "game_forward.hpp"

enum class GhostType {
    BLINKY,  // Red - M
    PINKY,   // Pink - W
    INKY,    // Cyan - Y
    CLYDE    // Orange - U
};

enum class Direction {
    UP = 1,
    DOWN = 2,
    RIGHT = 3,
    LEFT = 4
};

class Ghost {
private:
    int posY, posX;
    GhostType type;
    char character;
    Direction direction;
    int speed; // Delay in milliseconds
    bool alive;
    
    // AI behavior
    void changeDirection(int targetY, int targetX, int currentY, int currentX, Map& map, Game& game);
    bool canMove(char nextChar, Map& map);
    void moveTowardsTarget(int targetY, int targetX, Map& map, Game& game);
    void randomMove(Map& map, Game& game);
    
public:
    Ghost();
    Ghost(GhostType t, int y, int x, int spd);
    
    // Movement and AI
    void update(int pacmanY, int pacmanX, Map& map, Game& game);
    void move(Map& map, Game& game);
    
    // Getters
    int getY() const { return posY; }
    int getX() const { return posX; }
    char getCharacter() const { return character; }
    GhostType getType() const { return type; }
    bool isAlive() const { return alive; }
    
    // Setters
    void setPosition(int y, int x);
    void setDirection(Direction dir);
    void setAlive(bool a) { alive = a; }
    
    // Game logic
    void reset();
    void die();
    void respawn();
    
    // AI targeting
    void setTarget(int& targetY, int& targetX, int pacmanY, int pacmanX, bool superMode);
};
