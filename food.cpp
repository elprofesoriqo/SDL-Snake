#include "food.h"
#include "constants.h"
#include <time.h>
#include <stdlib.h>

Food::Food() : redFoodActive(false), redFoodTimer(0), redFoodCooldown(0) {
    srand(time(NULL));
    reset();
}

void Food::reset() {
    generateNewBlueFood();
    redFoodActive = false;
    redFoodTimer = 0;
    redFoodCooldown = 0;
}

void Food::generateNewBlueFood() {
    blueFood.x = rand() % BOARD_SIZE_X;
    blueFood.y = rand() % BOARD_SIZE_Y;
}

void Food::generateNewRedFood() {
    redFood.x = rand() % BOARD_SIZE_X;
    redFood.y = rand() % BOARD_SIZE_Y;
    redFoodActive = true;
    redFoodTimer = 0;
}

void Food::update(double delta) {
    if (redFoodActive) {
        redFoodTimer += delta;
        if (redFoodTimer >= 5.0) { // Czerwone jedzenie znika po 5 sekundach
            redFoodActive = false;
        }
    }
    else {
        redFoodCooldown += delta;
        if (redFoodCooldown >= 30.0) { // Nowe czerwone jedzenie pojawia się co 30 sekund
            redFoodCooldown = 0;
            generateNewRedFood();
        }
    }
}
