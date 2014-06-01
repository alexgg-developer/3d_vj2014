#pragma once

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
  GLuint setAttribute(std::string name) const;
  void unsetAttribute(GLuint attributeLocation) const;
  void printShaderLog( GLuint shader );
  bool compile();
  void set();
  bool link(GLuint program);
  static bool linkProgram(GLuint program);
  static void printProgramLog(GLuint program);
private:
  uint mID;
  std::string mShaderSource;
};

