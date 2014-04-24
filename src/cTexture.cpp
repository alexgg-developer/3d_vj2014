#include "cTexture.hpp"
#include <iostream>

Texture::Texture(): mTexture(NULL)
{}

Texture::~Texture()
{
  SDL_DestroyTexture( mTexture );
}

bool Texture::load(std::string fileName, SDL_Renderer * renderer )
{
   bool success = true;
   int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
   if( !( IMG_Init( imgFlags ) & imgFlags ) )  {
       std::cout <<  "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
       success = false;
   }
   else {
     SDL_Surface* textureSurface = IMG_Load( fileName.c_str() );
     if( textureSurface == NULL )
     {
         std::cout << "Unable to load image " <<  fileName.c_str() << " SDL Error: " << SDL_GetError() << std::endl;
         success = false;
     }
     else {
       mTexture = SDL_CreateTextureFromSurface(renderer, textureSurface);
       if( mTexture == NULL ) {
         std::cout << "Unable create texture " <<  fileName.c_str() << " SDL Error: " << SDL_GetError() << std::endl;
         success = false;
       }
     }
     SDL_FreeSurface( textureSurface );
   }

   return success;
}

