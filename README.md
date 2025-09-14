# 👻 Pacman Game - OOP Terminal Edition

A console-based Pacman clone written in C++ with Object-Oriented Programming principles and ANSI art flair.

---

<img width="818" height="723" alt="image" src="https://github.com/user-attachments/assets/a52e4c23-7a63-485c-bd46-cd16855d7ea0" />

--- 

## 🎮 Features

- 🧩 **OOP Design** – Clean separation of game components  
- ⚡ **Multi-threaded** – Smooth concurrent Pacman & ghost movement  
- 🗺️ **Two Levels** – Unique maps with different layouts  
- 🔊 **Sound Effects** – Audio feedback for events  
- 🌈 **Colors** – Distinct visuals for each element  
- 💥 **Super Mode** – Power pellets to eat ghosts  
- 🌀 **Portals** – Teleport from edge to edge  

---

## 🧱 Class Structure

- **`Game`** – Main controller (loop, score, lives, states)  
- **`Pacman`** – Player movement, collisions, portals  
- **`Ghost`** – AI (Blinky, Pinky, Inky, Clyde) with chase/flee modes  
- **`Map`** – Loads levels, checks walls, dots, portals  
- **`Console`** – Cursor control, colors, input  

---

## ⚙️ Building

### Prerequisites
- Linux/Unix terminal with ANSI color  
- C++11 compiler (GCC/Clang)  
- `make` or CMake  

### Compile & Run
```bash
cd ascii-game/pacman-game
make
make run
```

## 🕹️ Controls

| Key        | Action       |
|------------|--------------|
| **W / ⬆️** | Move Up      |
| **S / ⬇️** | Move Down    |
| **A / ⬅️** | Move Left    |
| **D / ➡️** | Move Right   |
| **S** (title) | Start     |
| **Q**      | Quit         |
| **R** (game over) | Restart |


## 🎨 Game Elements

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

## 📂 File Structure

```
pacman-game/
├── src/
│   ├── headers/
│   │   ├── game.hpp
│   │   ├── pacman.hpp
│   │   ├── ghost.hpp
│   │   ├── map.hpp
│   │   ├── ultils.hpp
│   │   └── game_forward.hpp
│   ├── main.cpp
│   ├── game.cpp
│   ├── pacman.cpp
│   ├── ghost.cpp
│   ├── map.cpp
│   └── ultils.cpp
├── Makefile
└── README.md
```

## Dependencies

- **ANSI Escape Codes**: For console manipulation and colors
- **C++11 Standard Library**: For threading, containers, and utilities
- **Linux/Unix System Calls**: For terminal input/output and sound
- **aplay**: For audio playback (Linux)

## 🚀 Future Enhancements

- Additional game levels
- High score system
- Configurable difficulty settings
- More ghost AI patterns
- Enhanced visual effects
- Sound effects for different terminals
- Terminal size detection and adjustment

## Credits

- Original console Pacman by Zoraiz Qureshi & Zainab Aamir.
- Repo: https://github.com/ZoraizQ/console-pacman.git
- OOP refactor and enhancements by Hantaro1917.

✨ Built with ghosts, dots, and way too much coffee ☕
