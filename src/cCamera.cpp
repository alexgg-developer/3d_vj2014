#include "cCamera.hpp"

Camera::Camera(glm::vec3 const &position, glm::vec3 const &lookAt, glm::vec3 const & up) : mPosition(position), mLookAt(lookAt), mUp(up) {}

void Camera::init(glm::vec3 const &position, glm::vec3 const &lookAt, glm::vec3 const & up) 
{
  mPosition = position;
  mLookAt = lookAt;
  mUp = up;
}

void Camera::getViewMatrix(glm::mat4& viewMatrix) const
{
  viewMatrix = glm::lookAt(mPosition, mLookAt, mUp);
}