#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include "cGLShader.hpp"

class Renderer
{
public:
  Renderer();
  bool initGL();
  void render();
  void free();
private:
  GLuint mProgramID;
  std::vector<GLShader> mVertexShader;
  std::vector<GLShader> mFragmentShader;
  std::vector<GLuint> mVBO;
  std::vector<GLuint> mIBO;
  bool link();
  void printProgramLog(GLuint program);

  struct LVertexPos2D
  {
      GLfloat x;
      GLfloat y;
  };
};

