#pragma once
#include <iostream>
#include "cInput.hpp"
#include "cTimer.hpp"
#include "cWindow.hpp"
#include "cRenderer.hpp"
#include "cMusic.hpp"
#include "cHud.hpp"
#include "cMenu.hpp"
#include "cSound.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include "LevelManager.hpp"

class Game
{
public:
  Game();
  int init();
  int main();
private:
  size_t mMenuOption;
  Window mWindow;
  Input mInput;
  Timer mTimer;
  Renderer mRenderer;
  Music mMusic;
  Hud mHud;
  Menu mMenu;
  Sound mWinSound, mLoseSound;
  bool mInMenu;
  bool mQuitDone;
  size_t mLevel;
  int initSDL();
  int initGLEW();
  int initGL();
  int quit();
  void logic(float const dt, LevelManager & lm, float const te);

};
