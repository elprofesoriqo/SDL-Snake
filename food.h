#pragma once
#include "types.h"

//typy bonusów
enum BonusType {
    SHORTEN,
    SLOW_DOWN
};

class Food {
private:
    //pozycje
    Point blueFood;
    Point redFood;
    
    bool redFoodActive;
    double redFoodTimer;
    double redFoodCooldown;
    BonusType currentBonus;

public:
    Food();
    void reset();
    void update(double delta);
    Point getBlueFood() const { return blueFood; }
    Point getRedFood() const { return redFood; }
    bool isRedFoodActive() const { return redFoodActive; }
    void generateNewBlueFood();
    void generateNewRedFood();
    void deactivateRedFood() { redFoodActive = false; }
    BonusType getBonusType() const { return currentBonus; }
    double getRedFoodTimer() const { return redFoodTimer; }
};