#pragma once
#include <SDL.h>
#include "game.h"
#include "food.h"

class Renderer {
public:
    static void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
    static void DrawFrame(SDL_Surface* screen);
    static void DrawBoard(SDL_Surface* screen, const Snake& snake);
    static void DrawInfoPanel(SDL_Surface* screen, SDL_Surface* charset, double worldTime, const Snake& snake);
    static void DrawMenu(SDL_Surface* screen, SDL_Surface* charset);
    static void DrawGameOver(SDL_Surface* screen, SDL_Surface* charset, const Snake& snake, double worldTime);
    static void DrawFood(SDL_Surface* screen, const Food& food);
};
