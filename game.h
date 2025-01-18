#pragma once
#include "snake.h"
#include "food.h"
#include <SDL.h>

class Game {
private:
    GameState state;
    Snake snake;
    Food food;
    double gameTime; //czas gry
    double stateTimer;
    double moveTimer;
    double speedUpTimer;
    double finalTime;
    double speedMultiplier; //zwiększenie prędkości
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
    double getSpeedMultiplier() const { return speedMultiplier; }
};