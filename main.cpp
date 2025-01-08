#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <random>
#include <time.h>

extern "C" {
#include "SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define BOARD_SIZE_X 40
#define BOARD_SIZE_Y 30
#define CELL_SIZE 20
#define BOARD_OFFSET_X 20
#define BOARD_OFFSET_Y ((SCREEN_HEIGHT - (BOARD_SIZE_Y * CELL_SIZE)) / 2)
#define INITIAL_SNAKE_LENGTH 3
#define INFO_PANEL_X (BOARD_OFFSET_X + BOARD_SIZE_X * CELL_SIZE + 20)
#define INFO_PANEL_WIDTH 200

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    QUIT
};

struct Point {
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class Snake {
private:
    std::vector<Point> body;
    Direction direction;
    Direction nextDirection;
    bool isDead;
    int score;

public:
    Snake() {
        reset();
    }

    void reset() {
        body.clear();
        int startX = BOARD_SIZE_X / 2;
        int startY = BOARD_SIZE_Y / 2;

        for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
            body.push_back(Point(startX - i, startY));
        }

        direction = RIGHT;
        nextDirection = RIGHT;
        isDead = false;
        score = 0;
    }

    void setDirection(Direction newDir) {
        if ((direction == UP && newDir == DOWN) ||
            (direction == DOWN && newDir == UP) ||
            (direction == LEFT && newDir == RIGHT) ||
            (direction == RIGHT && newDir == LEFT)) {
            return;
        }
        nextDirection = newDir;
    }

    bool update() {
        if (isDead) return false;

        direction = nextDirection;
        Point newHead = body.front();

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

        for (const Point& p : body) {
            if (newHead == p) {
                isDead = true;
                return false;
            }
        }

        body.insert(body.begin(), newHead);
        body.pop_back();
        return true;
    }

    // Non-const version
    std::vector<Point>& getBody() { return body; }

    // Const version
    const std::vector<Point>& getBody() const { return body; }

    bool getIsDead() const { return isDead; }
    int getScore() const { return score; }
    void addScore(int points) { score += points; }
};

class Game {
private:
    GameState state;
    Snake snake;
    double worldTime;
    double stateTimer;
    bool exitConfirmed;

public:
    Game() : state(MENU), worldTime(0), stateTimer(0), exitConfirmed(false) {}

    void reset() {
        snake.reset();
        worldTime = 0;
        stateTimer = 0;
        state = PLAYING;
    }

    void update(double delta) {
        worldTime += delta;
        stateTimer += delta;

        switch (state) {
        case GAME_OVER:
            if (stateTimer >= 5.0) {
                state = MENU;
                stateTimer = 0;
            }
            break;
        default:
            break;
        }
    }
    bool shouldExitGameOver() const {
        return state == GAME_OVER && stateTimer >= 5.0;
    }

    GameState getState() const { return state; }
    Snake& getSnake() { return snake; }
    double getWorldTime() const { return worldTime; }
    void setState(GameState newState) {
        state = newState;
        stateTimer = 0;
    }
};

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = 8;
    d.h = 8;
    while (*text) {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = x;
        d.y = y;
        SDL_BlitSurface(charset, &s, screen, &d);
        x += 8;
        text++;
    }
}

void DrawFrame(SDL_Surface* screen) {
    Uint32 frameColor = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);

    // Rysowanie obramowania
    SDL_Rect topFrame = { 0, 0, SCREEN_WIDTH, 2 };
    SDL_Rect bottomFrame = { 0, SCREEN_HEIGHT - 2, SCREEN_WIDTH, 2 };
    SDL_Rect leftFrame = { 0, 0, 2, SCREEN_HEIGHT };
    SDL_Rect rightFrame = { SCREEN_WIDTH - 2, 0, 2, SCREEN_HEIGHT };

    SDL_FillRect(screen, &topFrame, frameColor);
    SDL_FillRect(screen, &bottomFrame, frameColor);
    SDL_FillRect(screen, &leftFrame, frameColor);
    SDL_FillRect(screen, &rightFrame, frameColor);
}

