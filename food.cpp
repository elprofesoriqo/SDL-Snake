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

void Food::generateNewBlueFood() {
    // Generowanie pozycji dla niebieskiego jedzenia
    blueFood.x = rand() % BOARD_SIZE_X;
    blueFood.y = rand() % BOARD_SIZE_Y;
}

void Food::generateNewRedFood() {
    // Generowanie pozycji dla czerwonego jedzenia (bonusu)
    redFood.x = rand() % BOARD_SIZE_X;
    redFood.y = rand() % BOARD_SIZE_Y;
    
    // Losowy wybór typu bonusu
    currentBonus = (rand() % 2 == 0) ? SHORTEN : SLOW_DOWN;
    
    redFoodActive = true;
    redFoodTimer = 0;
}

void Food::update(double delta) {
    if (redFoodActive) {
        redFoodTimer += delta;
        if (redFoodTimer >= 5.0) { // Czerwone jedzenie znika po 5 sekundach
            redFoodActive = false;
            redFoodTimer = 0;
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