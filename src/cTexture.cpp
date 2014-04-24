#include "cTexture.hpp"
#include <iostream>


Texture::Texture(): mTexture(NULL), mWidth(0), mHeight(0)
{}

Texture::~Texture()
{
  free();
}

bool Texture::load(std::string fileName, SDL_Renderer * renderer )
{
   bool success = true;
   free();
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
       mWidth  = textureSurface->w;
       mHeight = textureSurface->h;
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

bool Texture::load(std::string fileName, SDL_Renderer * renderer, vec3 const & colorKey )
{
   bool success = true;
   free();
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
       SDL_SetColorKey( textureSurface, SDL_TRUE, SDL_MapRGB( textureSurface->format, colorKey.x, colorKey.y, colorKey.z ) );
       mWidth  = textureSurface->w;
       mHeight = textureSurface->h;
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

void Texture::free() {
    SDL_DestroyTexture( mTexture );
}
