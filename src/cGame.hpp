#pragma once
#include <iostream>
#include "cInput.hpp"
#include "cTimer.hpp"
#include "cWindow.hpp"
#include "cRenderer.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <GL/glew.h>
#include <GL/glu.h>

class Game
{
public:
  Game();
  int main();
private:
  Window mWindow;
  Input mInput;
  Timer mTimer;
  Renderer mRenderer;

  int init();
  int initSDL();
  int initGLEW();
  int initGL();
  int quit();
  void logic(float const dt);

};
