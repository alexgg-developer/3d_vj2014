#pragma once
#include "TypesDefined.hpp"
#include "cGLShader.hpp"

class Terrain
{
public:
  Terrain() {};
  bool init();
  void free();
  void render();
private:
  GLuint mVBO;
  GLuint mIBO;
  uint mWidth, mHeight;
  GLuint mProgramID;
  GLShader mVertexShader, mFragmentShader;
};