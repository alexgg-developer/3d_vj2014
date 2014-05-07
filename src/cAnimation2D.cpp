#include "cAnimation2D.hpp"

Animation2D::Animation2D(Texture const & t, uint images, uint framesPerAnimation, SDL_Renderer * const renderer):
  mTexture(t), mImages(images), mFramesPerAnimation(framesPerAnimation), mRenderer(renderer)
{}

void Animation2D::update(uint frame)
{
  mFrame = frame % (mImages * mFramesPerAnimation);
}

void Animation2D::setConstantSizeOfImage()
{
  mSizeConstantOfImage = true;
  SDL_Rect clip = {0, 0, int(mTexture.mWidth / mImages), mTexture.mHeight};
  mClip.push_back(clip);
}

void Animation2D::draw()
{
  /*if(mSizeConstantOfImage) {
    SDL_Rect rectPos = { 400, 300, mClip[0].w, mClip[0].h };
    SDL_Rect rectToDraw = {0 + mClip[0].w * int(mFrame / mFramesPerAnimation), 0, mClip[0].w, mClip[0].h };
    SDL_RenderCopy( mRenderer, mTexture.mTexture, &rectToDraw, &rectPos );
  }*/
}
