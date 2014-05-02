#pragma once
#include "SDL_ttf.h"
#include "SDL.h"
#include "TypesDefined.hpp"
#include "vec3.hpp"
#include <string>

class Text
{

public:
  Text(SDL_Renderer * const renderer);
  ~Text();
  bool loadFont(std::string path, uint size);
  bool loadText(std::string text, SDL_Color color, Quality q);
  void free();
  void draw();
  void setPosition(vec3 const & position);
  uint mWidth, mHeight;
private:
  TTF_Font* mFont;
  SDL_Texture* mTexture;
  SDL_Renderer * const mRenderer;
  vec3 mPosition;
};
