#include "map.hpp"
#include "ultils.hpp"
#include <iostream>

using namespace std;

Map::Map() : maxDots(0), currentLevel(1) {
    loadLevel(1);
}

Map::Map(int level) : maxDots(0), currentLevel(level) {
    loadLevel(level);
}

void Map::loadLevel(int level) {
    currentLevel = level;
    if (level == 1) {
        loadLevel1();
    } else if (level == 2) {
        loadLevel2();
    }
}

void Map::loadLevel1() {
    char map1[MAP_HEIGHT][MAP_WIDTH] = {
        // "╔═══╦═════════════════╦═══╗",
        // "║...║.................║...║",
        // "║.O.║.══╦══.....══╦══.║.O.║",
        // "║.......║...╔.....║.......║",
        // "╠══###.#║###║═════║#.####.║",
        // "║......#.....#.....#......║",
        // "╠══##.╔####.#.═════.═════╝",
        // "║    #.#           #.#     ",
        // "║═════.#           #.═════#",
        // "║.....#             #.....║",
        // "═════#.#           #.═════╝",
        // "     #.#           #.#     ",
        // "╔═════.#..═════##..#.#####║",
        // "║............#............║",
        // "║.####.#####.#.#####.####.║",
        // "║O...#.......<.......#...O║",
        // "║###.#.#.#########.#.#.###║",
        // "║......#.....#.....#......║",
        // "║.####.#####.#.#####.####.║",
        // "║.........................║",
        // "╚═════════════════════════╝"

        "###########################",
        "#O..........###..........O#",
        "#....#################....#",
        "#.........................#",
        "#.######.#########.######.#",
        "[......#.....#.....#......]",
        "######....#######....######",
        "#O.....#           #.....O#",
        "######.#           #.######",
        "[......             ......]",
        "######.#           #.######",
        "#O.....#           #.....O#",
        "######.#..#######..#.######",
        "[.........................]",
        "#.####.#############.####.#",
        "#............<............#",
        "######...###...###...######",
        "#....#...###...###...#....#",
        "#O.......................O#",
        "#..........#####..........#",
        "###########################"
    };
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            level[y][x] = map1[y][x];
        }
    }
    maxDots = 210;
}

void Map::loadLevel2() {
    char map2[MAP_HEIGHT][MAP_WIDTH] = {
        "###########################",
        "#O..........###..........O#",
        "#....#################....#",
        "#.........................#",
        "#.######.#########.######.#",
        "[......#.....#.....#......]",
        "######....#######....######",
        "#O.....#           #.....O#",
        "######.#           #.######",
        "[......             ......]",
        "######.#           #.######",
        "#O.....#           #.....O#",
        "######.#..#######..#.######",
        "[.........................]",
        "#.####.#############.####.#",
        "#............<............#",
        "######...###...###...######",
        "#....#...###...###...#....#",
        "#O.......................O#",
        "#..........#####..........#",
        "###########################"
    };
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            level[y][x] = map2[y][x];
        }
    }
    maxDots = 251;
}

void Map::reset() {
    loadLevel(currentLevel);
}

char Map::getCell(int y, int x) const {
    if (y < 0 || y >= getHeight() || x < 0 || x >= getWidth()) return '#';
    return level[y][x];
}

void Map::setCell(int y, int x, char c) {
    if (isValidPosition(y, x)) {
        level[y][x] = c;
    }
}

bool Map::isValidPosition(int y, int x) const {
    return y >= 0 && y < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH;
}

bool Map::isWall(int y, int x) const {
    return getCell(y, x) == '#';
}

bool Map::isDot(int y, int x) const {
    return getCell(y, x) == '.';
}

bool Map::isSuperPellet(int y, int x) const {
    return getCell(y, x) == 'O';
}

bool Map::isPortal(int y, int x) const {
    char cell = getCell(y, x);
    return cell == '[' || cell == ']';
}

bool Map::isGhost(int y, int x) const {
    char cell = getCell(y, x);
    return cell == 'M' || cell == 'W' || cell == 'Y' || cell == 'U';
}

bool Map::isPacman(int y, int x) const {
    char cell = getCell(y, x);
    return cell == '<' || cell == '>' || cell == '^' || cell == 'v';
}

void Map::handlePortal(int& y, int& x) const {
    char cell = getCell(y, x);
    if (cell == '[') {
        x = 26; // Teleport to right portal
    } else if (cell == ']') {
        x = 0;  // Teleport to left portal
    }
}

string Map::renderCell(int y, int x) const {
    char cell = getCell(y, x);
    switch(cell) {
        case '#': return BLOCK_FULL; // Wall - █ (full UTF-8 string)
        case '.': return ".";      // Dot
        case 'O': return "o";      // Super Pellet
        case '[': return "[";      // Left Portal
        case ']': return "]";      // Right Portal
        default:  return " ";      // Empty space or unknown
    }
}

void Map::display() const {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            cout << renderCell(y, x);
        }
        cout << endl;
    }
}
