#pragma once
#include "Defensor.hpp"
#include <GL/GLU.h>

Defensor::~Defensor() {}

glm::vec4 MouseToWorld(glm::vec4 const& p, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix) {
  return glm::inverse(aMVMatrix) * glm::inverse(aProjectionMatrix) * p;
}

glm::vec3 IntersectionWithYE0(glm::vec2 const aMousePositionXY, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix) {
  GLint viewport[4];
  GLdouble mvmatrix[16], projmatrix[16];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
  GLint realy = viewport[3] - (GLint)aMousePositionXY.y-1;
  GLint x = aMousePositionXY.x;

  //near point
  GLdouble nearX, nearY, nearZ;
  gluUnProject(x,realy,0.0,mvmatrix,projmatrix,viewport, &nearX, &nearY, &nearZ);
  //far point
  GLdouble farX, farY, farZ;
  gluUnProject(x,realy,1.0,mvmatrix,projmatrix,viewport, &farX, &farY, &farZ);
  
  glm::vec3 aMousePos_World(nearX, nearY, nearZ);
  glm::vec3 aMousePos_WorldFar(farX, farY, farZ);


  //Translate clic point to world coordinates
  /*glm::vec4 const aMousePosition(aMousePositionXY.x, aMousePositionXY.y,0,1);
  glm::vec4 const aMousePositionFar(aMousePositionXY.x, aMousePositionXY.y,1,1);

  glm::vec4 aMousePos_World    = MouseToWorld(aMousePosition   , aProjectionMatrix, aMVMatrix);
  aMousePos_World /= aMousePos_World.w;
  glm::vec4 aMousePos_WorldFar = MouseToWorld(aMousePositionFar, aProjectionMatrix, aMVMatrix);
  aMousePos_WorldFar /= aMousePos_WorldFar.w;*/

  glm::vec3 const directionClicked = glm::normalize(aMousePos_WorldFar-aMousePos_World);
  ///Line-plane intersection
  float const t = -aMousePos_World.y / directionClicked.y;
  glm::vec3 const clickedEarth(aMousePos_World.x + t * directionClicked.x, 0.0f, aMousePos_World.z + t * directionClicked.z);
  
    std::cout << "clicked on " << aMousePos_World.x << ", " << aMousePos_World.y << ", " << aMousePos_World.z
     << " to direction " << directionClicked.x << ", " << directionClicked.y << ", " << directionClicked.z << std::endl;
  return clickedEarth;
}

void Defensor::receive_input(Input& in, LevelLogic& ll, glm::mat4x4 const& aProjectionMatrix, glm::mat4x4 const& aMVMatrix, std::vector<Turret> const& aTurrets, std::vector<Weapon> const& aWeapons) {
  if (in.checkMouse(Input::BLEFT)) {
    glm::vec3 const clickedEarth = IntersectionWithYE0(glm::vec2(in.mPositionMousePressed.x, in.mPositionMousePressed.y), aProjectionMatrix, aMVMatrix);
    //user has clicked: show a visualization of the will-be-bought tower

    std::cout << "Earth clic is on " << clickedEarth.x << ", " << clickedEarth.y << ", " << clickedEarth.z << std::endl;
  } else if(in.checkMouseReleased(Input::BLEFT)) {
    glm::vec3 const clickedEarth = IntersectionWithYE0(glm::vec2(in.mPositionMousePressed.x, in.mPositionMousePressed.y), aProjectionMatrix, aMVMatrix);
    //user has released button: try to purchase
    in.UseMouseLastRelease(Input::BLEFT);

    glm::vec2 const tileCoordinates(-clickedEarth.x+0.5f,-clickedEarth.z+0.5f);
    if (ll.getMap()(tileCoordinates.x, tileCoordinates.y)==Map::TileType::BUILDABLE) {
      std::cout << "Build a turret on tile " << (int)tileCoordinates.x << ", " << (int)tileCoordinates.y << std::endl;
 
      //Instantiate one turret
      if(this->mMoney>=aTurrets[0].MonetaryCost()) {
        this->mMoney -= aTurrets[0].MonetaryCost();
        TurretLogic tl(&aTurrets[0], &aWeapons[0]);
        tl.setPosition(glm::vec2(tileCoordinates.x-0.5f,tileCoordinates.y-0.5f));
        ll.spawnsTurret(std::move(tl));
      } else {
        std::cout << "Do not have enough money to buy" << std::endl;
      }
    } else {
      std::cout << "Can't build a turret on " << (int)tileCoordinates.x << ", " << (int)tileCoordinates.y << std::endl;
    }

    std::cout << "Earth release is on " << clickedEarth.x << ", " << clickedEarth.y << ", " << clickedEarth.z << std::endl;
  }
}