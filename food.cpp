#include "food.h"
#include "constants.h"
#include <time.h>
#include <stdlib.h>

Food::Food() : redFoodActive(false), redFoodTimer(0), redFoodCooldown(0), currentBonus(SHORTEN) {
    srand(static_cast<unsigned int>(time(NULL)));
    reset();
}

void Food::reset() {
    generateNewBlueFood();
    redFoodActive = false;
    redFoodTimer = 0;
    redFoodCooldown = 0;
}

//pozycja dla blue food
void Food::generateNewBlueFood() {
    blueFood.x = rand() % BOARD_SIZE_X;
    blueFood.y = rand() % BOARD_SIZE_Y;
}

//pozycja dla red food
void Food::generateNewRedFood() {
    redFood.x = rand() % BOARD_SIZE_X;
    redFood.y = rand() % BOARD_SIZE_Y;
    
    //typ bonusu
    currentBonus = (rand() % 2 == 0) ? SHORTEN : SLOW_DOWN;
    
    redFoodActive = true;
    redFoodTimer = 0;
}


void Food::update(double delta) {
    if (redFoodActive) {
        redFoodTimer += delta;
        if (redFoodTimer >= 5.0) { //red food znika po x sekundach
            redFoodActive = false;
            redFoodTimer = 0;
        }
    }
    else {
        redFoodCooldown += delta;
        if (redFoodCooldown >= 20.0) { //pojawia się co y sekund
            redFoodCooldown = 0;
            generateNewRedFood();
        }
    }
}