#include "cGLShader.hpp"

GLShader::GLShader()
{}

void GLShader::init(KindOfShader kind, uint programID)
{
  switch(kind) {
    case VERTEX: {
      mID = glCreateShader( GL_VERTEX_SHADER );
      mShaderSource = "#version 130\nin vec2 LVertexPos2D; out vec2 texcoords; void main() { texcoords = LVertexPos2D.xy + 0.5; texcoords.y = 1.0 - texcoords.y; gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }";
      const char* source = mShaderSource.c_str();
      glShaderSource( mID, 1, &source, NULL );
    }
    break;
    case FRAGMENT: {
      mID = glCreateShader( GL_FRAGMENT_SHADER );
      mShaderSource = "#version 130\nin vec2 texcoords; uniform sampler2D teximg; out vec4 LFragment; void main() { LFragment = vec4(texture(teximg, texcoords.xy).rgba); }";
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

bool GLShader::link(GLuint program)
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

bool GLShader::linkProgram(GLuint program)
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

void GLShader::printProgramLog(GLuint program)
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

GLuint GLShader::setAttribute(std::string name)
{
  GLuint attributeLocation = glGetAttribLocation( mProgramID, name.c_str() );
  if( attributeLocation == -1 ) {
    std::cout << name << " is not a valid glsl program variable!" << std::endl;
    std::cout << "Program: " << mProgramID << std::endl;
  }
  glEnableVertexAttribArray( attributeLocation );

  return attributeLocation;
}


void GLShader::unsetAttribute(GLuint attributeLocation )
{
  glDisableVertexAttribArray( attributeLocation );
}

