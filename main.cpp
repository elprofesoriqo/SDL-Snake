#include <math.h>
#include <stdio.h>
#include <string.h>

extern "C" {
#include "SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_main.h"
}

#include "game.h"
#include "renderer.h"
#include "constants.h"

// Obsługa klawiszy
void handleKeyPress(SDL_Keycode key, Game& game, bool& quit) {
    switch (key) {
        case SDLK_ESCAPE:
            quit = true;
            break;
        case SDLK_RETURN:
            if (game.getState() == MENU) {
                game.reset();
                game.setState(PLAYING);
            }
            break;
        case SDLK_n:
            if (game.getState() == GAME_OVER) {
                game.reset();
                game.setState(PLAYING);
            }
            break;
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_LEFT:
        case SDLK_RIGHT:
            if (game.getState() == PLAYING) {
                Direction dir = (key == SDLK_UP) ? UP :
                                (key == SDLK_DOWN) ? DOWN :
                                (key == SDLK_LEFT) ? LEFT : RIGHT;
                game.getSnake().setDirection(dir);
            }
            break;
    }
}

// Błędy SDL
void exitWithError(const char* message) {
    printf("%s error: %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(1);
}

// Inicjalizacja SDL
void initSDL(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& charset, SDL_Surface*& screen, SDL_Texture*& scrtex) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        exitWithError("SDL_Init");
    }

    // Tworzenie okna i renderera
    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        exit(1);
    }

    // Ładowanie czcionki
    charset = SDL_LoadBMP("./cs8x8.bmp");
    if (charset == NULL) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    SDL_SetColorKey(charset, true, 0x000000);

    // Renderer
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(window, "Snake Game");

    // Powierzchnia ekranu i tekstury
    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                  0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING,
                              SCREEN_WIDTH, SCREEN_HEIGHT);
}

// Inicjalizacja gry
void initGame(Game& game) {
    game.setState(MENU);
}

// Renderowanie ekranu
void renderScreen(SDL_Renderer* renderer, SDL_Texture* scrtex, SDL_Surface* screen, Game& game, SDL_Surface* charset) {
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

    Renderer::DrawFrame(screen);

    // Ekrany/komponenty
    switch (game.getState()) {
        case MENU:
            Renderer::DrawMenu(screen, charset);
            break;
        case PLAYING:
            Renderer::DrawBoard(screen, game.getSnake());
            Renderer::DrawInfoPanel(screen, charset, game.getGameTime(), game.getSnake(), game.getSpeedMultiplier(), game.getFood());
            Renderer::DrawFood(screen, game.getFood());
            break;
        case GAME_OVER:
            Renderer::DrawBoard(screen, game.getSnake());
            Renderer::DrawGameOver(screen, charset, game.getSnake(), game.getFinalTime());
            if (game.shouldExitGameOver()) {
                game.setState(MENU);
            }
            break;
    }

    // Aktualizacja ekranu
    SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, scrtex, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* charset;
    SDL_Surface* screen;
    SDL_Texture* scrtex;

    // Inicjalizacja SDL, gry i zasobów
    initSDL(window, renderer, charset, screen, scrtex);
    Game game;
    initGame(game);
    bool quit = false;
    int t1 = SDL_GetTicks();

    // Główna pętla gry
    while (!quit) {
        // Czas
        int t2 = SDL_GetTicks();
        double delta = (t2 - t1) * 0.001;
        t1 = t2;

        // Zdarzenia SDL
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                handleKeyPress(event.key.keysym.sym, game, quit);
            }
        }

        game.update(delta);

        // Renderowanie ekranu
        renderScreen(renderer, scrtex, screen, game, charset);
    }

    // Zwalnianie zasobów
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
