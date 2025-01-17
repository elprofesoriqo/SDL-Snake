#pragma once
#include "snake.h"
#include "food.h"
#include <SDL.h>  // Dodane

class Game {
private:
    GameState state;
    Snake snake;
    Food food;
    double gameTime;
    double stateTimer;
    double moveTimer;
    double speedUpTimer;
    double finalTime;

public:
    Game();
    void reset();
    void update(double delta);
    bool processInput(SDL_Event& event);
    void updateGameLogic(double delta);
    GameState getState() const { return state; }
    Snake& getSnake() { return snake; }
    Food& getFood() { return food; }
    double getFinalTime() const { return finalTime; }
    double getGameTime() const { return gameTime; }
    void setState(GameState newState);
    bool shouldExitGameOver() const;

};