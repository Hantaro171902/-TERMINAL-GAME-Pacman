#pragma once

#include <iostream>
#include <string>
#include "game_forward.hpp"

class Pacman {
private:
    int posY, posX;
    char direction; // '<', '>', '^', 'v'
    char character; // Current character representation
    bool alive;
    
    bool canMove(char nextChar, Map& map, Game& game);
    void handleCollision(char nextChar, Map& map, Game& game);
    void resetPosition();
    
public:
    Pacman();
    Pacman(int y, int x);
    
    // Movement methods
    void move(char input, Map& map, Game& game);
    void update(Map& map, Game& game);
    
    // Getters
    int getY() const { return posY; }
    int getX() const { return posX; }
    char getDirection() const { return direction; }
    char getCharacter() const { return character; }
    bool isAlive() const { return alive; }
    
    // Setters
    void setPosition(int y, int x);
    void setDirection(char dir);
    void setAlive(bool a) { alive = a; }
    
    // Game logic
    void reset();
    void die();
    void respawn();
};
