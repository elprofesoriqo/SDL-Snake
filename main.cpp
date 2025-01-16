// main.cpp
#define _USE_MATH_DEFINES
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

// Funkcja główna
int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    // Tworzenie okna i renderera
    SDL_Window* window;
    SDL_Renderer* renderer;
    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    }

    // Ładowanie czcionki
    SDL_Surface* charset = SDL_LoadBMP("./cs8x8.bmp");
    if (charset == NULL) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_SetColorKey(charset, true, 0x000000);

    // Konfiguracja renderera
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(window, "Snake Game");

    // Tworzenie powierzchni ekranu i tekstury
    SDL_Surface* screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Texture* scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           SCREEN_WIDTH, SCREEN_HEIGHT);

    // Inicjalizacja gry
    Game game;
    bool quit = false;
    int t1 = SDL_GetTicks();

    // Główna pętla gry
    while (!quit) {
        // Obliczanie delta time
        int t2 = SDL_GetTicks();
        double delta = (t2 - t1) * 0.001;
        t1 = t2;

        // Obsługa zdarzeń SDL
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            quit = true;  // Bezpośrednie wyjście z gry
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
                                Direction dir = (event.key.keysym.sym == SDLK_UP) ? UP :
                                              (event.key.keysym.sym == SDLK_DOWN) ? DOWN :
                                              (event.key.keysym.sym == SDLK_LEFT) ? LEFT : RIGHT;
                                game.getSnake().setDirection(dir);
                            }
                            break;
                    }
                    break;
            }
        }

        // Aktualizacja stanu gry
        game.update(delta);

        // Czyszczenie ekranu
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

        // Rysowanie ramki
        Renderer::DrawFrame(screen);

        // Rysowanie odpowiedniego ekranu w zależności od stanu gry
        switch (game.getState()) {
            case MENU:
                Renderer::DrawMenu(screen, charset);
                break;

            case PLAYING:
                Renderer::DrawBoard(screen, game.getSnake());
                Renderer::DrawInfoPanel(screen, charset, game.getWorldTime(), game.getSnake());
                Renderer::DrawFood(screen, game.getFood());
                break;

            case GAME_OVER:
                Renderer::DrawBoard(screen, game.getSnake());
                Renderer::DrawGameOver(screen, charset, game.getSnake(), game.getWorldTime());
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

    // Zwalnianie zasobów
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}