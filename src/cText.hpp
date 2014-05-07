#pragma once
#include "SDL_ttf.h"
#include "SDL.h"
#include "TypesDefined.hpp"
#include "vec3.hpp"
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>

class Text
{

public:
  Text();
  ~Text();
  bool loadFont(std::string path, uint size);
  bool loadText(std::string text, SDL_Color color, Quality q);
  void free();
  void draw();
  void setPosition(vec3 const & position);
  uint mWidth, mHeight;
private:
  TTF_Font* mFont;
  GLuint mTexture;
  SDL_Surface* mTextureSurface;
  vec3 mPosition;
};
