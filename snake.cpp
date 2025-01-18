#include "snake.h"

Snake::Snake() {
    reset();
}

void Snake::reset() {
    length = INITIAL_SNAKE_LENGTH;
    int startX = BOARD_SIZE_X / 2;
    int startY = BOARD_SIZE_Y / 2;

    for (int i = 0; i < length; i++) {
        body[i] = Point(startX - i, startY); //pozycja startowa
    }

    direction = RIGHT;
    nextDirection = RIGHT;
    isDead = false;
    score = 0;
}

void Snake::setDirection(Direction newDir) {
    Point head = body[0];

    //czy wąż jest przy ścianie
    bool atTopWall = head.y <= 0;
    bool atBottomWall = head.y >= BOARD_SIZE_Y - 1;
    bool atLeftWall = head.x <= 0;
    bool atRightWall = head.x >= BOARD_SIZE_X - 1;

    //ignorowanie skrętu w ścianę
    if ((atTopWall && newDir == UP) ||
        (atBottomWall && newDir == DOWN) ||
        (atLeftWall && newDir == LEFT) ||
        (atRightWall && newDir == RIGHT)) {
        return;
    }

    //ignorowanie zawracania
    if ((direction == UP && newDir == DOWN) ||
        (direction == DOWN && newDir == UP) ||
        (direction == LEFT && newDir == RIGHT) ||
        (direction == RIGHT && newDir == LEFT)) {
        return;
    }

    nextDirection = newDir;
}

Direction Snake::getValidTurnDirection(const Point& pos) const {
    bool atTopWall = pos.y <= 0;
    bool atBottomWall = pos.y >= BOARD_SIZE_Y - 1;
    bool atLeftWall = pos.x <= 0;
    bool atRightWall = pos.x >= BOARD_SIZE_X - 1;

    //skeanie w rogach
    bool atTopLeftCorner = atTopWall && atLeftWall;
    bool atTopRightCorner = atTopWall && atRightWall;
    bool atBottomLeftCorner = atBottomWall && atLeftWall;
    bool atBottomRightCorner = atBottomWall && atRightWall;

    //skręcanie w rogach
    if (atTopLeftCorner) {
        return (direction == UP) ? RIGHT : DOWN;
    }
    if (atTopRightCorner) {
        return (direction == UP) ? LEFT : DOWN;
    }
    if (atBottomLeftCorner) {
        return (direction == DOWN) ? RIGHT : UP;
    }
    if (atBottomRightCorner) {
        return (direction == DOWN) ? LEFT : UP;
    }

    //skecanie w ściany
    if (atTopWall && direction == UP) return RIGHT;
    if (atBottomWall && direction == DOWN) return LEFT;
    if (atLeftWall && direction == LEFT) return UP;
    if (atRightWall && direction == RIGHT) return DOWN;

    return direction;
}

//czy w planszy?
bool Snake::isValidMove(const Point& newHead) const {
    return (newHead.x >= 0 && newHead.x < BOARD_SIZE_X &&
            newHead.y >= 0 && newHead.y < BOARD_SIZE_Y);
}

//przedłużanie snake'a
void Snake::grow() {
    if (length < MAX_SNAKE_LENGTH) {
        body[length] = body[length - 1];
        length++;
    }
}

void Snake::addScore(int points) {
    score += points;
}

//zmniejszanie
void Snake::shrink(int units) {
    length = (length - units < INITIAL_SNAKE_LENGTH) ? INITIAL_SNAKE_LENGTH : length - units;
}

bool Snake::update() {
    //poprzedni direction
    Direction previousDirection = direction;
    direction = nextDirection;
    
    Point newHead = body[0];
    
    //nowa pozycja głowy
    switch (direction) {
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
    }
    
    //kolizja w głową
    bool wallCollision = (newHead.x < 0 || newHead.x >= BOARD_SIZE_X ||
                         newHead.y < 0 || newHead.y >= BOARD_SIZE_Y);
    
    if (wallCollision) {
        //poprzednia pozycja
        direction = previousDirection;
        newHead = body[0];
        
        Direction wallDirection = getValidTurnDirection(body[0]);
        
        if (wallDirection != direction) {
            direction = wallDirection;
            nextDirection = wallDirection;
            
            //obliczanie pozycji
            switch (direction) {
                case UP:    newHead.y--; break;
                case DOWN:  newHead.y++; break;
                case LEFT:  newHead.x--; break;
                case RIGHT: newHead.x++; break;
            }
        }
    }
    
    //kolizja z ciałem
    for (int i = 0; i < length; i++) {
        if (newHead == body[i]) {
            return false;
        }
    }
    
    //aktualizacja snake'a
    for (int i = length - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }
    body[0] = newHead;
    
    return true;
}