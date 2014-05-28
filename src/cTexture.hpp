#pragma once
#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "vec3.hpp"
#include "cGLShader.hpp"


class Texture {
public:
  Texture();
  ~Texture();
  bool load(std::string fileName);
  bool load(std::string fileName, vec3 const & colorKey );
  void bind();
  void unbind();
  void draw();
  void free();

  GLuint mTexture;
  SDL_Surface* mTextureSurface;
  int mWidth, mHeight;
private:
  GLuint mProgramID;
  std::vector<GLuint> mVBO;
  std::vector<GLuint> mIBO;
  std::vector<GLShader> mVertexShader;
  std::vector<GLShader> mFragmentShader;
  bool init();
  bool isBMP(std::string fileName);
};
