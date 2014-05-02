#pragma once
#include "SDL.h"
#include "TypesDefined.hpp"

class Window
{
  static const uint DEFAULT_WINDOW_WIDTH  = 800;
  static const uint DEFAULT_WINDOW_HEIGHT = 600;
public:
  uint mWidth, mHeight;
  bool mFullScreen;
  bool mMinimized;
  SDL_Renderer* mRenderer;

  Window();
  int init();
  SDL_Renderer* createRenderer();
  void free();
  void handleEvent(SDL_Event const & event);
  void switchFullScreen();
private:
  SDL_Window*   mWindow;
};
