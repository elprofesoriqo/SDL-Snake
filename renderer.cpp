#include "renderer.h"
#include <stdio.h>
#include "constants.h"
#include <cstring>

void Renderer::DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
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
void Renderer::DrawFrame(SDL_Surface* screen) {
    // Czarne tło
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
    
    // Białe obramowanie planszy gry
    SDL_Rect boardArea = {
        BOARD_OFFSET_X - 1,
        BOARD_OFFSET_Y - 1,
        BOARD_SIZE_X * CELL_SIZE + 2,
        BOARD_SIZE_Y * CELL_SIZE + 2
    };
    SDL_FillRect(screen, &boardArea, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

void Renderer::DrawBoard(SDL_Surface* screen, const Snake& snake) {
    Uint32 black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    Uint32 white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
    Uint32 green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
    
    SDL_Rect boardRect = {
        BOARD_OFFSET_X - 1,
        BOARD_OFFSET_Y - 1,
        BOARD_SIZE_X * CELL_SIZE + 2,
        BOARD_SIZE_Y * CELL_SIZE + 2
    };
    SDL_FillRect(screen, &boardRect, white);
    
    const Point* snakeBody = snake.getBody();
    for (int i = 0; i < snake.getLength(); i++) {
        SDL_Rect snakeRect = {
            BOARD_OFFSET_X + snakeBody[i].x * CELL_SIZE,
            BOARD_OFFSET_Y + snakeBody[i].y * CELL_SIZE,
            CELL_SIZE - 1,
            CELL_SIZE - 1
        };
        SDL_FillRect(screen, &snakeRect, green);
    }
}

void Renderer::DrawFood(SDL_Surface* screen, const Food& food) {
    Uint32 blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
    Uint32 red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    
    // Rysowanie niebieskiego jedzenia
    SDL_Rect blueRect = {
        BOARD_OFFSET_X + food.getBlueFood().x * CELL_SIZE,
        BOARD_OFFSET_Y + food.getBlueFood().y * CELL_SIZE,
        CELL_SIZE - 1,
        CELL_SIZE - 1
    };
    SDL_FillRect(screen, &blueRect, blue);
    
    // Rysowanie czerwonego jedzenia
    if (food.isRedFoodActive()) {
        SDL_Rect redRect = {
            BOARD_OFFSET_X + food.getRedFood().x * CELL_SIZE,
            BOARD_OFFSET_Y + food.getRedFood().y * CELL_SIZE,
            CELL_SIZE - 1,
            CELL_SIZE - 1
        };
        SDL_FillRect(screen, &redRect, red);
    }
}

void Renderer::DrawInfoPanel(SDL_Surface* screen, SDL_Surface* charset, double worldTime, const Snake& snake) {
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
    
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 100, "Sterowanie:", charset);
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 120, "Strzalki - ruch", charset);
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 140, "N - nowa gra", charset);
    DrawString(screen, INFO_PANEL_X + 10, BOARD_OFFSET_Y + 160, "ESC - wyjscie", charset);
}


void Renderer::DrawMenu(SDL_Surface* screen, SDL_Surface* charset) {
    // Czarne tło
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
    
    const char* title = "SNAKE GAME";
    const char* startPrompt = "Nacisnij ENTER aby rozpoczac gre";
    const char* exitPrompt = "ESC - wyjscie";
    
    // Białe napisy
    DrawString(screen, (SCREEN_WIDTH - strlen(title) * 8) / 2,
               SCREEN_HEIGHT / 3, title, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(startPrompt) * 8) / 2,
               SCREEN_HEIGHT / 2, startPrompt, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(exitPrompt) * 8) / 2,
               SCREEN_HEIGHT / 2 + 30, exitPrompt, charset);
}

void Renderer::DrawGameOver(SDL_Surface* screen, SDL_Surface* charset, const Snake& snake, double finalTime) {
    // Czarne tło
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
    
    const char* gameOverText = "KONIEC GRY";
    char scoreText[128];
    char timeText[128];
    const char* restartPrompt = "N - nowa gra";
    const char* exitPrompt = "ESC - wyjscie";
    
    sprintf(scoreText, "Twoj wynik: %d punktow", snake.getScore());
    sprintf(timeText, "Czas gry: %.1f sekund", finalTime);
    
    
    // Białe napisy
    DrawString(screen, (SCREEN_WIDTH - strlen(gameOverText) * 8) / 2,
               SCREEN_HEIGHT / 3, gameOverText, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(scoreText) * 8) / 2,
               SCREEN_HEIGHT / 2, scoreText, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(timeText) * 8) / 2,
               SCREEN_HEIGHT / 2 + 20, timeText, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(restartPrompt) * 8) / 2,
               SCREEN_HEIGHT / 2 + 50, restartPrompt, charset);
    DrawString(screen, (SCREEN_WIDTH - strlen(exitPrompt) * 8) / 2,
               SCREEN_HEIGHT / 2 + 70, exitPrompt, charset);
}