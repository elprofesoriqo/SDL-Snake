# 🐍 Snake Game Project

## Project Overview
Advanced Snake game implementation in C++ using SDL2, featuring complex game mechanics and object-oriented design.

## Key Classes Architecture

### 1. Snake Class
```cpp
class Snake {
private:
    Point body[MAX_SNAKE_LENGTH];  // Snake body points
    int length;                     // Current snake length
    Direction direction;            // Current movement direction
    int score;                      // Game score

public:
    void grow();                    // Increase snake length
    void shrink(int units);         // Decrease snake length
    bool update();                  // Update snake position
    void setDirection(Direction);   // Change snake direction
}
```

### 2. Food Class
```cpp
class Food {
private:
    Point blueFood;     // Standard food
    Point redFood;      // Special bonus food
    bool redFoodActive; // Special food state

public:
    void generateNewBlueFood();  // Generate standard food
    void generateNewRedFood();   // Generate special food
    void update(double delta);   // Update food states
}
```

### 3. Game Class
```cpp
class Game {
private:
    GameState state;        // Current game state
    Snake snake;            // Snake game object
    Food food;              // Food management object
    double gameTime;        // Total game time
    double speedMultiplier; // Game speed modifier

public:
    void update(double delta);   // Main game logic update
    void reset();                // Reset game state
    GameState getState();        // Get current game state
}
```

## Game Mechanics
- Dynamic movement system
- Two food types with different effects
- Progressive difficulty increase
- Speed management
- Advanced collision detection

## Technical Features
- Grid-based rendering
- Time-delta movement
- State machine game flow
- Object-oriented design

## Requirements
- C++11 or newer
- SDL2 library
- CMake (optional)

## Installation
```bash
git clone https://github.com/[your-username]/snake-game.git
cd snake-game
mkdir build && cd build
cmake ..
make
```

## Game Controls
- ⬆️⬇️⬅️➡️: Move snake
- N: New game
- ESC: Exit game

## Configuration
Customize game parameters in `constants.h`:
- Board size
- Initial speed
- Difficulty progression

## Technologies
- Language: C++
- Graphics: SDL2
- Architecture: Object-Oriented
