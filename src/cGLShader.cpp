#include "cGLShader.hpp"

GLShader::GLShader()
{}

void GLShader::init(KindOfShader kind, uint programID)
{
  switch(kind) {
    case VERTEX: {
      mID = glCreateShader( GL_VERTEX_SHADER );
      mShaderSource = "#version 130\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }";
      const char* source = mShaderSource.c_str();
      glShaderSource( mID, 1, &source, NULL );
    }
    break;
    case FRAGMENT: {
      mID = glCreateShader( GL_FRAGMENT_SHADER );
      mShaderSource = "#version 130\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 0.0, 0.0, 1.0 ); }";
      const char* source = mShaderSource.c_str();
      glShaderSource( mID, 1, &source, NULL );
    }
    break;
    case GEOMETRY:
      mID = glCreateShader( GL_GEOMETRY_SHADER );
    break;
  }
  mProgramID = programID;
}

void GLShader::setSource(std::string shaderSource)
{
  mShaderSource = shaderSource;
  const char* source = mShaderSource.c_str();
  glShaderSource( mID, 1, &source, NULL );
}

void GLShader::printShaderLog( GLuint shader )
{
  if( glIsShader( shader ) ) {
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
    char* infoLog = new char[ maxLength ];
    glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
    if( infoLogLength > 0 ) {
      std::cout << infoLog << std::endl;
    }
    delete[] infoLog;
  }
  else {
    std::cout << "Name " << shader << "is not a shader " << std::endl;
  }
}

bool GLShader::compile()
{
  bool success = true;
  glCompileShader( mID );
  GLint shaderCompiled = GL_FALSE;
  glGetShaderiv( mID, GL_COMPILE_STATUS, &shaderCompiled );
  if( shaderCompiled != GL_TRUE ) {
    std::cout << "Unable to compile shader " << mID << std::endl;
    printShaderLog( mID );
    success = false;
  }
  else {
    glAttachShader( mProgramID, mID );
  }

  return success;
}

GLuint GLShader::setAttribute(std::string name)
{
  GLuint attributeLocation = glGetAttribLocation( mProgramID, name.c_str() );
  if( attributeLocation == -1 ) {
    std::cout << name << " is not a valid glsl program variable!" << std::endl;
  }
  glEnableVertexAttribArray( attributeLocation );

  return attributeLocation;
}


void GLShader::unsetAttribute(GLuint attributeLocation )
{
  glDisableVertexAttribArray( attributeLocation );
}

