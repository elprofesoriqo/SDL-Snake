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

//pozycje food
void Food::generateNewBlueFood() {
    blueFood.x = rand() % BOARD_SIZE_X;
    blueFood.y = rand() % BOARD_SIZE_Y;
}

void Food::generateNewRedFood() {
    redFood.x = rand() % BOARD_SIZE_X;
    redFood.y = rand() % BOARD_SIZE_Y;
    currentBonus = (rand() % 2 == 0) ? SHORTEN : SLOW_DOWN; //losowanie bonusu
    redFoodActive = true;
    redFoodTimer = 0;
}

void Food::update(double delta) {
    if (redFoodActive) {
        redFoodTimer += delta;
        if (redFoodTimer >= RED_FOOD_DURATION) { //red food znika po x sekundach
            redFoodActive = false;
            redFoodTimer = 0;
        }
    }
    else {
        redFoodCooldown += delta;
        if (redFoodCooldown >= RED_FOOD_COOLDOWN) { //pojawia się co y sekund
            redFoodCooldown = 0;
            generateNewRedFood();
        }
    }
}