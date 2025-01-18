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



//sprawdzenie wymiarów 
bool validateScreenDimensions() {
    //ekran wzg planszy
    const int minScreenWidth = BOARD_OFFSET_X + (BOARD_SIZE_X * CELL_SIZE) + 20 + INFO_PANEL_WIDTH + 20;
    const int minScreenHeight = BOARD_OFFSET_Y * 2 + (BOARD_SIZE_Y * CELL_SIZE);

    const int minCellSize = 15;
    
    if (SCREEN_WIDTH < minScreenWidth) {
        printf("Blad: Szerokosc ekranu jest zbyt mala! Minimalny wymagany rozmiar: %d\n", minScreenWidth);
        return false;
    }
    if (SCREEN_HEIGHT < minScreenHeight) {
        printf("Blad: Wysokosc ekranu jest zbyt mala! Minimalny wymagany rozmiar: %d\n", minScreenHeight);
        return false;
    }
    if (CELL_SIZE < minCellSize) {
        printf("Blad: Rozmiar komorki jest zbyt maly! Minimalny wymagany rozmiar: %d\n", minCellSize);
        return false;
    }
    if (INFO_PANEL_WIDTH < 150) {
        printf("Blad: Szerokosc panelu informacyjnego jest zbyt mala! Minimalny wymagany rozmiar: 150\n");
        return false;
    }

    return true;
}



//obsługa klawiszy
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
            if (game.getState() == GAME_OVER || PLAYING) {
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

//błędy SDL
void exitWithError(const char* message) {
    printf("%s error: %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(1);
}

//init SDL
void initSDL(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& charset, SDL_Surface*& screen, SDL_Texture*& scrtex) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        exitWithError("SDL_Init");
    }

    //tworzenie okna i renderera
    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        exit(1);
    }

    //czcionki
    charset = SDL_LoadBMP("./cs8x8.bmp");
    if (charset == NULL) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    SDL_SetColorKey(charset, true, 0x000000);

    //renderer
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(window, "Snake Game");

    //powierzchnia ekranu i textury
    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                  0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING,
                              SCREEN_WIDTH, SCREEN_HEIGHT);
}

//init gry
void initGame(Game& game) {
    game.setState(MENU);
}

//render ekranu
void renderScreen(SDL_Renderer* renderer, SDL_Texture* scrtex, SDL_Surface* screen, Game& game, SDL_Surface* charset) {
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

    Renderer::DrawFrame(screen);

    //ekrany/komponenty
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

    //up ekranu
    SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, scrtex, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {

   if (!validateScreenDimensions()) {
    printf("\nProsze dostosowac wymiary ekranu w constants.h zgodnie z powyzszymi wymaganiami.\n");
    printf("Zalecane wartosci:\n");
    printf("SCREEN_WIDTH >= 1000\n");
    printf("SCREEN_HEIGHT >= 600\n");
    printf("CELL_SIZE >= 15\n");
    printf("INFO_PANEL_WIDTH >= 150\n");
    return 1;
    }


    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* charset;
    SDL_Surface* screen;
    SDL_Texture* scrtex;

    //init sdl
    initSDL(window, renderer, charset, screen, scrtex);
    Game game;
    initGame(game);
    bool quit = false;
    int t1 = SDL_GetTicks();

    while (!quit) {
        //czas/klatki
        int t2 = SDL_GetTicks();
        double delta = (t2 - t1) * 0.001;
        t1 = t2;

        //SDL
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                handleKeyPress(event.key.keysym.sym, game, quit);
            }
        }

        game.update(delta);
        renderScreen(renderer, scrtex, screen, game, charset);
    }

    //czyszczneie pamięci
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
