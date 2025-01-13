#pragma once
#include "snake.h"
#include "food.h"
#include <SDL.h>  // Dodane

class Game {
private:
    GameState state;
    Snake snake;
    Food food;
    double worldTime;
    double stateTimer;
    double moveTimer;
    double speedUpTimer;

public:
    Game();
    void reset();
    void update(double delta);
    bool processInput(SDL_Event& event);
    void updateGameLogic(double delta);
    GameState getState() const { return state; }
    Snake& getSnake() { return snake; }
    Food& getFood() { return food; }
    double getWorldTime() const { return worldTime; }
    void setState(GameState newState);
    bool shouldExitGameOver() const;
};