void DrawInfoPanel(SDL_Surface* screen, SDL_Surface* charset, double worldTime, const Snake& snake) {
    Uint32 white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
    Uint32 gray = SDL_MapRGB(screen->format, 0x80, 0x80, 0x80);

    SDL_Rect panelRect = {
        INFO_PANEL_X,
        BOARD_OFFSET_Y,
        INFO_PANEL_WIDTH,
        BOARD_SIZE_Y * CELL_SIZE
    };
    SDL_FillRect(screen, &panelRect, gray);

    char text[128];

    sprintf(text, "Czas gry: %.1f s", worldTime);
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 20, text, charset);

    sprintf(text, "Punkty: %d", snake.getScore());
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 40, text, charset);

    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 80, "Sterowanie:", charset);
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 100, "Strzalki - ruch", charset);
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 120, "N - nowa gra", charset);
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 140, "ESC - wyjscie", charset);
}

void DrawBoard(SDL_Surface* screen, const Snake& snake) {
    Uint32 black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    Uint32 white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
    Uint32 green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);

    SDL_FillRect(screen, NULL, black);

    SDL_Rect boardRect = {
        BOARD_OFFSET_X - 1,
        BOARD_OFFSET_Y - 1,
        BOARD_SIZE_X * CELL_SIZE + 2,
        BOARD_SIZE_Y * CELL_SIZE + 2
    };
    SDL_FillRect(screen, &boardRect, white);

    const std::vector<Point>& snakeBody = snake.getBody();
    for (const Point& p : snakeBody) {
        SDL_Rect snakeRect = {
            BOARD_OFFSET_X + p.x * CELL_SIZE,
            BOARD_OFFSET_Y + p.y * CELL_SIZE,
            CELL_SIZE - 1,
            CELL_SIZE - 1
        };
        SDL_FillRect(screen, &snakeRect, green);
    }
}

void DrawMenu(SDL_Surface* screen, SDL_Surface* charset) {
    Uint32 textColor = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);

    const char* title = "SNAKE GAME";
    const char* startPrompt = "Nacisnij ENTER aby rozpoczac gre";

    DrawString(screen, (SCREEN_WIDTH - strlen(title) * 8) / 2,
        SCREEN_HEIGHT / 3, title, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(startPrompt) * 8) / 2,
        SCREEN_HEIGHT / 2, startPrompt, charset);
}


struct Dot {
    Point position;
    bool active;
    int type; // 0 for blue, 1 for red

    Dot(Point pos = Point(-1, -1), bool isActive = false, int dotType = 0)
        : position(pos), active(isActive), type(dotType) {}
};

void DrawDot(SDL_Surface* screen, const Dot& dot) {
    Uint32 color = (dot.type == 0) ? SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF) : SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    SDL_Rect rect = {
        BOARD_OFFSET_X + dot.position.x * CELL_SIZE,
        BOARD_OFFSET_Y + dot.position.y * CELL_SIZE,
        CELL_SIZE - 1,
        CELL_SIZE - 1
    };
    SDL_FillRect(screen, &rect, color);
}

Point generateRandomPosition() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> xDist(0, BOARD_SIZE_X - 1);
    static std::uniform_int_distribution<> yDist(0, BOARD_SIZE_Y - 1);

    return Point(xDist(gen), yDist(gen));
}

