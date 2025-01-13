#include "game.h"
#include "constants.h"

Game::Game() : state(MENU), worldTime(0), stateTimer(0), moveTimer(0), speedUpTimer(0) {
    reset();
}

void Game::reset() {
    snake.reset();
    food.reset();
    worldTime = 0;
    stateTimer = 0;
    moveTimer = 0;
    speedUpTimer = 0;
    state = PLAYING;
}

void Game::setState(GameState newState) {
    state = newState;
    stateTimer = 0; // Reset the state timer when changing states
}

bool Game::shouldExitGameOver() const {
    return state == GAME_OVER && stateTimer >= 5.0;
}

void Game::update(double delta) {
    worldTime += delta;
    stateTimer += delta;
    
    if (state == PLAYING) {
        moveTimer += delta;
        speedUpTimer += delta;
        
        // Przyspieszanie gry co 10 sekund
        if (speedUpTimer >= 10.0) {
            speedUpTimer = 0;
            moveTimer *= 0.9; // Zwiększenie prędkości o 10%
        }
        
        // Aktualizacja pozycji węża
        if (moveTimer >= 0.15) { // Interwał ruchu
            moveTimer = 0;
            updateGameLogic(delta);
        }
        
        food.update(delta);
    }
    else if (state == GAME_OVER && stateTimer >= 5.0) {
        stateTimer = 0;
    }
}

void Game::updateGameLogic(double delta) {
    if (!snake.update()) {
        setState(GAME_OVER);
        return;
    }

    // Sprawdzanie kolizji z jedzeniem
    Point head = snake.getBody()[0];
    
    if (head == food.getBlueFood()) {
        snake.addScore(10);
        snake.grow();
        food.generateNewBlueFood();
    }
    
    if (food.isRedFoodActive() && head == food.getRedFood()) {
        snake.addScore(20);
        snake.grow();
        snake.grow(); // Czerwone jedzenie daje podwójny wzrost
        food.deactivateRedFood();
    }
}