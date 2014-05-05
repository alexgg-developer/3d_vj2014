#include "cRenderer.hpp"

Renderer::Renderer(): mProgramID(0)
{
}

bool Renderer::initGL()
{
  bool success = true;

  mProgramID = glCreateProgram();
  GLShader defaultVertexShader;
  defaultVertexShader.init(GLShader::VERTEX, mProgramID);
  success = defaultVertexShader.compile();
  mVertexShader.push_back(defaultVertexShader);
  GLShader defaultFragmentShader;
  defaultFragmentShader.init(GLShader::FRAGMENT, mProgramID);
  success = defaultFragmentShader.compile();
  mFragmentShader.push_back(defaultFragmentShader);
  link();


  GLfloat vertexData[] =
  {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
  };
  GLuint indexData[] = { 0, 1, 2, 3 };
  GLuint vbo = 0;
  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );
  mVBO.push_back(vbo);


  GLuint ibo = 0;
  glGenBuffers( 1, &ibo );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );
  mIBO.push_back(ibo);

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

void Renderer::render()
{
  glClearColor( 0.f, 0.f, 0.f, 1.f );
  glClear( GL_COLOR_BUFFER_BIT );

  glUseProgram( mProgramID );
  GLuint attributeLocation = mVertexShader[0].setAttribute("LVertexPos2D");
  glEnableVertexAttribArray( attributeLocation );
  glBindBuffer( GL_ARRAY_BUFFER, mVBO[0] );
  glVertexAttribPointer( attributeLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO[0] );
  glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
  mVertexShader[0].unsetAttribute(attributeLocation);
  glUseProgram( NULL );
}

void Renderer::free()
{
  glDeleteProgram( mProgramID );
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
