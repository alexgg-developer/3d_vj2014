#include "Path.hpp"
#include <iostream>

Path::Path() { }
Path::~Path() { }

bool Path::Load(pugi::xml_node const& aPathNode) {
  mName = aPathNode.attribute("name").value();
  mStartPosition.x = static_cast<float>(aPathNode.child("start_point").attribute("x").as_float());
  mStartPosition.y = static_cast<float>(aPathNode.child("start_point").attribute("y").as_int());
  
  
  for(pugi::xml_node const& aOrderNode : aPathNode.children("order")) {
    Order o;
    if(!o.Load(aOrderNode)) {
      std::cout << "Could not load order on path" << std::endl;
      assert(0);
      return false;
    }
    mOrders.push_back(o);
  }
  return true;
}


Path::Order::Order() {}
Path::Order::~Order() {}
bool Path::Order::Load(pugi::xml_node const& aOrderNode) {
  std::string code = aOrderNode.attribute("code").value();
  if(code=="up") mOrderType=OrderType::UP;
  else if(code=="down") mOrderType=OrderType::DOWN;
  else if(code=="left") mOrderType=OrderType::LEFT;
  else if(code=="right") mOrderType=OrderType::RIGHT;
  else if(code=="attack-base") mOrderType=OrderType::ATTACK_BASE;
  else {
    std::cout << "unknown OrderType code: " << code << std::endl;
    assert(0);
    return false;
  }

  pugi::xml_attribute attr = aOrderNode.attribute("quantity");
  if(attr) {
    mQuantity = attr.as_int();
    mIndefinitely=false;
  } else {
    mQuantity=-1;
    mIndefinitely=true;
  }
  return true;
}

#include "Enemy.hpp"
#include "Map.hpp"
PathLogic::PathLogic(Path const*const aPath, Defensor *const aDefensor, Map const*const aMap)
  : mPath(aPath), mDefensor(aDefensor), mMap(aMap) {}
PathLogic::~PathLogic() {}

void PathLogic::assignEnemy(EnemyLogic* el) {
  mControlledEnemies.push_back(EnemyMoving(el));
  mControlledEnemies.back().enemy->setPosition(mPath->mStartPosition);
  ApplyNextOrderTo(mControlledEnemies.back());
}
void PathLogic::advance_time(float const init_time_ms, float const dt_ms) {
  for(EnemyMoving& em : mControlledEnemies) {
    glm::vec2 const deltaPos = em.mVelocity*dt_ms/1000.0f;
    glm::vec2 const newPosition = em.enemy->getPosition() + deltaPos;
    //TODO: Orientation
    int const nextX = static_cast<int>(newPosition.x + em.mVelocity.x*0.25f);
    int const nextY = static_cast<int>(newPosition.y /*+ em.mVelocity.y*0.25f*/);
    bool next_order = !mMap->EnemyCanBeIn(nextX,nextY);
    if(mPath->mOrders[em.mActualOrder].mIndefinitely==false) {
      //Check if enemy has advanced more than the specified quantity
      float const maxQuantity = static_cast<float>(mPath->mOrders[em.mActualOrder].mQuantity);
      auto const delta = em.mPositionWhenStartedOrder - em.enemy->getPosition();
      float const actual_separation_squared = delta.x*delta.x + delta.y*delta.y;
      if(actual_separation_squared >= maxQuantity*maxQuantity)
        next_order = true;
    }
    if(!next_order) {
      //std::cout << "Changing position to " << newPosition.x << newPosition.y << std::endl;
      em.enemy->setPosition(newPosition);
    } else {
      //Next order
      ApplyNextOrderTo(em);
    }
  }
  //Delete finished enemies. TODO: Move them to a non-attacking exit path
  for(std::vector<EnemyMoving>::iterator it = mControlledEnemies.begin(); it!=mControlledEnemies.end();) {
    if(it->mPathFinished) {
      std::cout << "an enemy died" << std::endl;
      it = mControlledEnemies.erase(it);
    }
    else ++it;
  }
}
void PathLogic::ApplyNextOrderTo(EnemyMoving& em) {
  if(em.mActualOrder==mPath->mOrders.size()-1) {
    em.mPathFinished=true;//finished this path
  } else {
    em.mActualOrder++;
    em.mPositionWhenStartedOrder = em.enemy->getPosition();
    Path::Order const& ord = mPath->mOrders[em.mActualOrder];
    if(ord.mOrderType==Path::Order::OrderType::DOWN) em.mVelocity=glm::vec2(0,-1);
    else if(ord.mOrderType==Path::Order::OrderType::UP) em.mVelocity=glm::vec2(0,1);
    else if(ord.mOrderType==Path::Order::OrderType::LEFT) em.mVelocity=glm::vec2(-1,0);
    else if(ord.mOrderType==Path::Order::OrderType::RIGHT) em.mVelocity=glm::vec2(1,0);
    else if(ord.mOrderType==Path::Order::OrderType::ATTACK_BASE) {
      em.mVelocity = glm::vec2(0, 0); //Do not move
      em.enemy->Attack(*mDefensor);  //Attacks prota
      em.mPathFinished=true;//attack only once
    }
    else { 
      std::cout << "Order type not recognized " << ord.mOrderType << std::endl;
      assert(0);
    }
  }
}

PathLogic::EnemyMoving::EnemyMoving(EnemyLogic* el) : enemy(el) {}