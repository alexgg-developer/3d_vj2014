#pragma once
#include "GL/glew.h"
#include <GL/gl.h>
#include <vector>
#include "cGLShader.hpp"
#include "cTexture.hpp"
#include "cText.hpp"

class Renderer
{
public:
  Renderer();
  bool initGL();
  bool initApp();
  void render();
  void free();
  static bool link(GLuint program);
private:
  GLuint mProgramID;
  //std::vector<GLShader> mVertexShader;
  //std::vector<GLShader> mFragmentShader;
  //std::vector<GLuint> mVBO;
  //std::vector<GLuint> mIBO;
  std::vector<Texture> mTexture;
  std::vector<Text> mText;

  GLuint texUniform;

  bool link();
  static void printProgramLog(GLuint program);
};

