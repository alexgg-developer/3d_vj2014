#include "cCamera.hpp"

Camera::Camera(glm::vec3 const &position, glm::vec3 const &lookAt, glm::vec3 const & up) : mPosition(position), mLookAt(lookAt), mUp(up), mVelocity(DEFAULT_VELOCITY) {
  mTranslation = glm::mat4(1.0f);
}

void Camera::init(glm::vec3 const &position, glm::vec3 const &lookAt, glm::vec3 const & up) 
{
  mPosition = position;
  mLookAt = lookAt;
  mUp = up;
  mVelocity = DEFAULT_VELOCITY;
  mTranslation = glm::mat4(1.0f);
}

#include <iostream>
glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(mPosition, mLookAt, mUp) * mTranslation;;
}
void Camera::getViewMatrix(glm::mat4& viewMatrix) const
{
  viewMatrix = glm::lookAt(mPosition, mLookAt, mUp) * mTranslation;
  //std::cout << "getPosition: " << mPosition.x << std::endl;
  //std::cout << "getmLookAt: " << mLookAt.x << std::endl;
}

void Camera::pan(glm::vec3 const direction, float const dt)
{
  glm::vec3 toMove = direction * mVelocity * dt;
  /*glm::vec3 camaraDirection = mLookAt - mPosition;
  //std::cout << "Dir: " << " camx: " << camaraDirection.x << " camy: " << camaraDirection.y << " camz: " << camaraDirection.z << " DT: " << dt << std::endl;
  camaraDirection = glm::normalize(camaraDirection);
  //std::cout << "Dir: " << " camx: " << camaraDirection.x << " camy: " << camaraDirection.y << " camz: " << camaraDirection.z << " DT: " << dt << std::endl;
  glm::vec3 rightDirection = glm::cross(camaraDirection, mUp);
  glm::vec3 upDirection = glm::cross(camaraDirection, rightDirection);
  glm::vec3 movement = rightDirection * toMove + upDirection * toMove;
  mPosition += (movement);
  mLookAt += (movement);
  std::cout << "ToMove: " << " x: " << movement.x << " y: " << movement.y << " z: " << movement.z << " DT: " << dt << std::endl;
  std::cout << "Vel: " << mVelocity << "Dir: " << " x: " << direction.x <<  " y: " << direction.y << " z: " << direction.z << " DT: " << dt << std::endl;
  std::cout << "Position: " << mPosition.x << "DT: " << dt << std::endl;
  std::cout << "mLookAt: " << mLookAt.x << std::endl;*/
  mTranslation = glm::translate(mTranslation, toMove);

}