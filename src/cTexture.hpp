#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "vec3.hpp"
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>

class Texture
{
public:
  Texture();
  ~Texture();
  bool load(std::string fileName);
  bool load(std::string fileName, vec3 const & colorKey );
  void draw();
  void free();

  GLuint mTexture;
  SDL_Surface* mTextureSurface;
  int mWidth, mHeight;
};
