#include "cTexture.hpp"
#include <iostream>


Texture::Texture(): mTexture(0), mWidth(0), mHeight(0), mTextureSurface(nullptr), mProgramID(0)
{
}

Texture::~Texture()
{}

bool Texture::init()
{
  bool success = true;
  mProgramID = glCreateProgram();
  GLShader defaultVertexShader;
  defaultVertexShader.init(GLShader::VERTEX, mProgramID);
  success = defaultVertexShader.compile();
  mVertexShader.push_back(defaultVertexShader);
  GLShader defaultFragmentShader;
  defaultFragmentShader.init(GLShader::FRAGMENT, mProgramID);
  success = defaultFragmentShader.compile();
  mFragmentShader.push_back(defaultFragmentShader);
  success = GLShader::linkProgram(mProgramID);

  GLfloat vertexData[] =
  {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
  };

  GLuint indexData[] = { 0, 1, 2, 3 };

  GLuint vbo = 0;
  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );
  mVBO.push_back(vbo);

  GLuint ibo = 0;
  glGenBuffers( 1, &ibo );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );
  mIBO.push_back(ibo);

  glBindBuffer( GL_ARRAY_BUFFER, NULL ); glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR ) {
      std::cout << "Error initializing App!" << gluErrorString( error ) << std::endl;
      success = false;
  }
  return success;
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
       glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mTextureSurface->pixels );
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       glBindTexture( GL_TEXTURE_2D, 0 );
       //Check for error
       GLenum error = glGetError();
       if( error != GL_NO_ERROR ) {
           std::cout << "Error loading texture" << gluErrorString( error ) << std::endl;
           success = false;
       }
       success = init();
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
       success = init();
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
  glDeleteProgram( mProgramID );

  for(uint i = 0; i < mVBO.size(); ++i) {
    glDeleteBuffers( 1, &mVBO[0] );
  }
  for(uint i = 0; i < mIBO.size(); ++i) {
    glDeleteBuffers( 1, &mIBO[0] );
  }
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::draw()
{
  glUseProgram( mProgramID );
  bind();
  GLuint attributeLocation = mVertexShader[0].setAttribute("LVertexPos2D");
  glEnableVertexAttribArray( attributeLocation );
  glBindBuffer( GL_ARRAY_BUFFER, mVBO[0] );
  glVertexAttribPointer( attributeLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO[0] );
  glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
  mVertexShader[0].unsetAttribute(attributeLocation);
  unbind();
  glUseProgram( NULL );
}
