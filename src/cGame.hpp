#pragma once
#include <iostream>
#include "cInput.hpp"
#include "cTimer.hpp"
#include "SDL.h"
#include "SDL_image.h"
class Game
{
public:
  static const int WINDOW_WIDTH  = 800;
  static const int WINDOW_HEIGHT = 600;

  Game();
  int main();
private:
  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
  Input mInput;
  Timer mTimer;

  int init();
  int initSDL();
  int quit();
};
