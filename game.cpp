#include "game.h"
#include "constants.h"
#include <cstdio>

//konstruktor
Game::Game() : state(MENU), gameTime(0), stateTimer(0), moveTimer(0), speedUpTimer(0), finalTime(0), speedMultiplier(1.0) {
    reset(); //init gry
}

void Game::reset() {
    snake.reset();
    food.reset();
    gameTime = 0;
    stateTimer = 0;
    moveTimer = 0;
    speedUpTimer = 0;
    speedMultiplier = 1.0; //startowa wartość prędkości
    state = PLAYING;
}

void Game::setState(GameState newState) {
    state = newState;
    stateTimer = 0;
}

//wyświetlanie podsumowania gry
bool Game::shouldExitGameOver() const {
    return state == GAME_OVER && stateTimer >= GAME_OVER_DURATION;
}

//delata- czas który upłynał od ostatniej aktualizacji
void Game::update(double delta) {
    gameTime += delta;
    stateTimer += delta;
    
    if (state == PLAYING) {
        moveTimer += delta;
        speedUpTimer += delta;
        
        if (speedUpTimer >= SPEED_INCREASE_INTERVAL) { //przyśpieszanie gry co x sekund
            speedUpTimer = 0;
            speedMultiplier += SPEED_INCREASE_AMOUNT;
        }
        
        //up pozycja snake'a
        if (moveTimer >= BASE_MOVE_DELAY / speedMultiplier) { //base czas ruchu * mnożnik
            moveTimer = 0;
            updateGameLogic(delta);
        }
        
        food.update(delta);
    }
}

void Game::updateGameLogic(double delta) {
    //czy nie zajął całej planszy
    if (snake.isFullBoard()) {
        finalTime = gameTime;
        setState(GAME_OVER);
        return;
    }

    //dotknięcie samego siebie
    if (!snake.update()) {
        finalTime = gameTime;
        setState(GAME_OVER);
        return;
    }

    Point head = snake.getBody()[0];
    
    if (head == food.getBlueFood()) {
        snake.addScore(BLUE_FOOD_SCORE);
        snake.grow();
        //po zjedzeniu czy nie ma całej planszy
        if (snake.isFullBoard()) {
            finalTime = gameTime;
            setState(GAME_OVER);
            return;
        }
        food.generateNewBlueFood();
    }
    
    if (food.isRedFoodActive() && head == food.getRedFood()) {
        snake.addScore(RED_FOOD_SCORE);
    
        //randomowe zmniejszanie o x || spowolanienie o y%
        if (food.getBonusType() == SHORTEN) {
            snake.shrink(SNAKE_SHRINK_AMOUNT);
        } else {
            speedMultiplier *= SPEED_DECREASE_FACTOR;
        }
    
        food.deactivateRedFood();
    }
}