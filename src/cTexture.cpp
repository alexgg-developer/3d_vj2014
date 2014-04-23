#include "cTexture.hpp"
#include <iostream>

Texture::Texture(): mTextureSurface(NULL)
{}

Texture::~Texture()
{
  SDL_FreeSurface( mTextureSurface );
}

bool Texture::load(const char* fileName)
{
   bool success = true;
   mTextureSurface = SDL_LoadBMP( fileName );
   if( mTextureSurface == NULL )
   {
       std::cout << "Unable to load image " <<  fileName << " SDL Error: " << SDL_GetError() << std::endl;
       success = false;
   }

   return success;
}
