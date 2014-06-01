#pragma once
#include "SDL_ttf.h"
#include "SDL.h"
#include "TypesDefined.hpp"
#include "vec3.hpp"
#include "cGLShader.hpp"
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

class Text
{

public:
  Text();
  ~Text();
  bool init();
  bool loadFont(std::string path, uint size);
  bool loadText(std::string text, SDL_Color color, Quality q);
  void free();
  void draw() const;
  void setPosition(vec3 const & position);
  void setScale(glm::vec3 const & scale);
  uint mWidth, mHeight;
private:
  TTF_Font* mFont;
  GLuint mTexture;
  SDL_Surface* mTextureSurface;
  vec3 mPosition;
  glm::vec3 mScale;
  GLuint mProgramID;
  GLShader mVertexShader, mFragmentShader;
  GLuint mVBO, mIBO;
};
