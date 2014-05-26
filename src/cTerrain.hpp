#pragma once
#include "TypesDefined.hpp"
#include "cGLShader.hpp"
#include <glm/glm.hpp>

class Terrain
{
public:
  Terrain();
  bool init();
  void free();
  void setMatrix(glm::mat4 &projectionMatrix, glm::mat4 &viewMatrix);
  void render();
private:
  GLuint mVBO;
  GLuint mIBO;
  uint mWidth, mHeight;
  GLuint mProgramID;
  GLShader mVertexShader, mFragmentShader;
  glm::mat4 mModelMatrix;
};