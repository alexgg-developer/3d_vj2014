#pragma once
#include "SDL.h"

class Texture
{
public:
  Texture();
  ~Texture();
  bool load(const char* fileName);

  SDL_Surface* mTextureSurface;
};
