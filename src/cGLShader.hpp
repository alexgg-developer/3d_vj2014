#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
#include "TypesDefined.hpp"

class GLShader
{
public:
  enum KindOfShader {VERTEX, FRAGMENT, GEOMETRY};
  uint mProgramID;
  GLShader();
  void init(KindOfShader kind, uint programID);
  void init(KindOfShader kind, std::string shaderSource, uint programID);
  void setSource(std::string shaderSource);
  GLuint setAttribute(std::string name);
  void unsetAttribute(GLuint attributeLocation);
  void printShaderLog( GLuint shader );
  bool compile();
private:
  uint mID;
  std::string mShaderSource;
};

