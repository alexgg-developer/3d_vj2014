#include "cText.hpp"
#include <iostream>

Text::Text() : mFont(nullptr), mTexture(0), mWidth(0), mHeight(0), mPosition(vec3(0, 0)), mTextureSurface(nullptr), mProgramID(0), mIBO(0), mVBO(0)
{
}

Text::~Text()
{
  //free();
}

bool Text::init()
{
  bool success = true;
  if(mProgramID==0) {
    mProgramID = glCreateProgram();
    mVertexShader.init(GLShader::VERTEX, mProgramID);
    success = mVertexShader.compile();
    mFragmentShader.init(GLShader::FRAGMENT, mProgramID);
    success = mFragmentShader.compile();
    success = GLShader::linkProgram(mProgramID);

    GLfloat vertexData[] =
    {
      -0.5f, -0.5f,
      0.5f, -0.5f,
      0.5f, 0.5f,
      -0.5f, 0.5f
    };

    GLuint indexData[] = { 0, 1, 2, 3 };

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, NULL); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
  }
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Error initializing App!" << gluErrorString(error) << std::endl;
    success = false;
  }

  return success;
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
  if (mTextureSurface != nullptr) {
    SDL_FreeSurface(mTextureSurface);
    mTextureSurface = nullptr;
  }
  switch(q) {
    case LOW:
      mTextureSurface = TTF_RenderText_Solid( mFont, text.c_str(), color);
    break;
    case MED: {
      SDL_Color bg = {1, 1, 1, 1};
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
    
    if( mTexture == 0 ) {
      glGenTextures( 1, &mTexture );
      glBindTexture(GL_TEXTURE_2D, mTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, mTextureSurface->pixels);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
    glBindTexture(GL_TEXTURE_2D, NULL);

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ) {
        std::cout << "Error loading texture" << gluErrorString( error ) << std::endl;
        success = false;
    }
    success = init();
  }

  return success;
}

void Text::free()
{
  if( mTexture != 0 ) {
    glDeleteTextures( 1, &mTexture );
    mTexture = 0;
  }
  if (mVBO != 0) {
    glDeleteBuffers(1, &mVBO);
  }
  if (mIBO != 0) {
    glDeleteBuffers(1, &mIBO);
  }
  if (mTextureSurface != nullptr) {
    SDL_FreeSurface(mTextureSurface);
    mTextureSurface = nullptr;
  }
  if (mFont != nullptr) {
    TTF_CloseFont(mFont);
    mFont = nullptr;
  }
}

void Text::setPosition(vec3 const & position)
{
  mPosition = position;
}

void Text::setScale(glm::vec3 const & scale)
{
  mScale = scale;
}

void Text::draw() const
{
  glDisable(GL_DEPTH_TEST);
  glUseProgram(mProgramID);
  glm::mat4 modelMatrix = glm::translate(glm::vec3(mPosition.x, mPosition.y, 0)) * glm::scale(mScale);
  GLuint MatrixID = glGetUniformLocation(mProgramID, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  GLuint attributeLocation = mVertexShader.setAttribute("LVertexPos2D");
  glEnableVertexAttribArray(attributeLocation);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glVertexAttribPointer(attributeLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
  mVertexShader.unsetAttribute(attributeLocation);
  glBindTexture(GL_TEXTURE_2D, NULL);
  glUseProgram(NULL);
  glEnable(GL_DEPTH_TEST);
}
