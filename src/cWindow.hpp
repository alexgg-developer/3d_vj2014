#pragma once
#include "SDL.h"
#include "TypesDefined.hpp"

class Window
{
  static const uint DEFAULT_WINDOW_WIDTH  = 800;
  static const uint DEFAULT_WINDOW_HEIGHT = 600;
  //static const uint DEFAULT_WINDOW_WIDTH = 1920;
  //static const uint DEFAULT_WINDOW_HEIGHT = 1080;
public:
  uint mWidth, mHeight;
  bool mFullScreen;
  bool mMinimized;
  SDL_Window*    mWindow;

  Window();
  int init();
  bool createGLContext();
  void free();
  void handleEvent(SDL_Event const & event);
  void switchFullScreen();
private:
  SDL_GLContext mContext;
};
