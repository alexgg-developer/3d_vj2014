#include "cRenderer.hpp"

Renderer::Renderer(): mProgramID(0)
{
}

bool Renderer::initApp()
{
    bool success = true;

    /*texUniform = glGetUniformLocation(mProgramID, "teximg");
    std::cerr << "uniform tex: " << texUniform << std::endl;*/
    Texture tex;
    if (!tex.load("../resources/img/nehe.png")) {
        std::cerr << "ERROR! shiet tex" << std::endl;
        success = false;
    }
    else {
      mTexture.push_back(tex);
    }
    return success;
}

bool Renderer::initGL()
{
  bool success = true;
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glShadeModel( GL_SMOOTH );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR ) {
      std::cout << "Error initializing OpenGL!" << gluErrorString( error ) << std::endl;
      success = false;
  }

  return success;
}

bool Renderer::link()
{
  bool success = true;
  glLinkProgram( mProgramID );
  GLint programSuccess = GL_TRUE;
  glGetProgramiv( mProgramID, GL_LINK_STATUS, &programSuccess );
  if( programSuccess != GL_TRUE ) {
    std::cout << "Error linking program " << mProgramID << std::endl;
    printProgramLog( mProgramID );
    success = false;
  }

  return success;
}

bool Renderer::link(GLuint program)
{
  bool success = true;
  glLinkProgram( program );
  GLint programSuccess = GL_TRUE;
  glGetProgramiv( program, GL_LINK_STATUS, &programSuccess );
  if( programSuccess != GL_TRUE ) {
    std::cout << "Error linking program " << program << std::endl;
    printProgramLog( program );
    success = false;
  }

  return success;
}

void Renderer::render()
{
  //glUseProgram(NULL);   //Se podría crear un group de shaders y meter esto y enlazar el program a un vertex/fragment/geometry
  mTexture[0].draw();
  //glUseProgram(NULL);
}

void Renderer::free()
{
  for(uint i = 0; i < mTexture.size(); ++i) {
    mTexture[i].free();
  }
}

void Renderer::printProgramLog(GLuint program)
{
  if( glIsProgram( program ) ) {
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
    char* infoLog = new char[ maxLength ];
    glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
    if( infoLogLength > 0 ) {
      std::cout << infoLog  << std::endl;
    }
    delete[] infoLog;
  }
  else {
    std::cout << "Name " << program << " is not a program" << std::endl;
  }
}
