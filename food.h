#pragma once
#include "types.h"

class Food {
private:
    Point blueFood;
    Point redFood;
    bool redFoodActive;
    double redFoodTimer;
    double redFoodCooldown;

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
};
