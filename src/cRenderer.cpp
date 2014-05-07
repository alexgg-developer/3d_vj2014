#include "cRenderer.hpp"

Renderer::Renderer(): mProgramID(0)
{
}

bool Renderer::initGL()
{
  bool success = true;

  /*mProgramID = glCreateProgram();
  GLShader defaultVertexShader;
  defaultVertexShader.init(GLShader::VERTEX, mProgramID);
  success = defaultVertexShader.compile();
  mVertexShader.push_back(defaultVertexShader);
  GLShader defaultFragmentShader;
  defaultFragmentShader.init(GLShader::FRAGMENT, mProgramID);
  success = defaultFragmentShader.compile();
  mFragmentShader.push_back(defaultFragmentShader);
  link();*/


  /*GLfloat vertexData[] =
  {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
  };*/


  const uint SCREEN_WIDTH = 800;
  const uint SCREEN_HEIGHT = 600;

  LVertexPos2D gQuadVertices[ 4 ];

  /*gQuadVertices[ 0 ].x = SCREEN_WIDTH * 1.f / 30.f;
  gQuadVertices[ 0 ].y = SCREEN_HEIGHT * 1.f / 30.f;

  gQuadVertices[ 1 ].x = SCREEN_WIDTH * 3.f / 30.f;
  gQuadVertices[ 1 ].y = SCREEN_HEIGHT * 1.f / 30.f;

  gQuadVertices[ 2 ].x = SCREEN_WIDTH * 3.f / 30.f;
  gQuadVertices[ 2 ].y = SCREEN_HEIGHT * 3.f / 30.f;

  gQuadVertices[ 3 ].x = SCREEN_WIDTH * 1.f / 30.f;
  gQuadVertices[ 3 ].y = SCREEN_HEIGHT * 3.f / 30.f;*/

  gQuadVertices[ 0 ].x = 0.0f;
  gQuadVertices[ 0 ].y = 0.0f;

  gQuadVertices[ 1 ].x = SCREEN_WIDTH * 3.f / 30.f;
  gQuadVertices[ 1 ].y = SCREEN_HEIGHT * 1.f / 30.f;

  gQuadVertices[ 2 ].x = SCREEN_WIDTH * 3.f / 30.f;
  gQuadVertices[ 2 ].y = SCREEN_HEIGHT * 3.f / 30.f;

  gQuadVertices[ 3 ].x = SCREEN_WIDTH * 1.f / 30.f;
  gQuadVertices[ 3 ].y = SCREEN_HEIGHT * 3.f / 30.f;


  GLuint indexData[] = { 0, 1, 2, 3 };

 /* GLuint vbo = 0;
  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );*/
  GLuint gVertexBuffer;
  glGenBuffers( 1, &gVertexBuffer );
  glBindBuffer( GL_ARRAY_BUFFER, gVertexBuffer );
  glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(LVertexPos2D), gQuadVertices, GL_STATIC_DRAW );
  mVBO.push_back(gVertexBuffer);


  GLuint ibo = 0;
  glGenBuffers( 1, &ibo );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );
  mIBO.push_back(ibo);


  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glShadeModel( GL_SMOOTH );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

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

void Renderer::render()
{
  /*glUseProgram( mProgramID );
  GLuint attributeLocation = mVertexShader[0].setAttribute("LVertexPos2D");
  glEnableVertexAttribArray( attributeLocation );
  glBindBuffer( GL_ARRAY_BUFFER, mVBO[0] );
  glVertexAttribPointer( attributeLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO[0] );
  glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
  mVertexShader[0].unsetAttribute(attributeLocation);
  glUseProgram( NULL );*/
  glColor3f(1.0f, 0.0f, 0.0f);
  glEnableClientState( GL_VERTEX_ARRAY );
  glBindBuffer( GL_ARRAY_BUFFER, mVBO[0] );
  glVertexPointer( 2, GL_FLOAT, 0, NULL );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO[0] );
  glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );
  glDisableClientState( GL_VERTEX_ARRAY );
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
