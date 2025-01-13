#include "snake.h"

Snake::Snake() {
    reset();
}

void Snake::reset() {
    length = INITIAL_SNAKE_LENGTH;
    int startX = BOARD_SIZE_X / 2;
    int startY = BOARD_SIZE_Y / 2;

    for (int i = 0; i < length; i++) {
        body[i] = Point(startX - i, startY);
    }

    direction = RIGHT;
    nextDirection = RIGHT;
    isDead = false;
    score = 0;
}

void Snake::setDirection(Direction newDir) {
    if ((direction == UP && newDir == DOWN) ||
        (direction == DOWN && newDir == UP) ||
        (direction == LEFT && newDir == RIGHT) ||
        (direction == RIGHT && newDir == LEFT)) {
        return;
    }
    nextDirection = newDir;
}

void Snake::grow() {
    if (length < MAX_SNAKE_LENGTH) {
        body[length] = body[length - 1];
        length++;
    }
}

void Snake::addScore(int points) {
    score += points;
}

bool Snake::update() {
    if (isDead) return false;

    direction = nextDirection;
    Point newHead = body[0];

    switch (direction) {
    case UP: newHead.y--; break;
    case DOWN: newHead.y++; break;
    case LEFT: newHead.x--; break;
    case RIGHT: newHead.x++; break;
    }

    if (newHead.x < 0 || newHead.x >= BOARD_SIZE_X ||
        newHead.y < 0 || newHead.y >= BOARD_SIZE_Y) {
        isDead = true;
        return false;
    }

    for (int i = 0; i < length; i++) {
        if (newHead == body[i]) {
            isDead = true;
            return false;
        }
    }

    for (int i = length - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }
    body[0] = newHead;
    
    return true;
}