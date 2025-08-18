#include "pacman.hpp"
#include "map.hpp"
#include "game.hpp"
#include "ultils.hpp"
#include <iostream>

Pacman::Pacman() : posY(15), posX(13), direction('<'), character('<'), alive(true) {
}

Pacman::Pacman(int y, int x) : posY(y), posX(x), direction('<'), character('<'), alive(true) {
}

void Pacman::move(char input, Map& map, Game& game) {
    char newDirection = direction;
    
    switch(input) {
        case 'w': newDirection = '^'; break;
        case 'a': newDirection = '>'; break;
        case 's': newDirection = 'v'; break;
        case 'd': newDirection = '<'; break;
    }
    
    direction = newDirection;
    character = newDirection;
}

void Pacman::update(Map& map, Game& game) {
    if (!alive) return;
    
    // Clear current position
    map.setCell(posY, posX, ' ');
    
    int newY = posY;
    int newX = posX;
    
    // Calculate new position based on direction
    switch(direction) {
        case '^': newY--; break;
        case 'v': newY++; break;
        case '<': newX++; break;
        case '>': newX--; break;
    }
    
    char nextChar = map.getCell(newY, newX);
    
    if (canMove(nextChar, map, game)) {
        posY = newY;
        posX = newX;
        
        // Handle portal teleportation
        if (map.isPortal(posY, posX)) {
            map.handlePortal(posY, posX);
        }
    }
    
    // Set new position
    map.setCell(posY, posX, character);
}

bool Pacman::canMove(char nextChar, Map& map, Game& game) {
    switch(nextChar) {
        case 'M': case 'W': case 'Y': case 'U': // Ghost
            handleCollision(nextChar, map, game);
            return false;
        case 'O': // Super Pellet
            game.setSuperMode(true);
            game.setMessage("Super mode is now active!");
            // play_sound("assets/pac_interm.wav");
            return true;
        case '.': // Dot
            // play_sound("assets/pac_munch.wav");
            game.incrementDotsEaten();
            game.incrementScore();
            return true;
        case '#': // Wall
            return false;
        case '[': case ']': // Portal
            return true;
        default:
            return true;
    }
}

void Pacman::handleCollision(char ghostChar, Map& map, Game& game) {
    if (!game.isSuperMode()) {
        // Pacman gets eaten
        game.setMessage("You were eaten by a ghost! You lost a life. :(");
        // play_sound("assets/pac_death.wav");
        game.decrementLives();
        die();
    } else {
        // Pacman eats ghost
        game.setMessage("You ate a ghost! +100 SCORE!");
        // play_sound("assets/pac_eatghost.wav");
        game.setScore(game.getScore() + 100);
        // Ghost will be reset by the ghost class
    }
}

void Pacman::setPosition(int y, int x) {
    posY = y;
    posX = x;
}

void Pacman::setDirection(char dir) {
    direction = dir;
    character = dir;
}

void Pacman::reset() {
    posY = 15;
    posX = 13;
    direction = '<';
    character = '<';
    alive = true;
}

void Pacman::die() {
    alive = false;
}

void Pacman::respawn() {
    resetPosition();
    alive = true;
}

void Pacman::resetPosition() {
    posY = 15;
    posX = 13;
    direction = '<';
    character = '<';
}
