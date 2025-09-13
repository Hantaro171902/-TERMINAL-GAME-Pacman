#include "ghost.hpp"
#include "map.hpp"
#include "game.hpp"
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iostream>

using namespace std;

Ghost::Ghost() : posY(9), posX(12), type(GhostType::BLINKY), character('M'), 
                 direction(Direction::UP), speed(250), alive(true) {
}

Ghost::Ghost(GhostType t, int y, int x, int spd) : posY(y), posX(x), type(t), 
                                                   speed(spd), alive(true) {
    switch(type) {
        case GhostType::BLINKY:
            character = 'M';
            direction = Direction::UP;
            break;
        case GhostType::PINKY:
            character = 'W';
            direction = Direction::DOWN;
            break;
        case GhostType::INKY:
            character = 'Y';
            direction = Direction::RIGHT;
            break;
        case GhostType::CLYDE:
            character = 'U';
            direction = Direction::LEFT;
            break;
    }
}

void Ghost::update(int pacmanY, int pacmanX, Map& map, Game& game) {
    if (!alive) return;
    
    int targetY, targetX;
    setTarget(targetY, targetX, pacmanY, pacmanX, game.isSuperMode());
    
    moveTowardsTarget(targetY, targetX, map, game);
}

void Ghost::move(Map& map, Game& game) {
    if (!alive) return;
    
    // Clear current position
    map.setCell(posY, posX, ' ');
    
    int newY = posY;
    int newX = posX;
    
    // Calculate new position based on direction
    switch(direction) {
        case Direction::UP: newY--; break;
        case Direction::DOWN: newY++; break;
        case Direction::RIGHT: newX++; break;
        case Direction::LEFT: newX--; break;
    }
    
    char nextChar = map.getCell(newY, newX);
    
    if (canMove(nextChar, map)) {
        posY = newY;
        posX = newX;
        
        // Handle portal teleportation
        if (map.isPortal(posY, posX)) {
            map.handlePortal(posY, posX);
        }
    } else {
        // Change direction if can't move
        changeDirection(newY, newX, posY, posX, map, game);
    }
    
    // Set new position
    map.setCell(posY, posX, character);
}

bool Ghost::canMove(char nextChar, Map& map) {
    switch(nextChar) {
        case 'v': case '>': case '<': case '^': // Pacman
            return false;
        case 'O': case '.': // Can move on dots and super pellets
            return true;
        case '#': case 'M': case 'W': case 'Y': case 'U': // Walls and other ghosts
            return false;
        case '[': case ']': // Portals
            return false;
        default:
            return true;
    }
}

void Ghost::changeDirection(int targetY, int targetX, int currentY, int currentX, Map& map, Game& game) {
    Direction currentDir = direction;
    int dY = currentY - targetY;
    int dX = currentX - targetX;
    
    char cUp = map.getCell(currentY - 1, currentX);
    char cDown = map.getCell(currentY + 1, currentX);
    char cLeft = map.getCell(currentY, currentX - 1);
    char cRight = map.getCell(currentY, currentX + 1);
    
    // Try to move towards target
    if (direction != Direction::DOWN && canMove(cUp, map) && dY > 0) {
        direction = Direction::UP;
    } else if (direction != Direction::RIGHT && canMove(cLeft, map) && dX > 0) {
        direction = Direction::LEFT;
    } else if (direction != Direction::UP && canMove(cDown, map) && dY < 0) {
        direction = Direction::DOWN;
    } else if (direction != Direction::LEFT && canMove(cRight, map) && dX < 0) {
        direction = Direction::RIGHT;
    }
    
    // If stuck, choose random direction
    if (direction == currentDir) {
        direction = static_cast<Direction>((rand() % 4) + 1);
    }
}

void Ghost::moveTowardsTarget(int targetY, int targetX, Map& map, Game& game) {
    changeDirection(targetY, targetX, posY, posX, map, game);
    move(map, game);
}

void Ghost::randomMove(Map& map, Game& game) {
    direction = static_cast<Direction>((rand() % 4) + 1);
    move(map, game);
}

void Ghost::setPosition(int y, int x) {
    posY = y;
    posX = x;
}

void Ghost::setDirection(Direction dir) {
    direction = dir;
}

void Ghost::reset() {
    switch(type) {
        case GhostType::BLINKY:
            posY = 9; posX = 12;
            direction = Direction::UP;
            break;
        case GhostType::PINKY:
            posY = 9; posX = 14;
            direction = Direction::DOWN;
            break;
        case GhostType::INKY:
            posY = 10; posX = 12;
            direction = Direction::RIGHT;
            break;
        case GhostType::CLYDE:
            posY = 10; posX = 14;
            direction = Direction::LEFT;
            break;
    }
    alive = true;
}

void Ghost::die() {
    alive = false;
}

void Ghost::respawn() {
    reset();
}

void Ghost::setTarget(int& targetY, int& targetX, int pacmanY, int pacmanX, bool superMode) {
    if (!superMode) {
        // Chase mode - different targeting for each ghost
        switch(type) {
            case GhostType::BLINKY:
                targetY = pacmanY;
                targetX = pacmanX;
                break;
            case GhostType::PINKY:
                targetY = pacmanY + 2;
                targetX = pacmanX + 2;
                break;
            case GhostType::INKY:
                targetY = pacmanY;
                targetX = pacmanX;
                break;
            case GhostType::CLYDE:
                targetY = pacmanY;
                targetX = pacmanX;
                break;
        }
    } else {
        // Flee mode - move away from pacman
        targetY = 15 - pacmanY;
        targetX = 15 - pacmanX;
    }
}
