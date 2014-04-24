#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>

class Texture
{
public:
  Texture();
  ~Texture();
  bool load(std::string fileName, SDL_Renderer* renderer );

  SDL_Texture* mTexture;
};
