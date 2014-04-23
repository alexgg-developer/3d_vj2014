#pragma once
#include "SDL.h"
#include <iostream>

class Game
{
  static const int WINDOW_WIDTH  = 800;
  static const int WINDOW_HEIGHT = 600;
public:
    Game();
    int main();
private:
    int init();
    int initSDL();
    int quit();
    SDL_Window* mWindow;
    SDL_Surface* mScreenSurface;
};
