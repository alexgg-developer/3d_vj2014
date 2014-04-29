#include "cText.hpp"
#include <iostream>

Text::Text(SDL_Renderer * const renderer): mFont(NULL), mTexture(NULL), mWidth(0), mHeight(0), mRenderer(renderer), mPosition(vec3(0,0))
{
}

Text::~Text()
{
  //free();
}

bool Text::loadFont(std::string path, uint size)
{
  bool success = true;
  mFont = TTF_OpenFont( path.c_str(), size);
   if( mFont == NULL ) {
       std::cout << "Failed to load lazy font! SDL_ttf Error: " <<  TTF_GetError() << std::endl;
       success = false;
   }
  return success;
}

bool Text::loadText(std::string text, SDL_Color color, Quality q)
{
  bool success = true;
  SDL_Surface* textSurface = NULL;
  switch(q) {
    case LOW:
      textSurface = TTF_RenderText_Solid( mFont, text.c_str(), color);
    break;
    case MED: {
      SDL_Color bg = {255, 255, 255, 0};
      textSurface = TTF_RenderText_Shaded( mFont, text.c_str(), color, bg );
    }
    break;
    case HIGH:
      textSurface = TTF_RenderText_Blended( mFont, text.c_str(), color);
    break;
  }
  if( textSurface == NULL ) {
      std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
      success = false;
  }
  else {
      mTexture = SDL_CreateTextureFromSurface( mRenderer, textSurface );
      if( mTexture == NULL ) {
          std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
          success = false;
      }
      else  {
          mWidth  = textSurface->w;
          mHeight = textSurface->h;
      }
      SDL_FreeSurface( textSurface );
  }

  return success;
}

void Text::free()
{
  SDL_DestroyTexture( mTexture );
  mTexture = NULL;
  TTF_CloseFont( mFont );
  mFont = NULL;
}

void Text::setPosition(vec3 const & position)
{
  mPosition = position;
}

void Text::draw()
{
  SDL_Rect rectPos = { static_cast<int>(mPosition.x), static_cast<int>(mPosition.y), static_cast<int>(mWidth), static_cast<int>(mHeight) };
  SDL_RenderCopy( mRenderer, mTexture, NULL, &rectPos );
}
