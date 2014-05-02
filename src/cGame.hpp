#pragma once
#include <iostream>
#include "cInput.hpp"
#include "cTimer.hpp"
#include "cWindow.hpp"
#include "SDL.h"
#include "SDL_image.h"
class Game
{
public:

  Game();
  int main();
private:
  Window mWindow;
  Input mInput;
  Timer mTimer;

  int init();
  int initSDL();
  int quit();
};
