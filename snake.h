#pragma once
#include "types.h"
#include "constants.h"

class Snake {
private:
    Point body[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
    Direction nextDirection;
    bool isDead;
    int score;

public:
    Snake();
    void reset();
    void setDirection(Direction newDir);
    bool update();
    const Point* getBody() const { return body; }
    int getLength() const { return length; }
    bool getIsDead() const { return isDead; }
    int getScore() const { return score; }
    void addScore(int points);
    void grow();
};
