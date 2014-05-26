#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
  Camera() {};
  Camera(glm::vec3 const &position, glm::vec3 const &lookAt, glm::vec3 const &up);
  void init(glm::vec3 const &position, glm::vec3 const &lookAt, glm::vec3 const &up);
  void getViewMatrix(glm::mat4& viewMatrix) const;
private:
  glm::vec3 mPosition, mLookAt, mUp;
};