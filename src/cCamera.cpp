#include "cCamera.hpp"

Camera::Camera(glm::vec3 const &position, glm::vec3 const &lookAt, glm::vec3 const & up) : mPosition(position), mLookAt(lookAt), mUp(up), mVelocity(DEFAULT_VELOCITY) {}

void Camera::init(glm::vec3 const &position, glm::vec3 const &lookAt, glm::vec3 const & up) 
{
  mPosition = position;
  mLookAt = lookAt;
  mUp = up;
  mVelocity = DEFAULT_VELOCITY;
}

#include <iostream>
void Camera::getViewMatrix(glm::mat4& viewMatrix) const
{
  viewMatrix = glm::lookAt(mPosition, mLookAt, mUp);
  //std::cout << "getPosition: " << mPosition.x << std::endl;
  //std::cout << "getmLookAt: " << mLookAt.x << std::endl;
}

void Camera::pan(glm::vec3 const direction, float const dt)
{
  mPosition += direction * mVelocity * dt;
  mLookAt += direction * mVelocity * dt;
  glm::vec3 toMove = direction * mVelocity * dt;
  std::cout << "ToMove: " << " x: " << toMove.x << " y: " << toMove.y << " z: " << toMove.z << " DT: " << dt << std::endl;
  std::cout << "Vel: " << mVelocity << "Dir: " << " x: " << direction.x <<  " y: " << direction.y << " z: " << direction.z << " DT: " << dt << std::endl;
  std::cout << "Position: " << mPosition.x << "DT: " << dt << std::endl;
  std::cout << "mLookAt: " << mLookAt.x << std::endl;
}