#include "cTexture.hpp"
#include <iostream>


Texture::Texture(): mTexture(0), mWidth(0), mHeight(0), mTextureSurface(nullptr)
{}

Texture::~Texture()
{
  free();
}

bool Texture::load(std::string fileName)
{
   bool success = true;
   free();
   int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
   if( !( IMG_Init( imgFlags ) & imgFlags ) )  {
       std::cout <<  "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
       success = false;
   }
   else {
     mTextureSurface = IMG_Load( fileName.c_str() );
     if( mTextureSurface == nullptr ) {
         std::cout << "Unable to load image " <<  fileName.c_str() << " SDL Error: " << SDL_GetError() << std::endl;
         success = false;
     }
     else {
       mWidth  = mTextureSurface->w;
       mHeight = mTextureSurface->h;
       glGenTextures( 1, &mTexture );
       glBindTexture( GL_TEXTURE_2D, mTexture );
       glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mTextureSurface->pixels );
       glBindTexture( GL_TEXTURE_2D, 0 );
       //Check for error
       GLenum error = glGetError();
       if( error != GL_NO_ERROR ) {
           std::cout << "Error loading texture" << gluErrorString( error ) << std::endl;
           success = false;
       }
       /* glBindTexture( GL_TEXTURE_2D, mTexture );
       GLuint size = mWidth * mHeight;
       GLuint* mPixels = new GLuint[ size ];
       glGetTexImage( GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, mPixels );

       //Unbind texture
       glBindTexture( GL_TEXTURE_2D, NULL );*/

     }
   }

   return success;
}

bool Texture::load(std::string fileName, vec3 const & colorKey )
{
   bool success = true;
   free();
   int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
   if( !( IMG_Init( imgFlags ) & imgFlags ) )  {
       std::cout <<  "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
       success = false;
   }
   else {
     mTextureSurface = IMG_Load( fileName.c_str() );
     if( mTextureSurface == nullptr ) {
         std::cout << "Unable to load image " <<  fileName.c_str() << " SDL Error: " << SDL_GetError() << std::endl;
         success = false;
     }
     else {
       SDL_SetColorKey( mTextureSurface, SDL_TRUE, SDL_MapRGB( mTextureSurface->format, colorKey.x, colorKey.y, colorKey.z ) );
       mWidth  = mTextureSurface->w;
       mHeight = mTextureSurface->h;
       glGenTextures( 1, &mTexture );
       glBindTexture( GL_TEXTURE_2D, mTexture );
       glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTextureSurface->pixels );
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glBindTexture( GL_TEXTURE_2D, 0 );
       //Check for error
       GLenum error = glGetError();
       if( error != GL_NO_ERROR ) {
           std::cout << "Error loading texture" << gluErrorString( error ) << std::endl;
           success = false;
       }
     }
   }

   return success;
}

void Texture::free() {  
  if( mTexture != 0 ) {
    glDeleteTextures( 1, &mTexture );
    mTexture = 0;
  }
  SDL_FreeSurface( mTextureSurface );
  mTextureSurface = nullptr;
}

void Texture::draw()
{
  glBindTexture( GL_TEXTURE_2D, mTexture );
  glBegin( GL_QUADS );
    glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f,            0.f );
    glTexCoord2f( 1.f, 0.f ); glVertex2f( mWidth,            0.f );
    glTexCoord2f( 1.f, 1.f ); glVertex2f( mWidth, mHeight );
    glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f, mHeight );
  glEnd();
  glBindTexture( GL_TEXTURE_2D, 0 );
}
