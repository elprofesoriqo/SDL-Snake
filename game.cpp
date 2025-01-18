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

//po 5 sekundach gra się zaczyna
bool Game::shouldExitGameOver() const {
    return state == GAME_OVER && stateTimer >= 5.0;
}


//delata- czas który upłynał od ostatniej aktualizacji
void Game::update(double delta) {
    gameTime += delta;
    stateTimer += delta;
    
    if (state == PLAYING) {
        moveTimer += delta;
        speedUpTimer += delta;
        
        //przyśpieszanie gry co x sekund
        if (speedUpTimer >= 10.0) {
            speedUpTimer = 0;
            speedMultiplier += 0.2; //zwiększanie o y

        }
        
        //up pozycja snake'a
        if (moveTimer >= 0.15 / speedMultiplier) { // Dzielimy bazowy czas ruchu przez mnożnik
            moveTimer = 0;
            updateGameLogic(delta);
        }
        
        food.update(delta);
    }
}

void Game::updateGameLogic(double delta) {

    //dotknięcie samego siebie
    if (!snake.update()) {
        finalTime = gameTime;
        setState(GAME_OVER);
        return;
    }

    //doktnięcie food
    Point head = snake.getBody()[0];
    
    if (head == food.getBlueFood()) {
        snake.addScore(10); //+punkty
        snake.grow();
        food.generateNewBlueFood();
    }
    
    if (food.isRedFoodActive() && head == food.getRedFood()) {
        snake.addScore(20); //+punkty
    
        //randomowe zmniejszanie o x || spowolanienie o y%
        if (food.getBonusType() == SHORTEN) {
            snake.shrink(2); 
        } else {
            speedMultiplier *= 0.9;
        }
    
        food.deactivateRedFood();
    }
}