void DrawGameOver(SDL_Surface* screen, SDL_Surface* charset, const Snake& snake, double worldTime) {
    Uint32 textColor = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);

    char scoreText[128];
    char timeText[128];
    const char* gameOverText = "GAME OVER";
    sprintf(scoreText, "Twoj wynik: %d", snake.getScore());
    sprintf(timeText, "Czas gry: %.1f s", worldTime);

    DrawString(screen, (SCREEN_WIDTH - strlen(gameOverText) * 8) / 2,
        SCREEN_HEIGHT / 3, gameOverText, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(scoreText) * 8) / 2,
        SCREEN_HEIGHT / 2, scoreText, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(timeText) * 8) / 2,
        SCREEN_HEIGHT / 2 + 20, timeText, charset);
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface* charset = SDL_LoadBMP("./cs8x8.bmp");
    if (charset == NULL) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetColorKey(charset, true, 0x000000);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(window, "Snake Game");

    SDL_Surface* screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Texture* scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    Game game;
    bool quit = false;
    double moveTimer = 0;
    const double moveInterval = 0.15;
    double speedUpTimer = 0;
    const double speedUpInterval = 10.0;
    double redDotTimer = 0;
    const double redDotDuration = 5.0;
    double redDotCooldown = 0;
    const double redDotSpawnInterval = 30.0;

    Dot blueDot(generateRandomPosition(), true, 0);
    Dot redDot;

    int t1 = SDL_GetTicks();
    while (!quit) {
        int t2 = SDL_GetTicks();
        double delta = (t2 - t1) * 0.001;
        t1 = t2;

        game.update(delta);
        moveTimer += delta;
        speedUpTimer += delta;
        redDotCooldown += delta;

        if (speedUpTimer >= speedUpInterval) {
            speedUpTimer -= speedUpInterval;
            moveTimer *= 0.8; // Increase game speed
        }

        if (!redDot.active && redDotCooldown >= redDotSpawnInterval) {
            redDotCooldown = 0;
            redDot = Dot(generateRandomPosition(), true, 1);
            redDotTimer = 0;
        }

        if (redDot.active) {
            redDotTimer += delta;
            if (redDotTimer >= redDotDuration) {
                redDot.active = false;
            }
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    if (game.getState() == PLAYING) {
                        game.setState(GAME_OVER);
                    }
                    else {
                        quit = true;
                    }
                    break;
                case SDLK_RETURN:
                    if (game.getState() == MENU) {
                        game.reset();
                    }
                    break;
                case SDLK_n:
                    if (game.getState() != MENU) {
                        game.reset();
                    }
                    break;
                case SDLK_UP:
                case SDLK_DOWN:
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    if (game.getState() == PLAYING) {
                        Direction dir = (event.key.keysym.sym == SDLK_UP) ? UP :
                            (event.key.keysym.sym == SDLK_DOWN) ? DOWN :
                            (event.key.keysym.sym == SDLK_LEFT) ? LEFT : RIGHT;
                        game.getSnake().setDirection(dir);
                    }
                    break;
                }
                break;
            case SDL_QUIT:
                quit = true;
                break;
            }
        }

        if (game.getState() == PLAYING && moveTimer >= moveInterval) {
            moveTimer -= moveInterval;
            if (!game.getSnake().update()) {
                game.setState(GAME_OVER);
            }
            else {
                Point head = game.getSnake().getBody().front();
                if (blueDot.active && head == blueDot.position) {
                    game.getSnake().addScore(10);
                    Point tail = game.getSnake().getBody().back();
                    game.getSnake().getBody().push_back(tail);
                    blueDot = Dot(generateRandomPosition(), true, 0);
                }

                if (redDot.active && head == redDot.position) {
                    game.getSnake().addScore(20);
                    Point tail = game.getSnake().getBody().back();
                    game.getSnake().getBody().push_back(tail);
                    game.getSnake().getBody().push_back(tail);
                    redDot.active = false;
                }
            }
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

        DrawFrame(screen);

        switch (game.getState()) {
        case MENU:
            DrawMenu(screen, charset);
            break;

        case PLAYING:
            DrawBoard(screen, game.getSnake());
            DrawInfoPanel(screen, charset, game.getWorldTime(), game.getSnake());
            DrawDot(screen, blueDot);
            if (redDot.active) DrawDot(screen, redDot);
            break;

        case GAME_OVER:
            DrawBoard(screen, game.getSnake());
            DrawGameOver(screen, charset, game.getSnake(), game.getWorldTime());
            if (game.shouldExitGameOver()) {
                quit = true;
            }
            break;
        }

        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
