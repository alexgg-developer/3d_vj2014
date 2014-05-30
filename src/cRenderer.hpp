#pragma once
#include "GL/glew.h"
#include <GL/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "cGLShader.hpp"
#include "cTexture.hpp"
#include "cText.hpp"
#include "cTerrain.hpp"
#include "cCamera.hpp"
#include "cInput.hpp"
#include <iostream>

class Renderer
{
public:
  Renderer();
  bool initGL(const uint mWidth, const uint mHeight);
  bool initApp();
  void render();
  void free();
  static bool link(GLuint program);
  void updateProjection(const uint mWidth, const uint mHeight);
  Camera mCamera;
  glm::mat4 const& getProjMatrix() const { return mProjection; }
private:
  GLuint mProgramID;
  //std::vector<GLShader> mVertexShader;
  //std::vector<GLShader> mFragmentShader;
  //std::vector<GLuint> mVBO;
  //std::vector<GLuint> mIBO;
  std::vector<Texture> mTexture;
  std::vector<Text> mText;
  std::vector<Terrain> mTerrain;
  GLuint texUniform;
  glm::mat4 mProjection;
  bool link();
  static void printProgramLog(GLuint program);
};

