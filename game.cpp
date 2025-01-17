#include "game.h"
#include "constants.h"

Game::Game() : state(MENU), gameTime(0), stateTimer(0), moveTimer(0), speedUpTimer(0), finalTime(0) {
    reset();
}

void Game::reset() {
    snake.reset();
    food.reset();
    gameTime = 0;
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
    gameTime += delta;
    stateTimer += delta;
    
    if (state == PLAYING) {
        moveTimer += delta;
        speedUpTimer += delta;
        
        // Speed up every 20 seconds instead of 10
        if (speedUpTimer >= 5.0) {
            speedUpTimer = 0;
            moveTimer *= 0.5; // Increase speed by 10%
        }
        
        // Update snake position
        if (moveTimer >= 0.15) {
            moveTimer = 0;
            updateGameLogic(delta);
        }
        
        food.update(delta);
    }
}

void Game::updateGameLogic(double delta) {
    if (!snake.update()) {
        finalTime = gameTime;  // Zapisz czas końca gry
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
    snake.addScore(5); // Mniej punktów za bonus
    
    if (food.getBonusType() == SHORTEN) {
        snake.shrink(2); // Skróć o 2 jednostki
    } else {
        moveTimer *= 1.2; // Spowolnij o 20%
    }
    
    food.deactivateRedFood();
}
}