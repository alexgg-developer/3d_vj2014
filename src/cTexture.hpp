#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "vec3.hpp"
#include <string>

class Texture
{
public:
  Texture();
  ~Texture();
  bool load(std::string fileName, SDL_Renderer* renderer );
  bool load(std::string fileName, SDL_Renderer * renderer, vec3 const & colorKey );
  void free();

  SDL_Texture* mTexture;
  int mWidth, mHeight;
};
