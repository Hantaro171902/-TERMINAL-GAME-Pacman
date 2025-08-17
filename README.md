# Pacman Game - OOP Version

A console-based Pacman game implemented in C++ using Object-Oriented Programming principles.

## Features

- **Object-Oriented Design**: Clean separation of concerns with dedicated classes for each game component
- **Multi-threaded Gameplay**: Smooth concurrent movement of Pacman and ghosts
- **Two Game Levels**: Different map layouts with varying difficulty
- **Sound Effects**: Audio feedback for game events
- **Color-coded Display**: Visual distinction between game elements
- **Super Mode**: Power pellets that allow Pacman to eat ghosts
- **Portal System**: Teleportation between map edges

## Class Structure

### Core Classes

- **`Game`**: Main game controller that manages the overall game flow
- **`Pacman`**: Player character with movement and collision handling
- **`Ghost`**: Enemy AI with different behaviors and targeting strategies
- **`Map`**: Game map management and level loading
- **`Console`**: Console utilities for display, colors, and input handling

### Class Responsibilities

#### Game Class
- Manages game state (score, lives, super mode)
- Controls game loop and threading
- Handles title screen and game end screens
- Coordinates between all game components

#### Pacman Class
- Handles player movement and input
- Manages collision detection with walls, dots, and ghosts
- Controls super mode activation
- Handles portal teleportation

#### Ghost Class
- Implements AI behavior for each ghost type
- Different targeting strategies (Blinky, Pinky, Inky, Clyde)
- Chase and flee modes based on super mode
- Individual movement speeds and patterns

#### Map Class
- Loads and manages game levels
- Provides collision detection utilities
- Handles portal logic
- Manages dot counting and level completion

#### Console Class
- Cursor control and positioning
- Color management for visual elements
- Input handling and keyboard detection
- Screen clearing and display utilities

## Building the Game

### Prerequisites
- Linux/Unix operating system
- C++ compiler with C++11 support (GCC, Clang)
- Make utility or CMake
- Terminal with ANSI color support

### Compilation

1. Navigate to the project directory:
   ```bash
   cd ascii-game/pacman-game
   ```

2. Build the game:
   ```bash
   make
   ```

3. Run the game:
   ```bash
   make run
   ```

### Alternative Build Commands

- `make clean` - Remove build files
- `make help` - Show available build targets

## Game Controls

- **W** - Move Up
- **A** - Move Left  
- **S** - Move Down
- **D** - Move Right
- **S** (title screen) - Start game
- **Q** (title screen) - Quit game
- **R** (game over) - Play again

## Game Elements

### Characters
- **< > ^ v** - Pacman (direction indicators)
- **M** - Blinky (Red ghost)
- **W** - Pinky (Pink ghost)
- **Y** - Inky (Cyan ghost)
- **U** - Clyde (Orange ghost)

### Map Elements
- **#** - Walls
- **.** - Dots (1 point each)
- **O** - Super Pellets (activates super mode)
- **[ ]** - Portals (teleportation)

### Game States
- **Normal Mode**: Ghosts chase Pacman
- **Super Mode**: Pacman can eat ghosts for 100 points each
- **Game Over**: When all lives are lost
- **Victory**: When all dots are collected

## Technical Details

### Threading Model
- Main game thread handles Pacman movement and display
- Separate threads for each ghost's AI behavior
- Thread-safe game state management

### Memory Management
- RAII principles for resource management
- Smart pointer usage where appropriate
- Proper cleanup in destructors

### Error Handling
- Exception-safe design
- Graceful error recovery
- User-friendly error messages

## File Structure

```
pacman-game/
├── src/
│   ├── headers/
│   │   ├── game.hpp
│   │   ├── pacman.hpp
│   │   ├── ghost.hpp
│   │   ├── map.hpp
│   │   ├── console.hpp
│   │   └── game_forward.hpp
│   ├── main.cpp
│   ├── game.cpp
│   ├── pacman.cpp
│   ├── ghost.cpp
│   ├── map.cpp
│   └── console.cpp
├── Makefile
└── README.md
```

## Dependencies

- **ANSI Escape Codes**: For console manipulation and colors
- **C++11 Standard Library**: For threading, containers, and utilities
- **Linux/Unix System Calls**: For terminal input/output and sound
- **aplay**: For audio playback (Linux)

## Future Enhancements

- Additional game levels
- High score system
- Configurable difficulty settings
- More ghost AI patterns
- Enhanced visual effects
- Sound effects for different terminals
- Terminal size detection and adjustment

## Credits

Original console-pacman implementation by Zoraiz Qureshi and Zainab Aamir.
OOP refactoring and enhancement by AI Assistant.
