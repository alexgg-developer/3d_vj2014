#include "cText.hpp"
#include <iostream>

Text::Text(): mFont(nullptr), mTexture(0), mWidth(0), mHeight(0), mPosition(vec3(0,0)), mTextureSurface(nullptr)
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
  switch(q) {
    case LOW:
      mTextureSurface = TTF_RenderText_Solid( mFont, text.c_str(), color);
    break;
    case MED: {
      SDL_Color bg = {255, 255, 255, 0};
      mTextureSurface = TTF_RenderText_Shaded( mFont, text.c_str(), color, bg );
    }
    break;
    case HIGH:
      mTextureSurface = TTF_RenderText_Blended( mFont, text.c_str(), color);
    break;
  }
  if( mTextureSurface == nullptr ) {
      std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
      success = false;
  }
  else {
    mWidth  = mTextureSurface->w;
    mHeight = mTextureSurface->h;
    glGenTextures( 1, &mTexture );
    glBindTexture( GL_TEXTURE_2D, mTexture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTextureSurface->pixels );
    glBindTexture( GL_TEXTURE_2D, 0 );
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ) {
        std::cout << "Error loading texture" << gluErrorString( error ) << std::endl;
        success = false;
    }

  }

  return success;
}

void Text::free()
{
  //SDL_DestroyTexture( mTexture );
  if( mTexture != 0 ) {
    glDeleteTextures( 1, &mTexture );
    mTexture = 0;
  }
  SDL_FreeSurface( mTextureSurface );
  mTextureSurface = nullptr;
  TTF_CloseFont( mFont );
  mFont = nullptr;
}

void Text::setPosition(vec3 const & position)
{
  mPosition = position;
}

void Text::draw()
{
  SDL_Rect rectPos = { static_cast<int>(mPosition.x), static_cast<int>(mPosition.y), static_cast<int>(mWidth), static_cast<int>(mHeight) };
  glBindTexture( GL_TEXTURE_2D, mTexture );
  glBegin( GL_QUADS );
    glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f,            0.f );
    glTexCoord2f( 1.f, 0.f ); glVertex2f( mWidth,            0.f );
    glTexCoord2f( 1.f, 1.f ); glVertex2f( mWidth, mHeight );
    glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f, mHeight );
  glEnd();
}
