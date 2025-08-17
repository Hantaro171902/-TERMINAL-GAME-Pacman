#pragma once

#include <iostream>
#include <string>
#include <vector>

class Map {
private:
    static const int MAP_HEIGHT = 21;
    static const int MAP_WIDTH = 28;
    
    char level[MAP_HEIGHT][MAP_WIDTH];
    int maxDots;
    int currentLevel;
    
    // Predefined maps
    void loadLevel1();
    void loadLevel2();
    
public:
    Map();
    Map(int level);
    
    // Map management
    void loadLevel(int level);
    void reset();
    char getCell(int y, int x) const;
    void setCell(int y, int x, char c);
    bool isValidPosition(int y, int x) const;
    
    // Game logic
    bool isWall(int y, int x) const;
    bool isDot(int y, int x) const;
    bool isSuperPellet(int y, int x) const;
    bool isPortal(int y, int x) const;
    bool isGhost(int y, int x) const;
    bool isPacman(int y, int x) const;
    
    // Portal handling
    void handlePortal(int& y, int& x) const;
    
    // Getters
    int getHeight() const { return MAP_HEIGHT; }
    int getWidth() const { return MAP_WIDTH; }
    int getMaxDots() const { return maxDots; }
    int getCurrentLevel() const { return currentLevel; }
    
    // Display
    void display() const;
};
