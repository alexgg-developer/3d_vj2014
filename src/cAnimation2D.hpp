#pragma once
#include "cTexture.hpp"
#include "TypesDefined.hpp"
#include "SDL.h"
#include <vector>

class Animation2D
{
public:
  Texture const & mTexture;

  Animation2D(Texture const & t, uint images, uint framesPerAnimation, SDL_Renderer * const renderer);
  void setConstantSizeOfImage();
  void update(uint frame);
  void draw();

private:
  uint mImages, mFramesPerAnimation, mFrame;
  bool mSizeConstantOfImage;
  std::vector<SDL_Rect> mClip;
  SDL_Renderer * const mRenderer;
};


