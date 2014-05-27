#include "cTerrain.hpp"

Terrain::Terrain()
{
  mModelMatrix = glm::mat4(1.0f);
}

bool Terrain::init()
{
  bool success = true;
  mProgramID = glCreateProgram();
  mVertexShader.init(GLShader::VERTEX, mProgramID);
  success = mVertexShader.compile();
  if (success) {
    mFragmentShader.init(GLShader::FRAGMENT, mProgramID);
    success = mFragmentShader.compile();
    success = GLShader::linkProgram(mProgramID);
  }

  GLfloat vertexData[] =
  {
  -1.0f, 1.0f, 1.0f,
  -1.0f, -1.0f, 1.0f,
  1.0f, -1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, -1.0f,
  1.0f, 1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f
  };

  GLuint indexData[] =
  {
  3, 0, 1,
  1, 2, 3,
  0, 4, 7,
  7, 1, 0,
  4, 5, 6,
  6, 7, 4,
  5, 3, 2,
  2, 6, 5,
  2, 1, 7,
  7, 6, 2,
  5, 4, 0,
  0, 3, 5
  };

  std::vector<glm::vec3> normals;
  normals.push_back(glm::vec3(0, 0, 1));
  normals.push_back(glm::vec3(-1, 0, 0));
  normals.push_back(glm::vec3(0, 0, -1));
  normals.push_back(glm::vec3(1, 0, 0));
  normals.push_back(glm::vec3(0, -1, 0));
  normals.push_back(glm::vec3(0, 1, 0));

  glGenBuffers(1, &mVBO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, NULL);

  glGenBuffers(1, &mIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 12 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

  glGenBuffers(1, &mNBO);
  glBindBuffer(GL_ARRAY_BUFFER, mNBO);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, NULL);

  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Error initializing App!" << gluErrorString(error) << std::endl;
    success = false;
  }
  //Transform = T * R * S * V
  mModelMatrix = glm::scale(glm::vec3(20.0f, 0.25f, 20.0f)) * mModelMatrix;
  return success;
}

void Terrain::free()
{
  if (mVBO != 0) {
    glDeleteBuffers(1, &mVBO);
  }
  if (mIBO != 0) {
    glDeleteBuffers(1, &mIBO);
  } 
  if (mNBO != 0) {
    glDeleteBuffers(1, &mNBO);
  }
}

void Terrain::setMatrix(glm::mat4 &projectionMatrix, glm::mat4 &viewMatrix)
{
  glUseProgram(mProgramID);
  glm::mat4 MVP = projectionMatrix * viewMatrix  * mModelMatrix;
  glm::mat4 MV = viewMatrix * mModelMatrix;
  glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MV));
  GLuint MatrixID = glGetUniformLocation(mProgramID, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  MatrixID = glGetUniformLocation(mProgramID, "NormalMatrix");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &NormalMatrix[0][0]);
  glUseProgram(NULL);
}

void Terrain::render()
{
  glUseProgram(mProgramID);
  GLuint attributeLocation = mVertexShader.setAttribute("LVertexPos3D");
  glEnableVertexAttribArray(attributeLocation); 
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glVertexAttribPointer(attributeLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
  glBindBuffer(GL_ARRAY_BUFFER, NULL);
  attributeLocation = mVertexShader.setAttribute("vNormal");
  glEnableVertexAttribArray(attributeLocation);
  glBindBuffer(GL_ARRAY_BUFFER, mNBO);
  glVertexAttribPointer(attributeLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
  glBindBuffer(GL_ARRAY_BUFFER, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
  mVertexShader.unsetAttribute(attributeLocation);
  glUseProgram(NULL);
}

